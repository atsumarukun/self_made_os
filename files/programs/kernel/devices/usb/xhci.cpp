#include "xhci.hpp"
#include "device_context.hpp"
#include "ring.hpp"

#define DEVICE_SLOTS 8

HostController::HostController(uintptr_t xhc_mmio_address, MemoryManager& memory_manager, int device_num, FrameBufferWriter& writer):
                                                            xhc_mmio_address_{xhc_mmio_address},
                                                            capability_registers_{(CapabilityRegisters*) xhc_mmio_address},
                                                            operational_registers_{(OperationalRegisters*) (xhc_mmio_address + capability_registers_->CAPLENGTH)},
                                                            hcsparams1_{(uint32_t) capability_registers_->HCSPARAMS1.data} {
    volatile USBCMDMap* usbcmd = &operational_registers_->USBCMD;
    volatile USBSTSMap* usbsts = &operational_registers_->USBSTS;
    CONFIGMap* config = &operational_registers_->CONFIG;
    uint64_t* dcbaap = &operational_registers_->DCBAAP;
    HCSPARAMS2Map* hcsparams2 = &capability_registers_->HCSPARAMS2;
    CRCRMap* crcr = &operational_registers_->CRCR;

    if (!usbsts->bits.HCH) {
        usbcmd->bits.RS = 0;
    }
    while (!usbsts->bits.HCH);

    usbcmd->bits.HCRST = 1;
    while (usbcmd->bits.HCRST);
    while (usbsts->bits.CNR);

    int max_device_slots = capability_registers_->HCSPARAMS1.bits.MaxSlots;
    int device_slots = max_device_slots < DEVICE_SLOTS? max_device_slots: DEVICE_SLOTS;
    config->bits.MaxSlotsEn = device_slots;

    uint16_t max_scratchpad_buffers = hcsparams2->bits.MaxScratchpadBufsHi << 5 | hcsparams2->bits.MaxScratchpadBufsLo;
    uintptr_t scratchpad_buffer_array[max_scratchpad_buffers];
    for (int i = 0; i < max_scratchpad_buffers; i++) {
        scratchpad_buffer_array[i] = memory_manager.Allocate(1).value;
    }
    DeviceContext* dcbaa[device_slots];
    for (int i = 0; i < device_slots; i++) {
        dcbaa[i] = nullptr;
    }
    dcbaa[0] = (DeviceContext*) scratchpad_buffer_array;
    *dcbaap = (uint64_t) &dcbaa;

    RingManager cr(device_num, memory_manager);
    crcr->bits.RCS = 1;
    crcr->bits.CS = 0;
    crcr->bits.CA = 0;
    crcr->bits.CRP = (uint64_t) cr.Buffer() << 6;

    InterrupterRegisterSet* interrupter_register_sets = (InterrupterRegisterSet*) (xhc_mmio_address_ + (capability_registers_->RTSOFF | 0x00000) + 0x20);
    EventRingManager er(device_num, interrupter_register_sets, memory_manager);

    interrupter_register_sets->IMAN = 3;

    usbcmd->bits.INTE = 1;

    usbcmd->bits.RS = 1;
    while (usbsts->bits.HCH);
}

uintptr_t XhcMmioBaseAddress(PCI& pci_devices) {
    Device* xhc;
    for (int i = 0; i < pci_devices.devices.size(); i++) {
        if (pci_devices.devices[i].class_code.base_class == 0x0cu && pci_devices.devices[i].class_code.sub_class == 0x03u && pci_devices.devices[i].class_code.interface == 0x30u){
            xhc = &pci_devices.devices[i];
            if (pci_devices.devices[i].vendor_id == 0x8086u) break;
        }
    }

    const auto [xhc_mmio_base_address, error] = pci_devices.ReadBar(*xhc, 0);
    return xhc_mmio_base_address & ~(uint64_t)0xfu;
}
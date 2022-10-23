#include "xhci.hpp"
#include "memory.hpp"
#include "context.hpp"

HostController::HostController(uintptr_t mmio_base_address, uint8_t device_num):
                                            mmio_base_address_{mmio_base_address},
                                            capability_registers_{(CapabilityRegisters*) mmio_base_address_},
                                            operational_registers_{(OperationalRegisters*) (mmio_base_address_ + capability_registers_->CAPLENGTH)},
                                            max_ports_{(uint8_t) capability_registers_->HCSPARAMS1.Read().bits.MaxPorts} {
    device_manager_.Initialize(device_num);

    USBCMDMap usbcmd = operational_registers_->USBCMD.Read();

    if (!operational_registers_->USBSTS.Read().bits.HCH) {
        usbcmd.bits.RS = 0;
        operational_registers_->USBCMD.Write(usbcmd);
        while (!operational_registers_->USBSTS.Read().bits.HCH);
    }

    usbcmd = operational_registers_->USBCMD.Read();
    usbcmd.bits.HCRST = 1;
    operational_registers_->USBCMD.Write(usbcmd);
    while (operational_registers_->USBCMD.Read().bits.HCRST);
    while (operational_registers_->USBSTS.Read().bits.CNR);

    CONFIGMap config = operational_registers_->CONFIG.Read();
    config.bits.MaxSlotsEn = device_num;
    operational_registers_->CONFIG.Write(config);

    HCSPARAMS2Map hcsparams2 = capability_registers_->HCSPARAMS2.Read();
    uint16_t max_scratchpad_buffers = hcsparams2.bits.MaxScratchpadBufsHi << 5 | hcsparams2.bits.MaxScratchpadBufsLo;
    if (max_scratchpad_buffers) {
        void** scratchpad_buffers = (void**) Allocate(sizeof(void*) * max_scratchpad_buffers, 64, 4096);
        for (int i = 0; i < max_scratchpad_buffers; i++) {
            scratchpad_buffers[i] = Allocate(4096, 4096, 4096);
        }
        device_manager_.DeviceContexts()[0] = (DeviceContext*) scratchpad_buffers;
    }

    DCBAAPMap dcbaap = operational_registers_->DCBAAP.Read();
    dcbaap.bits.DCBAAP = (uint64_t) device_manager_.DeviceContexts() >> 6;
    operational_registers_->DCBAAP.Write(dcbaap);

    cr_.Initialize(32);
    CRCRMap crcr = operational_registers_->CRCR.Read();
    crcr.bits.RCS = 1;
    crcr.bits.CS = 0;
    crcr.bits.CA = 0;
    crcr.bits.CRP = (uint64_t) cr_.Buffer() >> 6;
    operational_registers_->CRCR.Write(crcr);
    InterrupterRegisterSet* interrupter = &InterrupterRegisterSets()[0];
    er_.Initialize(32, interrupter);

    interrupter->IMAN = 0x3u;

    usbcmd = operational_registers_->USBCMD.Read();
    usbcmd.bits.INTE = 1;
    operational_registers_->USBCMD.Write(usbcmd);

    usbcmd = operational_registers_->USBCMD.Read();
    usbcmd.bits.RS = 1;
    operational_registers_->USBCMD.Write(usbcmd);
    while (operational_registers_->USBSTS.Read().bits.HCH);
}

uint8_t HostController::MaxPorts() const {
    return max_ports_;
}

Port HostController::PortAt(uint8_t port_num) {
    return Port(port_num, &PortRegisterSets()[port_num]);
}

void HostController::ConfigurePort(Port& port) {
    if (port.Status() == PortStatus::NotConnected) {
        ResetPort(port);
    }
}

uint64_t HostController::ProcessEvent() {
    return er_.HasEvent();
}

Array<InterrupterRegisterSet> HostController::InterrupterRegisterSets() const {
    return Array<InterrupterRegisterSet>(mmio_base_address_ + capability_registers_->RTSOFF + 0x20u, 1024);
}

Array<PortRegisterSet> HostController::PortRegisterSets() const {
    return Array<PortRegisterSet>((uintptr_t) operational_registers_ + 0x400u, max_ports_);
}

void HostController::ResetPort(Port& port) {
    if (!port.IsConnected()) return;
    if (addressing_port_) {
        port.SetStatus(PortStatus::WaitingAddress);
    } else {
        if (port.Status() != PortStatus::NotConnected && port.Status() != PortStatus::WaitingAddress) return;
        addressing_port_ = port.Number();
        port.SetStatus(PortStatus::ResettingPort);
        port.Reset();
    }
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
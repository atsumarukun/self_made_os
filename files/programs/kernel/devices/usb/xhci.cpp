#include "xhci.hpp"

#include <stdio.h>

HostController::HostController(uintptr_t xhc_mmio_address, FrameBufferWriter& writer): xhc_mmio_address_{xhc_mmio_address},
                                                            capability_registers_{(CapabilityRegisters*) xhc_mmio_address},
                                                            operational_registers_{(OperationalRegisters*) (xhc_mmio_address + capability_registers_->CAPLENGTH)} {
    USBCMDMap* usbcmd = &operational_registers_->USBCMD;
    USBSTSMap* usbsts = &operational_registers_->USBSTS;

    if (!usbsts->bits.HCH) {
        usbcmd->bits.RS = 0;
    }
    while (!usbsts->bits.HCH);

    usbcmd->bits.HCRST = 1;
    while (usbcmd->bits.HCRST);
    while (usbsts->bits.CNR);
}

void InitializeXHCI(PCI& pci_devices, FrameBufferWriter& writer) {
    Device* xhc;
    for (int i = 0; i < pci_devices.devices.size(); i++) {
        if (pci_devices.devices[i].class_code.base_class == 0x0cu && pci_devices.devices[i].class_code.sub_class == 0x03u && pci_devices.devices[i].class_code.interface == 0x30u){
            xhc  = &pci_devices.devices[i];
            if (pci_devices.devices[i].vendor_id == 0x8086u) break;
        }
    }

    const auto [xhc_mmio_address, error] = pci_devices.ReadBar(*xhc, 0);
    HostController host_controller(xhc_mmio_address & ~(uint64_t)0xfu, writer);
}
#include "xhci.hpp"

#include <stdio.h>

HostController::HostController(uintptr_t xhc_mmio_address, FrameBufferWriter& writer): xhc_mmio_address_{xhc_mmio_address},
                                                            capability_registers_{(CapabilityRegisters*) xhc_mmio_address},
                                                            operational_registers_{(OperationalRegisters*) xhc_mmio_address + capability_registers_->CAPLENGTH} {
    char s[1024];
    sprintf(s, "%p", operational_registers_);
    writer.WriteString({10, 10}, s, 0xffffff);
    sprintf(s, "%p", xhc_mmio_address + capability_registers_->CAPLENGTH);
    writer.WriteString({10, 26}, s, 0xffffff);

    auto usbsts = operational_registers_->USBSTS;
    auto usbcmd = operational_registers_->USBCMD;
    if (!usbsts.bits.HCH) {
        usbcmd.bits.RS = 0;
    }
    usbcmd.bits.HCRST = 1;
    while (usbcmd.bits.HCRST);
    // while (usbsts.bits.CNR);
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
    char s[1024];
    sprintf(s, "%p", xhc_mmio_address & ~(uint64_t)0xfu);
    writer.WriteString({10, 100}, s, 0xffffff);

    HostController host_controller(xhc_mmio_address & ~(uint64_t)0xfu, writer);
}
#include "xhci.hpp"

HostController::HostController(uintptr_t xhc_mmio_address): xhc_mmio_address{xhc_mmio_address_} {}

void InitializeXHCI(PCI& pci_devices) {
    Device* xhc;
    for (int i = 0; i < pci_devices.devices.size(); i++) {
        Device dev = pci_devices.devices[i];
        if (dev.class_code.base_class == 0x0cu && dev.class_code.sub_class == 0x03u && dev.class_code.interface == 0x30u) xhc  = &dev;
        if (dev.vendor_id == 0x8086u) break;
    }

    const auto [xhc_mmio_address, error] = pci_devices.ReadBar(*xhc, 0);
}
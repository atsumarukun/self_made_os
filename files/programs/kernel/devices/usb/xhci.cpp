#include "xhci.hpp"

int InitializeXHCI(PCI& pci_devices) {
    int n = 0;
    for (int i = 0; i < pci_devices.devices.size(); i++) {
        Device dev = pci_devices.devices[i];
        if (dev.class_code.base_class == 0x0cu && dev.class_code.sub_class == 0x03u && dev.class_code.interface == 0x30u) n++;
    }
    return n;
}
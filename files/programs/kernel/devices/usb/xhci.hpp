#pragma once

#include "../pci.hpp"

class HostController {
    public:
        HostController(uintptr_t xhc_mmio_address);

    private:
        const uintptr_t xhc_mmio_address_;
};

void InitializeXHCI(PCI& pci_devices);
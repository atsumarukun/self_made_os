#pragma once

#include "registers.hpp"
#include "../pci.hpp"

class HostController {
    public:
        HostController(uintptr_t xhc_mmio_address);

    private:
        const uintptr_t xhc_mmio_address_;
        CapabilityRegisters* capability_registers_;
        OperationalRegisters* operational_registers_;
};

void InitializeXHCI(PCI& pci_devices);
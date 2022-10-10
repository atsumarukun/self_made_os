#pragma once

#include "registers.hpp"
#include "../pci.hpp"
#include "../../graphics/graphics.hpp"

class HostController {
    public:
        HostController(uintptr_t xhc_mmio_address, FrameBufferWriter& writer);

    private:
        const uintptr_t xhc_mmio_address_;
        CapabilityRegisters* const capability_registers_;
        OperationalRegisters* const operational_registers_;
};

void InitializeXHCI(PCI& pci_devices, FrameBufferWriter& writer);
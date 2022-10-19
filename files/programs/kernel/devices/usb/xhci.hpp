#pragma once

#include "registers.hpp"
#include "../pci.hpp"
#include "../../memory/memory_manager.hpp"

#include "../../graphics/graphics.hpp"
#include <stdio.h>

class HostController {
    public:
        HostController(uintptr_t xhc_mmio_address, MemoryManager& memory_manager, int device_num, FrameBufferWriter& writer);
        uint8_t MaxPorts() const { return hcsparams1_ >> 24; }

    private:
        const uintptr_t xhc_mmio_address_;
        CapabilityRegisters* const capability_registers_;
        OperationalRegisters* const operational_registers_;
        const uint32_t hcsparams1_;
};

uintptr_t XhcMmioBaseAddress(PCI& pci_devices);
#pragma once

#include "registers.hpp"
#include "port.hpp"
#include "../pci.hpp"
#include "../../memory/memory_manager.hpp"

class HostController {
    public:
        HostController(uintptr_t xhc_mmio_address, MemoryManager& memory_manager, int device_num);
        uint8_t MaxPorts() const { return hcsparams1_ >> 24; }
        Port PortAt(uint8_t port_num) { return Port(port_num, (PortRegisterSet*) (GetPortRegisterSet() + port_num * sizeof(PortRegisterSet))); }

    private:
        const uintptr_t xhc_mmio_address_;
        CapabilityRegisters* const capability_registers_;
        OperationalRegisters* const operational_registers_;
        const uint32_t hcsparams1_;

        uintptr_t GetPortRegisterSet() { return (uintptr_t) operational_registers_ + 0x400u; }
};

uintptr_t XhcMmioBaseAddress(PCI& pci_devices);
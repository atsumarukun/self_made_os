#pragma once

#include <stdint.h>

#include "registers.hpp"
#include "device_manager.hpp"
#include "ring.hpp"
#include "port.hpp"
#include "../pci.hpp"
#include "../../memory/memory_manager.hpp"

class HostController {
    public:
        HostController(uintptr_t mmio_base_address, uint8_t device_num, MemoryManager& memory_manager);
        uint8_t MaxPorts() const;
        Port PortAt(uint8_t port_num);
        void ConfigurePort(Port& port);
        bool ProcessEvent();

    private:
        const uintptr_t mmio_base_address_;
        CapabilityRegisters* const capability_registers_;
        OperationalRegisters* const operational_registers_;
        uint8_t max_ports_;
        DeviceManager device_manager_;
        Ring cr_;
        EventRing er_;
        uint8_t addressing_port_ = 0;

        Array<InterrupterRegisterSet> InterrupterRegisterSets() const;
        Array<PortRegisterSet> PortRegisterSets() const;
        void ResetPort(Port& port);
};

uintptr_t XhcMmioBaseAddress(PCI& pci_devices);
#pragma once

#include "registers.hpp"

enum class PortStatus {
    NotConnected,
    WaitingAddress,
    ResettingPort,
};

class Port {
    public:
        Port(uint8_t port_num, PortRegisterSet* port_register_set): port_num_{port_num}, port_register_set_{port_register_set}, status_{PortStatus::NotConnected} {}
        uint32_t IsConnected() const;
        uint8_t Number() const;
        PortStatus Status() const;
        void SetStatus(PortStatus status);
        void Reset();

    private:
        uint8_t port_num_;
        PortRegisterSet* port_register_set_;
        PortStatus status_;
};
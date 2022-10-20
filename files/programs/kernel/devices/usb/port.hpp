#pragma once

#include "registers.hpp"

class Port {
    public:
        Port(uint8_t port_num, PortRegisterSet* port_register_set): port_num_{port_num}, port_register_set_{port_register_set} {}
        uint32_t IsConnected() const;
        PortRegisterSet* Pointer() {return port_register_set_;}

    private:
    uint8_t port_num_;
        PortRegisterSet* const port_register_set_;
};
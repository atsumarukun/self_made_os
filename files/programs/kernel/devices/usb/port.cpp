#include "port.hpp"

uint32_t Port::IsConnected() const {
    return port_register_set_->PORTSC.Read().bits.CCS;
}

uint8_t Port::Number() const {
    return port_num_;
}

PortStatus Port::Status() const {
    return status_;
}

void Port::SetStatus(PortStatus status) {
    status_ = status;
}

void Port::Reset() {
    PORTSCMap portsc = port_register_set_->PORTSC.Read();
    portsc.data[0] &= 0x0e00c3e0u;
    portsc.data[0] |= 0x00020010u;
    port_register_set_->PORTSC.Write(portsc);
    while (port_register_set_->PORTSC.Read().bits.PR);
    while (!port_register_set_->PORTSC.Read().bits.PED);
}
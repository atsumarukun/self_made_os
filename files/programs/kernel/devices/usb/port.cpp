#include "port.hpp"

uint32_t Port::IsConnected() const {
    return port_register_set_->PORTSC.bits.CCS;
}
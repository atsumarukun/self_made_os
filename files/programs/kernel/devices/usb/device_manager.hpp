#pragma once

#include <stdint.h>

#include "context.hpp"

class DeviceManager {
    public:
        void Initialize(uint8_t max_slots);
        DeviceContext** DeviceContexts() const;

    private:
        DeviceContext** device_context_pointers_;
        uint8_t max_slots_;
};
#pragma once

#include <stdint.h>

#include "context.hpp"
#include "../../memory/memory_manager.hpp"

class DeviceManager {
    public:
        void Initialize(uint8_t max_slots, MemoryManager& memory_manager);
        DeviceContext** DeviceContexts() const;

    private:
        DeviceContext** device_context_pointers_;
        uint8_t max_slots_;
};
#include "device_manager.hpp"
#include "memory.hpp"

void DeviceManager::Initialize(uint8_t max_slots) {
    max_slots_ = max_slots;

    device_context_pointers_ = (DeviceContext**) Allocate(sizeof(DeviceContext*) * (max_slots_ + 1), 64, 4096);
    if (!device_context_pointers_) {
        return;
    }

    for (int i = 0; i < max_slots_; i++) {
        device_context_pointers_[i] = nullptr;
    }
}

DeviceContext** DeviceManager::DeviceContexts() const {
    return device_context_pointers_;
}
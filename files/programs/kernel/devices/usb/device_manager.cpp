#include "device_manager.hpp"

void DeviceManager::Initialize(uint8_t max_slots, MemoryManager& memory_manager) {
    max_slots_ = max_slots;

    device_context_pointers_ = (DeviceContext**) memory_manager.Allocate((sizeof(DeviceContext*) * (max_slots_ + 1) + 4095) / 44096).value;
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
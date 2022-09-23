#include "memory_manager.hpp"

MemoryManager::MemoryManager(const MemoryMap& memory_map) {
    uintptr_t free_end_address = 0;
    for (uintptr_t memory_descriptor_address = (uintptr_t) memory_map.map_buf;
            memory_descriptor_address < (uintptr_t) memory_map.map_buf + memory_map.map_size;
            memory_descriptor_address += memory_map.descriptor_size) {

        MemoryDescriptor* memory_descriptor = (MemoryDescriptor*) memory_descriptor_address;
        if (free_end_address < memory_descriptor->physical_address) {
            Allocated((free_end_address) / FRAME_SIZE, (memory_descriptor->physical_address - free_end_address + FRAME_SIZE - 1) / FRAME_SIZE);
        }
        if (IsAvailableMemoryType(memory_descriptor->type)) {
            free_end_address = memory_descriptor->physical_address + memory_descriptor->number_of_pages * UEFI_FRAME_SIZE;
        } else {
            Allocated((memory_descriptor->physical_address) / FRAME_SIZE, (memory_descriptor->physical_address + memory_descriptor->number_of_pages * UEFI_FRAME_SIZE + FRAME_SIZE - 1) / FRAME_SIZE);
        }
    }
    last_frame_index_ = free_end_address / FRAME_SIZE;
}

void MemoryManager::Allocated(unsigned int start_frame_index, size_t number_of_frames) {
    for (size_t i = 0; i < number_of_frames; i++) {
        SetAllocateBit(start_frame_index + i, true);
    }
}

void MemoryManager::SetAllocateBit(unsigned int frame_index, bool is_allocation) {
    if (is_allocation) {
        memory_bit_map_[frame_index / NUMBER_OF_MAP_LINE_BITS] |= 1 << frame_index % NUMBER_OF_MAP_LINE_BITS;
    } else {
        memory_bit_map_[frame_index / NUMBER_OF_MAP_LINE_BITS] &= ~(1 << frame_index % NUMBER_OF_MAP_LINE_BITS);
    }
}
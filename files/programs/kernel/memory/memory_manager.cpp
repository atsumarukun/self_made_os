#include "memory_manager.hpp"

MemoryManager::MemoryManager(const MemoryMap& memory_map) {
    uintptr_t free_end_address = 0;
    for (uintptr_t memory_descriptor_address = (uintptr_t) memory_map.map_buf;
            memory_descriptor_address < (uintptr_t) memory_map.map_buf + memory_map.map_size;
            memory_descriptor_address += memory_map.descriptor_size) {

        MemoryDescriptor* memory_descriptor = (MemoryDescriptor*) memory_descriptor_address;
        if (free_end_address < memory_descriptor->physical_address) {
            SetIsAllocationBits((free_end_address) / FRAME_SIZE, (memory_descriptor->physical_address - free_end_address + FRAME_SIZE - 1) / FRAME_SIZE, true);
        }
        if (IsAvailableMemoryType(memory_descriptor->type)) {
            free_end_address = memory_descriptor->physical_address + memory_descriptor->number_of_pages * UEFI_FRAME_SIZE;
        } else {
            SetIsAllocationBits((memory_descriptor->physical_address) / FRAME_SIZE, (memory_descriptor->physical_address + memory_descriptor->number_of_pages * UEFI_FRAME_SIZE + FRAME_SIZE - 1) / FRAME_SIZE, true);
        }
    }
    last_frame_index_ = free_end_address / FRAME_SIZE;
}

WithError<size_t, size_t> MemoryManager::Allocate(size_t number_of_frames) {
    size_t start_frame_index = 0;
    for (size_t i = 0; i < last_frame_index_; i++) {
        if (memory_bit_map_[i / NUMBER_OF_MAP_LINE_BITS] >> (i % NUMBER_OF_MAP_LINE_BITS) & 1) {
            start_frame_index = i + 1;
        }
        if ((i + 1) - start_frame_index == number_of_frames) {
            SetIsAllocationBits(start_frame_index, number_of_frames, true);
            return {start_frame_index, i, MakeError(Error::Success)};
        }
    }
    return {0, 0, MakeError(Error::MemoryFrameShortage)};
}

void MemoryManager::SetIsAllocationBits(unsigned int start_frame_index, size_t number_of_frames, bool is_allocation) {
    for (size_t i = 0; i < number_of_frames; i++) {
        if (is_allocation) {
            memory_bit_map_[(start_frame_index + i) / NUMBER_OF_MAP_LINE_BITS] |= 1 << ((start_frame_index + i) % NUMBER_OF_MAP_LINE_BITS);
        } else {
            memory_bit_map_[(start_frame_index + i) / NUMBER_OF_MAP_LINE_BITS] &= ~(1 << (start_frame_index + i) % NUMBER_OF_MAP_LINE_BITS);
        }
    }
}
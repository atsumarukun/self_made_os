#pragma once

#include <array>

#include "memory_map.hpp"

using namespace std;

namespace {
    constexpr unsigned long long operator""_KB(unsigned long long kb) {
        return kb * 1024;
    }
    constexpr unsigned long long operator""_MB(unsigned long long mb) {
        return mb * 1024_KB;
    }
    constexpr unsigned long long operator""_GB(unsigned long long gb) {
        return gb * 1024_MB;
    }

    static const unsigned int UEFI_FRAME_SIZE = 4_KB;
    static const unsigned int FRAME_SIZE = 4_KB;
    static const unsigned int NUMBER_OF_FRAMES_SUPPORTED = 128_GB / FRAME_SIZE;
    static const unsigned int NUMBER_OF_MAP_LINE_BITS = 8 * sizeof(unsigned long);
}

class MemoryManager {
    public:
        MemoryManager(const MemoryMap& memory_map);
        void Allocated(unsigned int start_frame_index, size_t number_of_frames);
        void SetAllocateBit(unsigned int frame_index, bool is_allocation);

    private:
        array<unsigned long, NUMBER_OF_FRAMES_SUPPORTED / NUMBER_OF_MAP_LINE_BITS> memory_bit_map_;
        int last_frame_index_ = NUMBER_OF_FRAMES_SUPPORTED;
};
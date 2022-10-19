#pragma once

#include "trb.hpp"
#include "registers.hpp"
#include "../../memory/memory_manager.hpp"

union EventRingSegmentTableEntry {
    uint32_t data[4];

    struct {
        uint64_t ring_segment_base_address;

        uint32_t ring_segment_size: 16;
        uint32_t : 16;

        uint32_t : 32;
    } __attribute__((packed)) bits;
};

class RingManager {
    public:
        RingManager(int buffer_size, MemoryManager& memory_manager);
        TRB* Buffer() {return buffer_;}

    private:
        TRB* buffer_ = nullptr;
        int buffer_size_;
        bool cycle_bit_;
        int write_index_;
};

class EventRingManager {
    public:
        EventRingManager(int buffer_size, InterrupterRegisterSet* interrupter, MemoryManager& memory_manager);

    private:
        TRB* buffer_ = nullptr;
        int buffer_size_;
        bool cycle_bit_;
        EventRingSegmentTableEntry* erst_;
        InterrupterRegisterSet* interrupter_;
};
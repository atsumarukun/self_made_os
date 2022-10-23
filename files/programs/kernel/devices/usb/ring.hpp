#pragma once

#include <stdint.h>

#include "trb.hpp"
#include "registers.hpp"

union EventRingSegmentTableEntry {
    uint32_t data[4];

    struct {
        uint64_t ring_segment_base_address;

        uint32_t ring_segment_size: 16;
        uint32_t : 16;

        uint32_t : 32;
    } __attribute__((packed)) bits;
};

class Ring {
    public:
        void Initialize(int buffer_size);
        TRB* Buffer() const;

    private:
        TRB* buffer_;
        int buffer_size_;
        bool cycle_bit_;
        int write_index_;
};

class EventRing {
    public:
        void Initialize(int buffer_size, InterrupterRegisterSet* interrupter);
        uint64_t HasEvent();

    private:
        TRB* buffer_;
        int buffer_size_;
        bool cycle_bit_;
        EventRingSegmentTableEntry* erst_;
        InterrupterRegisterSet* interrupter_;
};
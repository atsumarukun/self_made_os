#include <string.h>

#include "ring.hpp"
#include "memory.hpp"

void Ring::Initialize(int buffer_size) {
    buffer_size_ = buffer_size;
    cycle_bit_ = true;
    write_index_ = 0;

    buffer_ = (TRB*) Allocate(sizeof(TRB) * buffer_size_, 64, 64 * 1024);
    if (!buffer_) return;
    memset(buffer_, 0, sizeof(TRB) * buffer_size_);
}

TRB* Ring::Buffer() const {
    return buffer_;
}

void EventRing::Initialize(int buffer_size, InterrupterRegisterSet* interrupter) {
    buffer_size_ = buffer_size;
    cycle_bit_ = true;
    interrupter_ = interrupter;

    buffer_ = (TRB*) Allocate(sizeof(TRB) * buffer_size_, 64, 64 * 1024);
    if (!buffer_) return;
    memset(buffer_, 0, sizeof(TRB) * buffer_size_);

    erst_ = (EventRingSegmentTableEntry*) Allocate(sizeof(EventRingSegmentTableEntry) * 1, 64, 64 * 1024);
    if (!erst_) return;
    memset(erst_, 0, sizeof(EventRingSegmentTableEntry) * 1);

    erst_[0].bits.ring_segment_base_address = (uint64_t) buffer_;
    erst_[0].bits.ring_segment_size = buffer_size_;

    interrupter_->ERSTSZ = 1;
    ERDPMap erdp = interrupter_->ERDP.Read();
    erdp.bits.ERDP = (uint64_t) (&buffer_[0]) >> 4;
    interrupter_->ERDP.Write(erdp);
    ERSTBAMap erstba = interrupter_->ERSTBA.Read();
    erstba.bits.ERSTBA = (uint64_t) erst_ >> 6;
    interrupter_->ERSTBA.Write(erstba);
}

uint64_t EventRing::HasEvent() {
    TRB* trb = (TRB*) (interrupter_->ERDP.Read().bits.ERDP << 4);
    return (uint64_t) trb->bits.cycle_bit;
}
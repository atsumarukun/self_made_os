#include "ring.hpp"

RingManager::RingManager(int buffer_size, MemoryManager& memory_manager): buffer_size_{buffer_size} {
    buffer_ = (TRB*) memory_manager.Allocate((sizeof(TRB) * buffer_size_ + 4095) / 4096).value;
    memset(buffer_, 0, sizeof(TRB) * buffer_size_);
    cycle_bit_ = true;
    write_index_ = 0;
}

EventRingManager::EventRingManager(int buffer_size, InterrupterRegisterSet* interrupter, MemoryManager& memory_manager): buffer_size_{buffer_size}, interrupter_{interrupter} {
    buffer_ = (TRB*) memory_manager.Allocate((sizeof(TRB) * buffer_size_ + 4095) / 4096).value;
    memset(buffer_, 0, sizeof(TRB) * buffer_size_);
    cycle_bit_ = true;

    erst_->bits.ring_segment_base_address = (uint64_t) buffer_;
    erst_->bits.ring_segment_size = buffer_size_;

    interrupter_->ERSTSZ = 1;
    interrupter_->ERSTBA = (uint64_t) erst_;
}
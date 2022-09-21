#include <array>

#include "segment.hpp"
#include "memory.h"

using namespace std;
uintptr_t gdtr;

namespace {
    array<SegmentDescriptor, 2> gdt;

    void SetCodeSegment(SegmentDescriptor& descriptor, enum SegmentDescriptorType type, uint8_t descriptor_privilege_level) {
        descriptor.data = 0;

        descriptor.bits.type = type;
        descriptor.bits.system_segment = 1;
        descriptor.bits.descriptor_privilege_level = descriptor_privilege_level;
        descriptor.bits.present = 1;
        descriptor.bits.long_mode = 1;
        descriptor.bits.default_operation_size = 0;
    }
}

void SetGlobalDescriptorTable() {
    gdt[0].data = 0;
    SetCodeSegment(gdt[1], ExecuteReadable, 0);
    gdtr |= sizeof(gdt) - 1;
    gdtr |= (uintptr_t) &gdt[0] << 16;
    __asm__ ("lgdt gdtr");
}
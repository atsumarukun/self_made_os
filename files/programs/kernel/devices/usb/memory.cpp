#include <stdint.h>

#include "memory.hpp"

namespace {
    alignas(64) uint8_t segment[4096 * 32];
    uintptr_t pointer = (uintptr_t) &segment;
}

void* Allocate(int size, unsigned int alignment, unsigned int boundary) {
    if (alignment) {
        pointer = (pointer + alignment - 1) & ~(uintptr_t) (alignment - 1);
    }

    if (boundary) {
        uintptr_t next_boundary = (pointer + boundary - 1) & ~(uintptr_t) (boundary - 1);
        if (next_boundary < pointer + size) {
            pointer = next_boundary;
        }
    }

    if ((unsigned long) (&segment + sizeof(segment)) < pointer + size) {
        return nullptr;
    }

    uintptr_t p = pointer;
    pointer += size;
    return (void*) p;
}
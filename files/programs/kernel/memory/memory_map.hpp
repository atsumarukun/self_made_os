#pragma once

#include <stdint.h>

struct MemoryMap {
    unsigned long long map_buf_size;
    void* map_buf;
    unsigned long long map_size;
    unsigned long long map_key;
    unsigned long long descriptor_size;
    uint32_t descriptor_version;
};
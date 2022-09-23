#pragma once

#include <stdint.h>

extern "C" {
    void SetCS(uint16_t cs);
    void SetCR3(uint64_t pml4_table_address);
}
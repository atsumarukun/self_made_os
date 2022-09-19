#pragma once

#include <stdint.h>

typedef struct {
    unsigned long long map_buf_size;
    void* map_buf;
    unsigned long long map_size;
    unsigned long long map_key;
    unsigned long long descriptor_size;
    uint32_t descriptor_version;
} MemoryMap;

#ifdef __cplusplus

enum class MemoryType: uint32_t {
    EfiReservedMemoryType,
    EfiLoaderCode,
    EfiLoaderData,
    EfiBootServicesCode,
    EfiBootServicesData,
    EfiRuntimeServiceCode,
    EfiRuntimeServicesData,
    EfiConventionalMemory,
    EfiUnusableMemory,
    EfiACPIReclaimMemory,
    EfiACPIMemoryNVS,
    EfiMemoryMappedIO,
    EfiMemoryMappedIOPortSpace,
    EfiPalCode,
    EfiPersistentMemory,
};

typedef struct {
    enum MemoryType memory_type;
    unsigned int* physical_address;
    unsigned int* virtual_address;
    uint64_t number_of_pages;
    uint64_t attribute;
} MemoryDescriptor;

inline bool IsAvailableMemoryType(MemoryType memory_type) {
    return memory_type == MemoryType::EfiLoaderCode       ||
           memory_type == MemoryType::EfiLoaderData       ||
           memory_type == MemoryType::EfiBootServicesCode ||
           memory_type == MemoryType::EfiBootServicesData ||
           memory_type == MemoryType::EfiConventionalMemory;
}

#endif
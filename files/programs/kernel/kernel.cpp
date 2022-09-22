/*
@file kernel.cpp

カーネルのメインファイル.
*/

#include <stdio.h>

#include "graphics/frame_buffer.hpp"
#include "graphics/graphics.hpp"
#include "graphics/text.hpp"
#include "memory/memory_map.hpp"
#include "memory/segment.hpp"
#include "register/register.h"

alignas(16) uint8_t kernel_main_stack[1024 * 1024];

extern "C" void KernelMain(const FrameBuffer& frame_buffer, const MemoryMap& memory_map) {
    FrameBufferWriter frame_buffer_writer(frame_buffer);

    SetGlobalDescriptorTable();
    const uint16_t kernel_cs = 8;
    SetCS(kernel_cs);

    for (int x = 0; x < frame_buffer.width; x++) {
        for (int y = 0; y < frame_buffer.height; y++) {
            frame_buffer_writer.WritePixel({x, y}, 0x000000);
        }
    }

    char s[1024];
    sprintf(s, "%lu", sizeof(GDTR));
    WriteString(frame_buffer_writer, {10, 10}, s);

    int y = 0;
    for (unsigned int* i = (unsigned int*) memory_map.map_buf; i < (unsigned int*) memory_map.map_buf + memory_map.map_size; i += memory_map.descriptor_size) {
        MemoryDescriptor* memory_descriptor = (MemoryDescriptor*) i;
        if (IsAvailableMemoryType(memory_descriptor->type)) {
            sprintf(s, "%d", memory_descriptor->type);
            WriteString(frame_buffer_writer, {10, 116 + 16 * y++}, s);
        }
    }

    while (1) __asm__("hlt");
}
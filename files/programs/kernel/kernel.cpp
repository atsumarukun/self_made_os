/*
@file kernel.cpp

カーネルのメインファイル.
*/

#include <stdio.h>

#include "graphics/frame_buffer.hpp"
#include "graphics/graphics.hpp"
#include "graphics/text.hpp"
#include "memory/memory_map.hpp"
#include "memory/memory_manager.hpp"
#include "memory/segment.hpp"
#include "memory/paging.hpp"
#include "register/register.h"

char memory_manager_buffer[sizeof(MemoryManager)];
MemoryManager* memory_manager;

alignas(16) uint8_t kernel_main_stack[1024 * 1024];

extern "C" void KernelMain(const FrameBuffer& frame_buffer_tmp, const MemoryMap& memory_map_tmp) {
    FrameBuffer frame_buffer = frame_buffer_tmp;
    MemoryMap memory_map = memory_map_tmp;
    FrameBufferWriter frame_buffer_writer(frame_buffer);

    SetGlobalDescriptorTable();
    const uint16_t kernel_cs = 8;
    SetCS(kernel_cs);
    MakePageTable();

    memory_manager = new(memory_manager_buffer) MemoryManager(memory_map);

    for (int x = 0; x < frame_buffer.width; x++) {
        for (int y = 0; y < frame_buffer.height; y++) {
            frame_buffer_writer.WritePixel({x, y}, 0x000000);
        }
    }

    while (1) __asm__("hlt");
}
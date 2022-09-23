/*
@file kernel.cpp

カーネルのメインファイル.
*/

#include <stdio.h>
#include "graphics/text.hpp"

#include "graphics/frame_buffer.hpp"
#include "graphics/graphics.hpp"
#include "memory/memory_map.hpp"
#include "memory/memory_manager.hpp"
#include "memory/segment.hpp"
#include "memory/paging.hpp"
#include "register/register.h"

#include "error/error.hpp"

alignas(16) uint8_t kernel_main_stack[1024 * 1024];

char memory_manager_buffer[sizeof(MemoryManager)];
MemoryManager* memory_manager;

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

    for (int i = 0; i < 2; i++) {
        auto [s, err] = memory_manager->Allocate(5);
        if (err) {
            WriteString(frame_buffer_writer, {10, 10 + 16 * i}, err.Message());
        } else {
            char st[1024];
            sprintf(st, "%lu", s);
            WriteString(frame_buffer_writer, {10, 10 + 16 * i}, st);
        }
    }

    while (1) __asm__("hlt");
}
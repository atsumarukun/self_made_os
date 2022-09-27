/*
@file kernel.cpp

カーネルのメインファイル.
*/

#include "graphics/frame_buffer.hpp"
#include "graphics/graphics.hpp"
#include "memory/memory_map.hpp"
#include "memory/memory_manager.hpp"
#include "memory/segment.hpp"
#include "memory/paging.hpp"
#include "register/register.h"
#include "devices/pci.hpp"

int printk(const char* format, ...) {
    va_list ap;
    int result;
    char s[1024];

    va_start(ap, format);
    result = vsprintf(s, format, ap);
    va_end(ap);

    return result;
}

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
    InitializeHeap(*memory_manager);

    frame_buffer_writer.DrawRectangle({0, 0}, {frame_buffer.width, frame_buffer.height}, 0x000000);
    frame_buffer_writer.DrawMouseCursor({300, 200});
    frame_buffer_writer.WriteString({10, 10}, "Hello World!", 0xffffff);

    PCI pci_devices;
    char s[1024];
    sprintf(s, "%lu", pci_devices.GetDevices());
    frame_buffer_writer.WriteString({10, 100}, s, 0xffffff);

    while (1) __asm__("hlt");
}
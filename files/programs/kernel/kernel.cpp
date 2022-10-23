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
#include "memory/heap.hpp"
#include "register/register.h"
#include "devices/pci.hpp"
#include "devices/usb/xhci.hpp"

#include <stdio.h>

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

    PCI pci_devices;
    HostController xhc(XhcMmioBaseAddress(pci_devices), pci_devices.devices.size(), *memory_manager);

    for (int i = 0; i < xhc.MaxPorts(); i++) {
        Port port = xhc.PortAt(i);
        if (port.IsConnected()) {
            xhc.ConfigurePort(port);         
        }
    }

    frame_buffer_writer.DrawRectangle({0, 0}, {frame_buffer.width, frame_buffer.height}, 0x000000);
    frame_buffer_writer.DrawMouseCursor({300, 200});
    frame_buffer_writer.WriteString({10, 10}, "Hello World!", 0xffffff);

    char s[1024];
    sprintf(s, "%d", xhc.ProcessEvent());
    frame_buffer_writer.WriteString({10, 26}, s, 0xffffff);
    // int i = 0;
    // while (1) {
    //     if (xhc.ProcessEvent()) {
    //         sprintf(s, "%d", xhc.ProcessEvent());
    //         frame_buffer_writer.WriteString({10, 26 + 16 * i++}, s, 0xffffff);
    //     }
    // }

    while (1) __asm__("hlt");
}
/*
@file kernel.cpp

カーネルのメインファイル.
*/

#include "graphics/frame_buffer.hpp"
#include "graphics/graphics.hpp"


extern "C" void KernelMain(const FrameBuffer& frame_buffer) {
    FrameBufferWriter writer(frame_buffer);

    for (int x = 0; x < frame_buffer.width; x++) {
        for (int y = 0; y < frame_buffer.height; y++) {
            writer.WritePixel(x, y, 0x0000ff);
        }
    }

    while (1) __asm__("hlt");
}
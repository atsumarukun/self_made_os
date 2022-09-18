/*
@file kernel.cpp

カーネルのメインファイル.
*/

#include <stdio.h>

#include "graphics/frame_buffer.hpp"
#include "graphics/graphics.hpp"
#include "graphics/text.hpp"
#include "error/error.hpp"

extern "C" void KernelMain(const FrameBuffer& frame_buffer) {
    FrameBufferWriter frame_buffer_writer(frame_buffer);

    for (int x = 0; x < frame_buffer.width; x++) {
        for (int y = 0; y < frame_buffer.height; y++) {
            frame_buffer_writer.WritePixel({x, y}, 0x000000);
        }
    }

    Error error = MakeError(Error::success);
    WriteString(frame_buffer_writer, {10, 26}, error.Message());

    while (1) __asm__("hlt");
}
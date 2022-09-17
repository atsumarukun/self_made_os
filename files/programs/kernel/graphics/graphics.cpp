#include "graphics.hpp"

namespace {
    int GetPixelBytes(enum PixelFormat pixel_format) {
        switch (pixel_format) {
            case RGBResv8bit:
                return 4;
            case BGRResv8bit:
                return 4;
        }
        return -1;
    }
}

void FrameBufferWriter::WritePixel(int x, int y, uint8_t color) {
    uint8_t* pixel = &frame_buffer_.base_address[GetPixelBytes(frame_buffer_.pixel_format) * (frame_buffer_.line * y + x)];
    pixel[0] = color >> 16;
    pixel[1] = (color >> 8) & 0xff;
    pixel[2] = color & 0xff;
}
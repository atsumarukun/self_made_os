#pragma once

#include "frame_buffer.hpp"

struct Coordinate{
    unsigned int x, y;
};

struct Size {
    unsigned int width, height;
};

class FrameBufferWriter {
    public:
        FrameBufferWriter(const FrameBuffer& frame_buffer): frame_buffer_{frame_buffer} {}
        void DrawRectangle(Coordinate coordinate, Size size, unsigned int color);
        void DrawCircle(Coordinate coordinate, Size size, unsigned int color);
        void WriteString(Coordinate coordinate, const char* string, unsigned int color);
        void DrawMouseCursor(Coordinate coordinate);

    private:
        const FrameBuffer& frame_buffer_;
        void WritePixel(Coordinate Coordinate, unsigned int color);
        void WriteOneLetter(Coordinate Coordinate, char character, unsigned int color);
};
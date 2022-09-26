#pragma once

#include "frame_buffer.hpp"

typedef struct {
    unsigned int x, y;
} Coordinate;

class FrameBufferWriter {
    public:
        FrameBufferWriter(const FrameBuffer& frame_buffer): frame_buffer_{frame_buffer} {}
        void CreateRectangle(Coordinate start_coordinate, Coordinate end_coordinate, unsigned int color);
        void WriteString(Coordinate coordinate, const char* string, unsigned int color);

    private:
        const FrameBuffer& frame_buffer_;
        void WritePixel(Coordinate Coordinate, unsigned int color);
        void WriteOneLetter(Coordinate Coordinate, char character, unsigned int color);
};
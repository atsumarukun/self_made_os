#pragma once

#include "frame_buffer.hpp"

typedef struct {
    int x, y;
} Coordinate;

class FrameBufferWriter {
    public:
        FrameBufferWriter(const FrameBuffer& frame_buffer): frame_buffer_{frame_buffer} {}
        void WritePixel(Coordinate Coordinate, unsigned int color);

    private:
        const FrameBuffer& frame_buffer_;
};
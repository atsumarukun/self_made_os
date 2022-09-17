#pragma once

#include "frame_buffer.hpp"

class FrameBufferWriter {
    public:
        FrameBufferWriter(const FrameBuffer& frame_buffer): frame_buffer_{frame_buffer} {}
        void WritePixel(int x, int y, unsigned int color);

    private:
        const FrameBuffer& frame_buffer_;
};
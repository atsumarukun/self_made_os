#include "text.hpp"

namespace {
    const char font_a[16][8 + 1] = {
        "        ",
        "   @    ",
        "   @    ",
        "  @ @   ",
        "  @ @   ",
        "  @ @   ",
        " @   @  ",
        " @   @  ",
        " @   @  ",
        " @@@@@  ",
        "@     @ ",
        "@     @ ",
        "@     @ ",
        "@     @ ",
        "        ",
        "        "
    };
}

void WriteOneLetter(FrameBufferWriter frame_buffer_writer, int x, int y) {
    for (int dy = 0; dy < 16; dy++) {
        for (int dx = 0; dx < 8; dx++) {
            if (font_a[dy][dx] == '@') frame_buffer_writer.WritePixel(x + dx, y + dy, 0xffffff);
        }
    }
}
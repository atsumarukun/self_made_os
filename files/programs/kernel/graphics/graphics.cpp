/*
@file graphics.cpp

グラフィックの設定ファイル.
*/

#include <math.h>

#include "graphics.hpp"

extern const uint8_t _binary_graphics_font_bin_start;
extern const uint8_t _binary_graphics_font_bin_size;

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

    const uint8_t* GetFont(char character) {
        auto index = 16 * (unsigned int) character;
        if (index >= (uintptr_t) &_binary_graphics_font_bin_size) return nullptr;
        return &_binary_graphics_font_bin_start + index;
    }

    const int mouse_cursor_height = 20;
    const int mouse_cursor_width = 12;
    const char mouse_cursor_form[mouse_cursor_height][mouse_cursor_width + 1] = {
        "@           ",
        "@@          ",
        "@.@         ",
        "@..@        ",
        "@...@       ",
        "@....@      ",
        "@.....@     ",
        "@......@    ",
        "@.......@   ",
        "@........@  ",
        "@.........@ ",
        "@..........@",
        "@.........@ ",
        "@........@  ",
        "@.......@   ",
        "@@@@@...@   ",
        "     @..@   ",
        "      @.@   ",
        "       @@   ",
        "        @   ",
    };
}

void FrameBufferWriter::DrawRectangle(Coordinate coordinate, Size size, unsigned int color) {
    for (int y = 0; y < size.height; y++) {
        for (int x = 0; x < size.width; x++) {
            WritePixel({coordinate.x + x, coordinate.y + y}, color);
        }
    }
}

void FrameBufferWriter::DrawCircle(Coordinate coordinate, Size size, unsigned int color) {
    for (int y = 0; y < size.height / 2; y++) {
        int max_x  = (int) sqrt((1 - pow(y, 2)/pow(size.height / 2, 2)) * pow(size.width / 2, 2));
        for (int x = 0; x < max_x; x++) {
            WritePixel({coordinate.x + x, coordinate.y + y}, color);
            WritePixel({coordinate.x - x, coordinate.y + y}, color);
            WritePixel({coordinate.x + x, coordinate.y - y}, color);
            WritePixel({coordinate.x - x, coordinate.y - y}, color);
        }
    }
}

void FrameBufferWriter::WriteString(Coordinate Coordinate, const char* string, unsigned int color) {
    for (int i = 0; string[i]; i++) {
        WriteOneLetter({Coordinate.x + 8 * i, Coordinate.y}, string[i], color);
    }
}

void FrameBufferWriter::DrawMouseCursor(Coordinate coordinate) {
    for (int y = 0; y < mouse_cursor_height; y++) {
        for (int x = 0; x < mouse_cursor_width; x++) {
            if (mouse_cursor_form[y][x] == '@') {
                WritePixel({coordinate.x + x, coordinate.y + y}, 0xdddddd);
            } else if (mouse_cursor_form[y][x] == '.') {
                WritePixel({coordinate.x + x, coordinate.y + y}, 0x222222);
            }
        }
    }
}

void FrameBufferWriter::WritePixel(Coordinate coordinate, unsigned int color) {
    uint8_t* pixel = &frame_buffer_.base_address[GetPixelBytes(frame_buffer_.pixel_format) * (frame_buffer_.line * coordinate.y + coordinate.x)];
    pixel[0] = color >> 16;
    pixel[1] = (color >> 8) & 0xff;
    pixel[2] = color & 0xff;
}

void FrameBufferWriter::WriteOneLetter(Coordinate Coordinate, char character, unsigned int color) {
    const uint8_t* font = GetFont(character);
    if (!font) return;
    for (int dy = 0; dy < 16; dy++) {
        for (int dx = 0; dx < 8; dx++) {
            if ((font[dy] << dx) & 0x80u) WritePixel({Coordinate.x + dx, Coordinate.y + dy}, color);
        }
    }
}
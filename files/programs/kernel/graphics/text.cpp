#include "text.hpp"

extern const uint8_t _binary_graphics_font_bin_start;
extern const uint8_t _binary_graphics_font_bin_size;

namespace {
    const uint8_t* GetFont(char character) {
        auto index = 16 * (unsigned int) character;
        if (index >= (uintptr_t) &_binary_graphics_font_bin_size) return nullptr;
        return &_binary_graphics_font_bin_start + index;
    }

    void WriteOneLetter(FrameBufferWriter frame_buffer_writer, int x, int y, char character) {
        const uint8_t* font = GetFont(character);
        if (!font) return;
        for (int dy = 0; dy < 16; dy++) {
            for (int dx = 0; dx < 8; dx++) {
                if ((font[dy] << dx) & 0x80u) frame_buffer_writer.WritePixel(x + dx, y + dy, 0xffffff);
            }
        }
    }
}

void WriteString(FrameBufferWriter frame_buffer_writer, const char* string) {
    unsigned int x = 0;
    while (*string) {
        WriteOneLetter(frame_buffer_writer, 10 + 8 * (x++), 10, *string);
        string++;
    }
}
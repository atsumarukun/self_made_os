/*
@file kernel.cpp

kernelのmainファイル.
*/

#include "graphics/frame_buffer.hpp"

extern "C" void Main(FrameBuffer frame_buffer, unsigned int size) {
  for (int i = 0; i < size; i++) {
    frame_buffer.base_address[i] = 255;
  }

  while (1) __asm__("hlt");
}
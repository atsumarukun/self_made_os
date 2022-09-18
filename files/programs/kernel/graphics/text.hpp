#pragma once

#include "graphics.hpp"

void WriteString(FrameBufferWriter& frame_buffer_writer, Coordinate Coordinate, const char* string);

#define PrintFileName(frame_buffer_writer) WriteString(frame_buffer_writer, {10, 10}, __FILE__)
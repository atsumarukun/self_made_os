/*
@file kernel.cpp

kernelのmainファイル.
*/

extern "C" void Main() {
  while (1) __asm__("hlt");
}
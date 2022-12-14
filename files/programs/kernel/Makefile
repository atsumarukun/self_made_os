TARGET = kernel.elf
OBJS = kernel.o newlib.o libcxx.o \
	   graphics/graphics.o graphics/font.o \
	   memory/stack.o memory/segment.o memory/memory_manager.o memory/paging.o memory/heap.o \
	   register/register.o \
	   devices/pci.o devices/usb/xhci.o devices/usb/ring.o devices/usb/port.o

CPPFLAGS += -I. -I/files/x86_64-elf/include/c++/v1 -I/files/x86_64-elf/include -nostdlibinc -D__ELF__ -D_LDBL_EQ_DBL -D_GNU_SOURCE -D_POSIX_TIMERS
CXXFLAGS += -O2 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone -fno-exceptions -fno-rtti -std=c++17
CFLAGS   += -O2 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone
LDFLAGS  += --entry CallKernelMain -z norelro --image-base 0x100000 --static -z separate-code -L/files/x86_64-elf/lib

.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	rm -rf *.o */*.o */*.bin */*/*.o

kernel.elf: $(OBJS) Makefile
	ld.lld $(LDFLAGS) -o kernel.elf $(OBJS) -lc -lc++ -lc++abi -lm

%.o: %.cpp Makefile
	clang++ $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

%.o: %.c Makefile
	clang $(CPPFLAGS) $(CFLAGS) -c $<

%.o: %.asm Makefile
	nasm -f elf64 -o $@ $<

graphics/font.bin: graphics/font.txt
	../utils/make_font.py $@ $<

graphics/font.o: graphics/font.bin
	objcopy -I binary -O elf64-x86-64 -B i386:x86-64 $< $@
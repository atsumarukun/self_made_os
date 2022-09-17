TARGET = kernel.elf
OBJS = kernel.o graphics/graphics.o graphics/text.o graphics/font.o

CPPFLAGS += -I.
CXXFLAGS += -O2 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone -fno-exceptions -fno-rtti -std=c++17
LDFLAGS  += --entry KernelMain -z norelro --image-base 0x100000 --static -z separate-code

.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	rm -rf *.o */*.o */*.bin

kernel.elf: $(OBJS) Makefile
	ld.lld $(LDFLAGS) -o kernel.elf $(OBJS)

%.o: %.cpp Makefile
	clang++ $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

graphics/font.bin: graphics/font.txt
	../utils/make_font.py $@ $<

graphics/font.o: graphics/font.bin
	objcopy -I binary -O elf64-x86-64 -B i386:x86-64 $< $@
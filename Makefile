CC = gcc
LD = ld
ASM = nasm

CFLAGS = -ffreestanding -nostdlib -m32 -Iinclude -Wall -Wextra -O2
LDFLAGS = -m elf_i386 -T embed/linker.ld
ASMFLAGS = -f elf32 

BUILD=build
SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c, $(BUILD)/kernel/%.o, $(SRCS))

.PHONY: all clean iso

all: iso 

# Compiling
$(BUILD)/kernel/%.o: src/%.c 
	@mkdir -p $(BUILD)/kernel
	$(CC) $(CFLAGS) -c $< -o $@

# Assembly
$(BUILD)/boot.o: embed/boot.asm
	@mkdir -p $(BUILD)
	$(ASM) $(ASMFLAGS) $< -o $@

# Linking 
$(BUILD)/crei-kernel: $(BUILD)/boot.o $(OBJS)
	$(LD) $(LDFLAGS) $(BUILD)/boot.o $(OBJS) -o $@

# Building
iso: $(BUILD)/crei-kernel
	@mkdir -p $(BUILD)/iso_root/boot/grub
	cp $(BUILD)/crei-kernel build/iso_root/boot/
	cp embed/grub.cfg $(BUILD)/iso_root/boot/grub/grub.cfg 
	cd build && grub-mkrescue -o crei.iso iso_root

clean:
	rm -r $(BUILD)

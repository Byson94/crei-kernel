CC = gcc
FLAGS = -ffreestanding -nostdlib -m32 -Iinclude -r

target: src/* include/*
	# Compiling.
	mkdir -p build
	$(CC) $(FLAGS) src/* -o build/crei.o
	# Linking
	nasm -f elf32 embed/boot.asm -o build/boot.o
	ld -m elf_i386 -T embed/linker.ld build/boot.o -o build/crei-kernel build/crei.o
	# Building ISO
	mkdir -p build/iso_root/boot/grub
	cp build/crei-kernel build/iso_root/boot/
	cp embed/grub.cfg build/iso_root/boot/grub/grub.cfg 
	cd build && grub-mkrescue -o crei.iso iso_root

clean:
	rm -r build

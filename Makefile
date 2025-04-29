build:
	rm tmp -r -f
	mkdir tmp

	# BOOT asm
	nasm -felf32 src/boot.s -o tmp/boot_s.o

	# GDT
	$$HOME/opt/cross/bin/i686-elf-gcc -g -c src/GDT/GDT.c -o tmp/GDT_c.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions
	nasm -felf32 src/GDT/GDT.s -o tmp/GDT_s.o

	# IDT
	$$HOME/opt/cross/bin/i686-elf-gcc -g -c src/IDT/IDT.c -o tmp/IDT_c.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions
	nasm -felf32 src/IDT/IDT.s -o tmp/IDT_s.o

	$$HOME/opt/cross/bin/i686-elf-gcc -g -c src/IDT/int_errh.c -o tmp/int_errh_c.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -Wno-unused-parameter -Wno-unused-function

	# Link
	rm bin -r -f
	mkdir bin
	
	$$HOME/opt/cross/bin/i686-elf-gcc -g -T src/LINKER.ld -ffreestanding -O2 -nostdlib -lgcc
	grub-file --is-x86-multiboot bin/TOS3.bin
	-@echo $$?

	# Debug info
	objcopy --only-keep-debug bin/TOS3.bin bin/TOS3.sym
	objcopy --strip-debug bin/TOS3.bin

	# Create iso
	mkdir -p tmp/iso/boot/grub
	cp bin/TOS3.bin tmp/iso/boot/TOS3.bin
	cp src/grub.cfg tmp/iso/boot/grub/grub.cfg
	grub-mkrescue -o bin/TOS3.iso tmp/iso

run: build 
	qemu-system-i386 -cdrom bin/TOS3.iso -monitor stdio

dbg: build
	xterm -e "gdb -ex 'target remote localhost:1234' -ex 'symbol-file bin/TOS3.sym'" &
	qemu-system-i386 -s -S -cdrom bin/TOS3.iso
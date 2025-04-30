TOS3build:
	bash build

run: TOS3build 
	qemu-system-i386 -cdrom bin/TOS3.iso -monitor stdio -serial stdio

dbg: TOS3build
	xterm -e "gdb -ex 'target remote localhost:1234' -ex 'symbol-file bin/TOS3.sym'" &
	qemu-system-i386 -s -S -cdrom bin/TOS3.iso -serial stdio
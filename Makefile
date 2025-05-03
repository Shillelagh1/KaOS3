TOS3build:
	bash build

run: TOS3build 
	qemu-system-i386 -cdrom bin/KaOS.iso -monitor stdio -serial pty

dbg: TOS3build
	xterm -e "gdb -ex 'target remote localhost:1234' -ex 'symbol-file bin/KaOS.sym'" &
	qemu-system-i386 -s -S -cdrom bin/KaOS.iso -serial stdio
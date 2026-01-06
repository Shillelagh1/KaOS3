TOS3build:
	bash build

run: 
	qemu-system-i386 -cdrom bin/KaOS.iso -monitor stdio -serial pty

dbg:
	xterm -e "gdb -ex 'target remote localhost:1234' -ex 'symbol-file bin/KaOS.sym'" &
	qemu-system-i386 -s -S -cdrom bin/KaOS.iso -serial stdio

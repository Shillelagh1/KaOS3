TOS3build:
	bash build

onlyrun:
	qemu-system-i386 -cdrom bin/KaOS.iso -monitor stdio -serial pty

onlydbg:
	xterm -e "gdb -ex 'target remote localhost:1234' -ex 'symbol-file bin/KaOS.sym'" &
	qemu-system-i386 -s -S -cdrom bin/KaOS.iso -serial stdio

run: TOS3build onlyrun
	echo "OKAY"

dbg: TOS3build onlydbg
	echo "OKAY"
	

fdbg: TOS3build
	xterm -e "gdb -ex 'target remote localhost:1234' -ex 'symbol-file bin/KaOS.sym'" &
	qemu-system-i386 -s -S -cdrom bin/KaOS.iso -monitor stdio -serial pty
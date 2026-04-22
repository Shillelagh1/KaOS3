ENTRY(_start)
OUTPUT(bin/KaOS.bin)

SECTIONS
{
	/* Kernel begins at 1 meb */
	. = 1M;

	.multiboot.data : {
		*(.multiboot.data)
	}

	.multiboot.text : {
		*(.multiboot.text)
	}

	. += 0xFFC00000;
	_ROMBEGIN = .;
	.text ALIGN (4K) : AT (ADDR (.text) - 0xFFC00000) {
		_CODEBEGIN = .;
		*(.text)
		_CODEEND = .;
	}
	.rodata ALIGN (4K) : AT (ADDR (.rodata) - 0xFFC00000) {
		*(.rodata)
	}
	.data ALIGN (4K) : AT (ADDR (.data) - 0xFFC00000) {
		*(.data)
	}
	.bss ALIGN (4K) : AT (ADDR (.bss) - 0xFFC00000) {
		*(COMMON)
		*(.bss)
	}
	_ROMEND = .;
}
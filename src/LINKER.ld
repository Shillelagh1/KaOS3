ENTRY(_start)
OUTPUT(bin/KaOS.bin)

SECTIONS
{
	/* Kernel begins at 1 meg */
	. = 1M;

	_ROMBEGIN = .;
	.multiboot.data : {
		*(.multiboot.data)
	}

	.multiboot.text : {
		*(.multiboot.text)
	}

	. += 0xC0000000;
	_ROMBEGIN = .;
	.text ALIGN (4K) : AT (ADDR (.text) - 0xC0000000) {
		_CODEBEGIN = .;
		*(.text)
		_CODEEND = .;
	}
	.rodata ALIGN (4K) : AT (ADDR (.rodata) - 0xC0000000) {
		*(.rodata)
	}
	.data ALIGN (4K) : AT (ADDR (.data) - 0xC0000000) {
		*(.data)
	}
	.bss ALIGN (4K) : AT (ADDR (.bss) - 0xC0000000) {
		*(COMMON)
		*(.bss)
	}
	_ROMEND = .;
}
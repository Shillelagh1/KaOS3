ENTRY(_start)
OUTPUT(bin/KaOS.bin)

HH_OFFSET = 0xFFC00000;

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

	. += HH_OFFSET;
	_ROMBEGIN = .;
	.text ALIGN (4K) : AT (ADDR (.text) - HH_OFFSET) {
		_CODEBEGIN = .;
		*(.text)
		_CODEEND = .;
	}
	.rodata ALIGN (4K) : AT (ADDR (.rodata) - HH_OFFSET) {
		*(.rodata)
	}
	.data ALIGN (4K) : AT (ADDR (.data) - HH_OFFSET) {
		*(.data)
	}
	.bss ALIGN (4K) : AT (ADDR (.bss) - HH_OFFSET) {
		*(COMMON)
		*(.bss)
	}
	_ROMEND = .;
}
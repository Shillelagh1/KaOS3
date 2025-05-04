ENTRY(_start)
OUTPUT(bin/KaOS.bin)

SECTIONS
{
	. = 2M;
	_ROMBEGIN = .;

	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
		_CODEBEGIN = .;
		*(.text)
		_CODEEND = .;
	}

	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}

	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}

	_ROMEND = .;
}
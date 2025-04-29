ENTRY(_start)
INPUT
(
    tmp/boot_s.o
    tmp/GDT_c.o
	tmp/GDT_s.o
	tmp/IDT_c.o
	tmp/IDT_s.o
	tmp/int_errh_c.o
)
OUTPUT(bin/TOS3.bin)

SECTIONS
{
	. = 2M;

	.text BLOCK(4K) : ALIGN(4K)
	{
		_kernel_start = .;
		*(.multiboot)
		*(.text)
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

	_KERNELEND = .;
}
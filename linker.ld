ENTRY(start)

SECTIONS {

    . = 0xC0010000;
    _start = .;
    _text_start = .;
    .text :
    {
        KEEP(*(.text.boot));

        . = 0x1000;
        KEEP(*(.text.pt_boot));

        . = 0x2000;
        blank_pagetable = .;
        KEEP(*(.text.pt_blank));

        . = 0x4000;
        KEEP(*(.text.pt_kernel));
        *(.text)
    }
    . = ALIGN(4096);
    _text_end = .;

    _rodata_start = .;
    .rodata :
    {
        *(.rodata)
    }
    . = ALIGN(4096);
    _rodata_end = .;

    _data_start = .;
    .data :
    {
        . = ALIGN(4096);

        *(.data)
    }
    . = ALIGN(4096);
    _data_end = .;

    _bss_start = .;
    .bss :
    {
        *(.bss)
    }
    . = ALIGN(4096);
    _bss_end = .;
    _end = .;
}

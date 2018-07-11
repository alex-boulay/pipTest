OUTPUT_FORMAT("binary")
ENTRY(main)
SECTIONS {
    .text 0x700000 :
    {
        *(.text)
        . = ALIGN(0x1000);
    }

    .data :
    {
        *(.data)
        *(.rodata)
        . = ALIGN(0x1000);
    }

    .bss :
    {
        *(.bss)
    }
    end = .;
}

OUTPUT_ARCH( "riscv" )
ENTRY(_start)

MEMORY
{
    RAM : ORIGIN = 0x80000000, LENGTH = 0x400000
}

SECTIONS
{
    .text.init :
    {
        *(.text.init)
    } > RAM

    .text :
    {
        *(.text)
    } > RAM

    .data :
    {
        *(.sdata)
        *(.data)
    } > RAM

    _edata = .;
    .bss :
    {
        PROVIDE( __global_pointer$ = . + 0x100);
        *(.bss)
    } > RAM
    _end = .;

    . = ALIGN(0x10);
    __init_sp = . + 0x800;
}


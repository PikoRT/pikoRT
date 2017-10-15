OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)

#define RAMORG 0x20000000

MEMORY
{
    rom (rx)  : ORIGIN = 0, LENGTH = ROMSZ
    ram (rwx) : ORIGIN = RAMORG, LENGTH = RAMSZ
}

SECTIONS
{
    .vector 0 : {
        PROVIDE(__vector_start__ = .);
        KEEP(*(.vector))
    } > rom

    .text BLOCK(8) : {
        PROVIDE(__text_start__ = .);
        *(.text*)
        PROVIDE(__text_end__ = .);
        . = ALIGN(8);
        PROVIDE(__shell_cmd_start__ = .);
        KEEP(*(.shell_cmd*))
        PROVIDE(__shell_cmd_end__ = .);
        PROVIDE(__sched_classes_start__ = .);
        KEEP(*(.sched.class*))
        PROVIDE(__sched_classes_end__ = .);
        PROVIDE(__rodata_start__ = .);
        *(.rodata*)
        PROVIDE(__rodata_end__ = .);
   } > rom

    /* Initialized data (.data* sections) are initially stored in Flash,
       but need to be copied to a volatile storage for they are not
       read-only.    */

    .data : AT (__rodata_end__) {
        PROVIDE(__data_start__ = .);
        *(.data*)
        PROVIDE(__data_end__ = .);
    } > ram

    PROVIDE(__data_size__ = SIZEOF(.data));

    .bss : {
        PROVIDE(__bss_start__ = .);
        *(.bss*)
        *(COMMON)
        PROVIDE(__bss_end__ = .);
    } > ram

    PROVIDE(__bss_size__ = SIZEOF(.bss));

    /* heap for the kernel's malloc */
    .heap BLOCK(32) : {
        PROVIDE(__heap_start__ = .);
        . += 4k;
        PROVIDE(__heap_end__ = .);
    } > ram

    PROVIDE(__heap_size__ = SIZEOF(.heap));

    .pgmem RAMORG + RAMSZ - 32k : {
        PROVIDE(__pgmem_start__ = .);
        . += 26k;

        /* Reserve the last page for the early stack. Must be of the biggest size
         * such that there will be no coalescing when freeing the page, for lower
         * booting time.    */
        PROVIDE(__early_stack_end__ = .);
        . += 2k;
        PROVIDE(__early_stack_start__ = .);

        PROVIDE(__pgmem_end__ = .);
    } > ram

    PROVIDE(__pgmem_size__ = SIZEOF(.pgmem));

    .mtdram : {
        PROVIDE(__mtdram_start__ = .);
        . += 4k;
    } > ram

    PROVIDE(__mtdram_size__ = SIZEOF(.mtdram));

    /DISCARD/ :
    {
        *(.ARM.exidx)
        *(.ARM.attributes)
    }
}

#ifndef _KERNEL_LINKAGE_H
#define _KERNEL_LINKAGE_H

#ifdef __ASSEMBLER__

#define ENTRY(name) \
    .globl name;    \
    .align 4;       \
    name:

#define END(name) .size name, .- name

#define ENDPROC(name)       \
    .type name, % function; \
    END(name)

#endif /* __ASSEMBLER__ */

#endif /* !_KERNEL_LINKAGE_H */

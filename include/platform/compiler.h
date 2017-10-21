#ifndef _PLATFORM_COMPILER_H
#define _PLATFORM_COMPILER_H

#define PLAT_EVAL(macro)                \
    ({                                  \
        void __wrapper__(void) {macro;} \
        __wrapper__;                    \
    })

#endif /* !_PLATFORM_COMPILER_H */

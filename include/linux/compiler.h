#ifndef _LINUX_COMPILER_H
#define _LINUX_COMPILER_H

#define READ_ONCE(x) (x)
#define WRITE_ONCE(x, val) \
    ({                     \
        x = val;           \
        val;               \
    })

#endif /* !_LINUX_COMPILER_H */

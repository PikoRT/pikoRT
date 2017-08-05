#ifndef _KERNEL_TYPES_H
#define _KERNEL_TYPES_H

#ifndef _U32
#define _U32
typedef unsigned int u32;
#endif

#ifndef _S32
#define _S32
typedef int s32;
#endif

#ifndef _U16
#define _U16
typedef unsigned short u16;
#endif

#ifndef _S16
#define _S16
typedef short s16;
#endif

#ifndef _U8
#define _U8
typedef unsigned char u8;
#endif

#ifndef _S8
#define _S8
typedef char s8;
#endif

#ifndef _UMODE_T
#define _UMODE_T
typedef unsigned short umode_t;
#endif

typedef u32 __u32;
typedef s32 __s32;
typedef u16 __u16;
typedef s16 __s16;
typedef u8 __u8;
typedef s8 __s8;

/* import struct list_head */
#include "linux/types.h"

#endif /* !_KERNEL_TYPES_H */

#ifndef _LINUX_STDDEF_H
#define _LINUX_STDDEF_H

#include <stddef.h>

/**
 * offsetofend(TYPE, MEMBER)
 *
 * @TYPE: The type of the structure
 * @MEMBER: The member within the structure to get the end offset of
 */
#define offsetofend(TYPE, MEMBER) \
    (offsetof(TYPE, MEMBER) + sizeof(((TYPE *) 0)->MEMBER))

#endif

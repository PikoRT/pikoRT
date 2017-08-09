#ifndef LIBC_ARPA_INET_H
#define LIBC_ARPA_INET_H

#include <stdint.h>

#include "platform.h"

static inline uint32_t htonl(uint32_t hostlong)
{
    return __REV(hostlong);
}

static inline uint16_t htons(uint16_t hostshort)
{
    return __REV16(hostshort);
}

static inline uint32_t ntohl(uint32_t netlong)
{
    return __REV(netlong);
}

static inline uint16_t ntohs(uint16_t netshort)
{
    return __REV16(netshort);
}

#endif /* !LIBC_ARPA_INET_H */

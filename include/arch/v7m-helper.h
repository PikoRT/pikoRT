#ifndef _ARCH_V7M_HELPER_H
#define _ARCH_V7M_HELPER_H

#define V7M_EXC_RETURN_HANDLER_MAIN 0xfffffff1
#define V7M_EXC_RETURN_THREAD_MAIN 0xfffffff9
#define V7M_EXC_RETURN_THREAD_PROCESS 0xfffffffd

static inline void *v7m_set_thumb_bit(void *addr)
{
	return (void *) ((unsigned long) addr | 1ul);
}

static inline void *v7m_clear_thumb_bit(void *addr)
{
	return (void *) ((unsigned long) addr & ~1ul);
}

#endif /* !_ARCH_V7M_HELPER_H */

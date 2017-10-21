#ifndef _PLATFORM_STM32_PLATFORM_H
#define _PLATFORM_STM32_PLATFORM_H

#include <cmsis.h>

void __platform_init(void);
void __platform_halt(void);


/* Default USART for outputing */
#define USARTx          USART2
#define USARTx_IRQn     USART2_IRQn

#endif /* !_PLATFORM_STM32_PLATFORM_H */

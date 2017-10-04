#ifndef _PLATFORM_STM32_PLATFORM_H
#define _PLATFORM_STM32_PLATFORM_H

#include <cmsis.h>

void __platform_init(void);
void __platform_halt(void);

/* Default USART for outputing
 *  Hard code in "driver/stm32-usart.c"
 */
#define USARTx          USART1
#define USARTx_IRQn     USART1_IRQn

#endif /* !_PLATFORM_STM32_PLATFORM_H */

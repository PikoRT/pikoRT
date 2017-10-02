#include <kernel/compiler.h>

#include "platform.h"

#define CPU_FREQ_IN_HZ 72000000
#define SYSTICK_FREQ_IN_HZ 1000
#define SYSTICK_PERIOD_IN_MSECS (SYSTICK_FREQ_IN_HZ / 1000)

struct timer_operations;

void config_timer_operations(struct timer_operations *tops);

extern struct timer_operations systick_tops;

void rcc_clock_init(void);
void __uart_enable(void);

__weak void __platform_init(void)
{
    config_timer_operations(&systick_tops);

    /* SysTick running at 1kHz */
    SysTick_Config(CPU_FREQ_IN_HZ / SYSTICK_FREQ_IN_HZ);
}

__weak void __platform_halt(void)
{
    for (;;)
        ;
}

void __printk_init(void)
{
    /* Enable peripherals and GPIO Clocks */

    /* Enable GPIO TX/RX clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /* Enable USART2 clock */
    __HAL_RCC_USART2_CLK_ENABLE();

    /* GPIO initialized with UART Tx/Rx configuration */
    GPIO_InitTypeDef GPIO_InitStruct = {
        .Pin = GPIO_PIN_2,
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_PULLUP,
        .Speed = GPIO_SPEED_FREQ_HIGH,
    };

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART init */

    /* Set the USART peripheral in the Asynchronous mode (UART Mode) */
    /* UART2 configured as follow:
        - Word Length = 8 Bits
        - Stop Bit = One Stop bit
        - Parity = None
        - BaudRate = 9600 baud
        - Hardware flow control disabled (RTS and CTS signals) */
    UART_HandleTypeDef UartHandle = {
        .Instance = USART2,
        .Init =
            {
                .BaudRate = 115200,
                .WordLength = UART_WORDLENGTH_8B,
                .StopBits = UART_STOPBITS_1,
                .Parity = UART_PARITY_NONE,
                .HwFlowCtl = UART_HWCONTROL_NONE,
                .Mode = UART_MODE_TX_RX | UART_IT_RXNE,
                .OverSampling = UART_OVERSAMPLING_16,
            },
    };

    HAL_UART_Init(&UartHandle);

    NVIC_SetPriority(USART2_IRQn, 0xE);
}

void __printk_putchar(char c)
{
    if (c == '\n')
        __printk_putchar('\r');

    while (!(USART2->SR & USART_SR_TXE))
        ;
    USART2->DR = (0xff) & c;
}

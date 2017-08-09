#include <kernel/compiler.h>

#include "platform.h"

#define CPU_FREQ_IN_HZ 72000000
#define SYSTICK_FREQ_IN_HZ 1000
#define SYSTICK_PERIOD_IN_MSECS (SYSTICK_FREQ_IN_HZ / 1000)

struct timer_operations;

void config_timer_operations(struct timer_operations *tops);

extern struct timer_operations systick_tops;

void stm32p103_init(void);

void rcc_clock_init(void);
void __uart_enable(void);

/**
 * FIXME: There are some unexpected bug for qemu with STM32CubeMX
 */
__weak void SystemInit(void)
{
    rcc_clock_init();
}

__weak void __platform_init(void)
{
    config_timer_operations(&systick_tops);

    /* SysTick running at 1kHz */
    SysTick_Config(CPU_FREQ_IN_HZ / SYSTICK_FREQ_IN_HZ);

    /* create /dev/ttyS0, serial interface for QEMU UART0 */
    stm32p103_init();
}

__weak void __platform_halt(void)
{
    for (;;)
        ;
}

void __printk_init(void)
{
    // UART0->CTL |= 1;         /* UART enabled */
    // UART0->LCRH |= (3 << 5); /* 8 bits word length, no parity */

    __uart_enable();

    NVIC_SetPriority(USART2_IRQn, 0xE);

    // USART2->CR1 |= (USART_CR1_UE | USART_CR1_TE | USART_CR1_RE);
}

void __printk_putchar(char c)
{
    if (c == '\n')
        __printk_putchar('\r');

    while (!(USART2->SR & USART_SR_TXE))
        ;
    USART2->DR = (0xff) & c;
}

void rcc_clock_init(void)
{
    /* Reset the RCC clock configuration to the default reset state(for debug
     * purpose) */
    /* Set HSION bit */
    RCC->CR |= (uint32_t) 0x00000001;

    /* Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits */
    RCC->CFGR &= (uint32_t) 0xF8FF0000;

    /* Reset HSEON, CSSON and PLLON bits */
    RCC->CR &= (uint32_t) 0xFEF6FFFF;

    /* Reset HSEBYP bit */
    RCC->CR &= (uint32_t) 0xFFFBFFFF;

    /* Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE bits */
    RCC->CFGR &= (uint32_t) 0xFF80FFFF;

    /* Disable all interrupts and clear pending bits  */
    RCC->CIR = 0x009F0000;

    /* Configure the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers */
    /* Configure the Flash Latency cycles and enable prefetch buffer */
    volatile uint32_t StartUpCounter = 0, HSEStatus = 0;

    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/
    /* Enable HSE */
    RCC->CR |= ((uint32_t) RCC_CR_HSEON);

    /* Wait till HSE is ready and if Time out is reached exit */
    do {
        HSEStatus = RCC->CR & RCC_CR_HSERDY;
        StartUpCounter++;
    } while ((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    if ((RCC->CR & RCC_CR_HSERDY) != 0)
        HSEStatus = (uint32_t) 0x01;
    else
        HSEStatus = (uint32_t) 0x00;

    if (HSEStatus == (uint32_t) 0x01) {
        /* Enable Prefetch Buffer */
        FLASH->ACR |= FLASH_ACR_PRFTBE;

        /* Flash 0 wait state */
        FLASH->ACR &= (uint32_t)((uint32_t) ~FLASH_ACR_LATENCY);

        FLASH->ACR |= (uint32_t) FLASH_ACR_LATENCY_0;

        /* HCLK = SYSCLK */
        RCC->CFGR |= (uint32_t) RCC_CFGR_HPRE_DIV1;

        /* PCLK2 = HCLK */
        RCC->CFGR |= (uint32_t) RCC_CFGR_PPRE2_DIV1;

        /* PCLK1 = HCLK */
        RCC->CFGR |= (uint32_t) RCC_CFGR_PPRE1_DIV1;

        /* Select HSE as system clock source */
        RCC->CFGR &= (uint32_t)((uint32_t) ~(RCC_CFGR_SW));
        RCC->CFGR |= (uint32_t) RCC_CFGR_SW_HSE;

        /* Wait till HSE is used as system clock source */
        while ((RCC->CFGR & (uint32_t) RCC_CFGR_SWS) != (uint32_t) 0x04)
            ;
    } else {
        /* If HSE fails to start-up, the application will have wrong clock
        configuration. User can add here some code to deal with this error */
    }
}

void __uart_enable(void)
{
    (RCC->APB2ENR) |= (uint32_t)(0x00000001 | 0x00000004);
    (RCC->APB1ENR) |= (uint32_t)(0x00020000);

    (GPIOA->CRL) = 0x00004B00;
    (GPIOA->CRH) = 0x44444444;
    (GPIOA->ODR) = 0x00000000;
    (GPIOA->BSRR) = 0x00000000;
    (GPIOA->BRR) = 0x00000000;

    USART2->CR1 = 0x0000000C;
    USART2->CR1 |= 0x2000;
    USART2->CR1 |= 0x0020;
}

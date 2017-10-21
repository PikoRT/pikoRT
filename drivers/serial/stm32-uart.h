#ifndef _DRIVER_SERIAL_STM32_USART_H
#define _DRIVER_SERIAL_STM32_USART_H

#include "platform.h"

struct stm32_uart_port {
    GPIO_TypeDef *gpio_tx;
    GPIO_TypeDef *gpio_rx;

    GPIO_InitTypeDef gpio_tx_init_info;
    GPIO_InitTypeDef gpio_rx_init_info;

    UART_HandleTypeDef uart_init_info;

    void (*gpio_init)(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init);

    HAL_StatusTypeDef (*uart_init)(UART_HandleTypeDef *huart);

    void (*gpio_tx_clk_enable)(void);
    void (*gpio_rx_clk_enable)(void);
    void (*uart_clk_enable)(void);
};

#endif /* !_DRIVER_SERIAL_STM32_USART_H */

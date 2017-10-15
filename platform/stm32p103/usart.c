#include <serial.h>
#include <platform/compiler.h>

#include "platform.h"
#include "kernel/kernel.h"
#include "stm32-usart.h"

#define STM32_USART_MAX 3

static void usart_port_setup(struct stm32_usart_port *port) {
    /* Enable peripherals and GPIO Clocks */
    /* Enable GPIO TX/RX clock */
    port->gpio_tx_clk_enable();
    port->gpio_rx_clk_enable();

    /* Enable USART/UART clock */
    port->usart_clk_enable();
}

static void usart_port_init(struct stm32_usart_port *port) {
    /* GPIO initialized with USART/UART Tx/Rx configuration */
    port->gpio_init(port->gpio_tx, &port->gpio_tx_init_info);
    port->gpio_init(port->gpio_rx, &port->gpio_rx_init_info);

    /* USART/UART init */
    /* Set the USART peripheral in the Asynchronous mode (UART Mode) */
    if (port->uart_init)
        port->uart_init(&port->uart_init_info);
    else
        port->usart_init(&port->usart_init_info);
}


void uart_init(void)
{
    struct stm32_usart_port ports[STM32_USART_MAX] = {
        /* [0] USART1 */
        /* [1] USART2 */
        [1] = {
            .gpio_tx = GPIOA,
            .gpio_rx = GPIOA,
     
            .gpio_tx_init_info = {
                .Pin = GPIO_PIN_2,
                .Mode = GPIO_MODE_AF_PP,
                .Pull = GPIO_PULLUP,
                .Speed = GPIO_SPEED_FREQ_HIGH,
            },
            .gpio_rx_init_info = {
                .Pin = GPIO_PIN_3,
                .Mode = GPIO_MODE_INPUT,
                .Pull = GPIO_PULLUP,
                .Speed = GPIO_SPEED_FREQ_HIGH,
            },
             /* UART2 configured as follow:
                 - Word Length = 8 Bits
                 - Stop Bit = One Stop bit
                 - Parity = None
                 - BaudRate = 9600 baud
                 - Hardware flow control disabled (RTS and CTS signals) */
            .uart_init_info = {
                .Instance = USART2,
                .Init = {
                    .BaudRate = 115200,
                    .WordLength = UART_WORDLENGTH_8B,
                    .StopBits = UART_STOPBITS_1,
                    .Parity = UART_PARITY_NONE,
                    .HwFlowCtl = UART_HWCONTROL_NONE,
                    .Mode = UART_MODE_TX_RX | UART_IT_RXNE,
                    .OverSampling = UART_OVERSAMPLING_16,
                },
            },
    
            .gpio_init = HAL_GPIO_Init,
            .uart_init = HAL_UART_Init,
        
            .gpio_tx_clk_enable = PLAT_EVAL(__HAL_RCC_GPIOA_CLK_ENABLE()),
            .gpio_rx_clk_enable = PLAT_EVAL(__HAL_RCC_GPIOA_CLK_ENABLE()),
            .usart_clk_enable = PLAT_EVAL(__HAL_RCC_USART2_CLK_ENABLE()),
       },
        /* [3] USART2 */
    };

    /* Specify initialize order */
    int init_order[] = {1};

    /* USART/UART port setup */
    for (size_t i = 0; i < ARRAY_SIZE(init_order); i++)
        usart_port_setup(&ports[init_order[i]]);

    /* USART/UART port init */
    for (size_t i = 0; i < ARRAY_SIZE(init_order); i++)
        usart_port_init(&ports[init_order[i]]);
}

#include <serial.h>
#include <platform/compiler.h>

#include "platform.h"
#include "kernel/kernel.h"
#include "stm32-uart.h"

#define STM32_USART_MAX 8

static void uart_port_setup(struct stm32_uart_port *port)
{
    /* Enable peripherals and GPIO Clocks */
    /* Enable GPIO TX/RX clock */
    port->gpio_tx_clk_enable();
    port->gpio_rx_clk_enable();

    /* Enable USART/UART clock */
    port->uart_clk_enable();
}

static void uart_port_init(struct stm32_uart_port *port)
{
    /* GPIO initialized with USART/UART Tx/Rx configuration */
    port->gpio_init(port->gpio_tx, &port->gpio_tx_init_info);
    port->gpio_init(port->gpio_rx, &port->gpio_rx_init_info);

    /* UART init */
    port->uart_init(&port->uart_init_info);
}

void uart_init(void)
{
    // clang-format off
    struct stm32_uart_port ports[STM32_USART_MAX] = {
        /* [0] USART1 */
        [0] = {
            .gpio_tx = GPIOA,
            .gpio_rx = GPIOA,
     
            .gpio_tx_init_info = {
                .Pin = GPIO_PIN_9,
                .Mode = GPIO_MODE_AF_PP,
                .Pull = GPIO_NOPULL,
                .Speed = GPIO_SPEED_FAST,
                .Alternate = GPIO_AF7_USART1,
            },
            .gpio_rx_init_info = {
                .Pin = GPIO_PIN_10,
                .Mode = GPIO_MODE_AF_PP,
                .Pull = GPIO_NOPULL,
                .Speed = GPIO_SPEED_FAST,
                .Alternate = GPIO_AF7_USART1,
            },
             /* UART2 configured as follow:
                 - Word Length = 8 Bits
                 - Stop Bit = One Stop bit
                 - Parity = None
                 - BaudRate = 9600 baud
                 - Hardware flow control disabled (RTS and CTS signals) */
            .uart_init_info = {
                .Instance = USART1,
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
            .uart_clk_enable = PLAT_EVAL(__HAL_RCC_USART1_CLK_ENABLE()),
       },
        /* [1] USART2 */
        /* [2] USART3 */
        /* [3] UART4 */
        /* [4] UART5 */
        /* [5] USART6 */
        /* [6] UART7 */
        /* [7] UART8 */
    };
    // clang-format on

    /* Specify initialize order */
    int init_order[] = {0};

    /* USART/UART port setup */
    for (size_t i = 0; i < ARRAY_SIZE(init_order); i++)
        uart_port_setup(&ports[init_order[i]]);

    /* USART/UART port init */
    for (size_t i = 0; i < ARRAY_SIZE(init_order); i++)
        uart_port_init(&ports[init_order[i]]);
}

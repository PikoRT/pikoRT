#include <serial.h>
#include <platform/compiler.h>

#include "platform.h"
#include "kernel/kernel.h"

#define STM32_USART_MAX 3

struct usart_port_setup_info {
    GPIO_TypeDef *gpio_tx;
    GPIO_TypeDef *gpio_rx;

    GPIO_InitTypeDef gpio_tx_init_info;
    GPIO_InitTypeDef gpio_rx_init_info;

    union {
        USART_HandleTypeDef usart_init_info;
        UART_HandleTypeDef  uart_init_info;
    };

    void (*gpio_init)(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init);

    HAL_StatusTypeDef (*uart_init)(UART_HandleTypeDef *huart);
    HAL_StatusTypeDef (*usart_init)(USART_HandleTypeDef *husart);

    void (*gpio_tx_clk_enable)(void);
    void (*gpio_rx_clk_enable)(void);
    void (*usart_clk_enable)(void);
};

static void usart_port_setup(struct usart_port_setup_info *info) {
    /* Enable peripherals and GPIO Clocks */
    /* Enable GPIO TX/RX clock */
    info->gpio_tx_clk_enable();
    info->gpio_rx_clk_enable();

    /* Enable USART/UART clock */
    info->usart_clk_enable();
}

static void usart_port_init(struct usart_port_setup_info *info) {
    /* GPIO initialized with USART/UART Tx/Rx configuration */
    info->gpio_init(info->gpio_tx, &info->gpio_tx_init_info);
    info->gpio_init(info->gpio_rx, &info->gpio_rx_init_info);

    /* USART/UART init */
    /* Set the USART peripheral in the Asynchronous mode (UART Mode) */
    if (info->uart_init)
        info->uart_init(&info->uart_init_info);
    else
        info->usart_init(&info->usart_init_info);
}


void uart_init(void)
{
    struct usart_port_setup_info infos[STM32_USART_MAX] = {
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
    for (size_t i = 0; i < ARRAY_SIZE(init_order); i++) {
        int idx = init_order[i];
        usart_port_setup(&infos[idx]);
    }

    /* USART/UART port init */
    for (size_t i = 0; i < ARRAY_SIZE(init_order); i++) {
        int idx = init_order[i];
        usart_port_init(&infos[idx]);
    }
}

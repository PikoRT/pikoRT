CPU = cortex-m3
ARCH = armv7-m

ROMSZ = 128k
RAMSZ = 64k

CFLAGS += \
    -Iplatform/stm32p103 \
    -I$(CMSIS)/util \
    -I$(CMSIS)/arm \
    -I$(CMSIS)/arm/TARGET_CORTEX_M \
    -I$(CMSIS)/arm/TARGET_CORTEX_M/TOOLCHAIN_GCC \
    -I$(CMSIS)/TARGET_STM \
    -I$(CMSIS)/TARGET_STM//TARGET_STM32F1 \
    -I$(CMSIS)/TARGET_STM//TARGET_STM32F1/device \
    -I$(CMSIS)/TARGET_STM//TARGET_STM32F1/TARGET_NUCLEO_F103RB \
    -I$(CMSIS)/TARGET_STM//TARGET_STM32F1/TARGET_NUCLEO_F103RB/device

CSRC += \
    platform/stm32p103/halt.c \
    platform/stm32p103/init.c \
    platform/stm32p103/usart.c

# CMSIS files
## STM32 HAL
CSRC += $(CMSIS)/TARGET_STM/TARGET_STM32F1/device/system_stm32f1xx.c
CSRC += $(CMSIS)/TARGET_STM/TARGET_STM32F1/device/stm32f1xx_hal_rcc.c
CSRC += $(CMSIS)/TARGET_STM/TARGET_STM32F1/device/stm32f1xx_hal_rcc_ex.c
CSRC += $(CMSIS)/TARGET_STM/TARGET_STM32F1/device/stm32f1xx_hal_gpio.c
CSRC += $(CMSIS)/TARGET_STM/TARGET_STM32F1/device/stm32f1xx_hal_uart.c

## SystemInit()
CSRC += $(CMSIS)/TARGET_STM/TARGET_STM32F1/TARGET_NUCLEO_F103RB/device/system_clock.c

# Timer driver files
CSRC += drivers/timer/systick.c

# Serial driver files
CSRC += drivers/serial/stm32-usart.c

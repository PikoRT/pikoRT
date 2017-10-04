CPU = cortex-m4
ARCH = armv7-m

ROMSZ = 2048k
RAMSZ = 192k

CFLAGS += \
    -Iplatform/f429disco \
    -I$(CMSIS)/util \
    -I$(CMSIS)/arm \
    -I$(CMSIS)/arm/TARGET_CORTEX_M \
    -I$(CMSIS)/arm/TARGET_CORTEX_M/TOOLCHAIN_GCC \
    -I$(CMSIS)/TARGET_STM \
    -I$(CMSIS)/TARGET_STM//TARGET_STM32F4 \
    -I$(CMSIS)/TARGET_STM//TARGET_STM32F4/device \
    -I$(CMSIS)/TARGET_STM//TARGET_STM32F4/TARGET_STM32F429xI \
    -I$(CMSIS)/TARGET_STM//TARGET_STM32F4/TARGET_STM32F429xI/device \
    -I$(CMSIS)/TARGET_STM//TARGET_STM32F4/TARGET_STM32F429xI/TARGET_DISCO_F429ZI

CSRC += \
    platform/f429disco/halt.c \
    platform/f429disco/init.c

# CMSIS files
## STM32 HAL
CSRC += $(CMSIS)/TARGET_STM/TARGET_STM32F4/device/system_stm32f4xx.c
CSRC += $(CMSIS)/TARGET_STM/TARGET_STM32F4/device/stm32f4xx_hal_rcc.c
CSRC += $(CMSIS)/TARGET_STM/TARGET_STM32F4/device/stm32f4xx_hal_rcc_ex.c
CSRC += $(CMSIS)/TARGET_STM/TARGET_STM32F4/device/stm32f4xx_hal_gpio.c
CSRC += $(CMSIS)/TARGET_STM/TARGET_STM32F4/device/stm32f4xx_hal_uart.c

## SystemInit()
CSRC += $(CMSIS)/TARGET_STM/TARGET_STM32F4/TARGET_STM32F429xI/TARGET_DISCO_F429ZI/system_clock.c

# Timer driver files
CSRC += drivers/timer/systick.c

# Serial driver files
CSRC += drivers/serial/stm32-usart.c

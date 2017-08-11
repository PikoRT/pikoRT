CPU = cortex-m3
ARCH = armv7-m

ROMSZ = 128k
RAMSZ = 64k

CFLAGS += \
    -Itarget/stm32p103 \
    -I$(CMSIS)/util \
    -I$(CMSIS)/arm \
    -I$(CMSIS)/arm/TARGET_CORTEX_M \
    -I$(CMSIS)/arm/TARGET_CORTEX_M/TOOLCHAIN_GCC \
    -I$(CMSIS)/TARGET_STM \
    -I$(CMSIS)/TARGET_STM//TARGET_STM32F1 \
    -I$(CMSIS)/TARGET_STM//TARGET_STM32F1/device \
    -I$(CMSIS)/TARGET_STM//TARGET_STM32F1/TARGET_NUCLEO_F103RB \
    -I$(CMSIS)/TARGET_STM//TARGET_STM32F1/TARGET_NUCLEO_F103RB/device \
    -I$(CMSIS)/TARGET_STM//TARGET_STM32F1/TARGET_NUCLEO_F103RB/TARGET_DISCO_F429ZI

CSRC += \
    target/stm32p103/halt.c \
    target/stm32p103/init.c

# CMSIS files
## STM32 HAL
CSRC += $(wildcard ../cmsis/TARGET_STM/TARGET_STM32F1/device/*.c)
## SystemInit()
CSRC += ../cmsis/TARGET_STM/TARGET_STM32F1/TARGET_NUCLEO_F103RB/device/system_clock.c

# Timer driver files
CSRC += drivers/timer/systick.c

# Serial driver files
CSRC += drivers/serial/stm32f1xx.c

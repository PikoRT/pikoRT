CPU	= cortex-m4
ARCH	= armv7-m

ROMSZ	= 2048k
RAMSZ	= 192k

CFLAGS	+=	-Itarget/stm32f429 \
			-I$(CMSIS)/util \
			-I$(CMSIS)/arm \
			-I$(CMSIS)/arm/TARGET_CORTEX_M \
			-I$(CMSIS)/arm/TARGET_CORTEX_M/TOOLCHAIN_GCC \
			-I$(CMSIS)/TARGET_STM \
			-I$(CMSIS)/TARGET_STM//TARGET_STM32F4 \
			-I$(CMSIS)/TARGET_STM//TARGET_STM32F4/device \
			-I$(CMSIS)/TARGET_STM//TARGET_STM32F4/TARGET_STM32F429xI \
			-I$(CMSIS)/TARGET_STM//TARGET_STM32F4/TARGET_STM32F429xI/device \
			-I$(CMSIS)/TARGET_STM//TARGET_STM32F4/TARGET_STM32F429xI/TARGET_DISCO_F429ZI \

CSRC	+=				\
	target/stm32f429/halt.c		\
	target/stm32f429/init.c

# CMSIS files
## STM HAL
CSRC	+= $(wildcard ../cmsis/TARGET_STM/TARGET_STM32F4/device/*.c)
## SystemInit()
CSRC	+= ../cmsis/TARGET_STM/TARGET_STM32F4/TARGET_STM32F429xI/TARGET_DISCO_F429ZI/system_clock.c

# Timer driver files
CSRC	+= drivers/timer/systick.c

# Serial driver files
CSRC	+= drivers/serial/stm32f429.c

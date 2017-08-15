CROSS_COMPILE = arm-none-eabi-
CC = $(CROSS_COMPILE)gcc
AS = $(CROSS_COMPILE)as
AR = $(CROSS_COMPILE)ar
OBJCOPY = $(CROSS_COMPILE)objcopy
GDB = $(CROSS_COMPILE)gdb
HOSTCC  = gcc

# FIXME: configurable via menuconfig or command line
CFLAGS_OPT = -Os # -flto

CFLAGS += \
    -std=c99 \
    -W -Wall \
    -Iinclude -Iinclude/libc -I. \
    -I$(CMSIS)/arm -I$(CMSIS)/$(TARGET) -I$(CMSIS)/$(TARGET)/hal \
    -Iinclude/kernel \
    -D_POSIX_THREADS=1 -D_POSIX_TIMERS=1 -D_POSIX_REALTIME_SIGNALS=1 \
    -Wno-main -fdiagnostics-color \
    -ffunction-sections -fdata-sections -ggdb \
    $(CFLAGS_OPT)

# FIXME: make Piko-specific build options configurable
CFLAGS += \
    -D CONFIG_KERNEL_STACK_CHECKING

LDFLAGS += \
    -nostartfiles -specs=nano.specs \
    -Wl,-Map=$(NAME).map -Wl,-Tpiko.lds -Wl,--gc-sections

CFLAGS  += -mthumb -mcpu=$(CPU)
LDFLAGS += -mthumb -march=$(ARCH)

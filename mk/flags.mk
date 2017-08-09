CROSS   = arm-none-eabi-
CC      = $(CROSS)gcc
AS      = $(CROSS)as
AR      = $(CROSS)ar
OBJCOPY = $(CROSS)objcopy
GDB     = $(CROSS)gdb
HOSTCC  = gcc

CFLAGS += \
    -Iinclude -Iinclude/libc -I. \
    -I$(CMSIS)/arm -I$(CMSIS)/$(TARGET) -I$(CMSIS)/$(TARGET)/hal \
    -Iinclude/kernel \
    -D_POSIX_THREADS=1 -D_POSIX_TIMERS=1 -D_POSIX_REALTIME_SIGNALS=1 \
    -Wno-main -DCONFIG_KERNEL_STACK_CHECKING -fdiagnostics-color \
    -ffunction-sections -fdata-sections -Os -ggdb

LDFLAGS += \
    -nostartfiles -specs=nano.specs \
    -Wl,-Map=$(NAME).map -Wl,-Tpiko.lds -Wl,--gc-sections

CFLAGS  += -mthumb -mcpu=$(CPU)
LDFLAGS += -mthumb -march=$(ARCH)

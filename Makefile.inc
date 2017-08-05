CROSS   = arm-none-eabi-
CC      = $(CROSS)gcc
AS      = $(CROSS)as
AR      = $(CROSS)ar
OBJCOPY = $(CROSS)objcopy
GDB     = $(CROSS)gdb
HOSTCC  = gcc

CFLAGS  += -mthumb -mcpu=$(CPU)
LDFLAGS += -mthumb -march=$(ARCH)

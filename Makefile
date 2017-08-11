NAME = piko

# select QEMU when the target is unspecified
TARGET ?= stm32f429
CMSIS = external/cmsis

# The platform Makefile contains hw details and flags
include target/$(TARGET)/Makefile

# arch-specific
SSRC += arch/v7m-head.S arch/v7m-entry.S arch/v7m-svcall.S
CSRC += arch/v7m-faults.c

LIBPIKO_CSRC = $(wildcard libc/*.c)
LIBPIKO_SSRC = $(wildcard libc/*.S) $(wildcard libc/piko/*.S)

SSRC += $(LIBPIKO_SSRC)

CSRC += kernel/syscall.c
CSRC += \
    $(wildcard kernel/*.c) \
    $(wildcard kernel/fs/*.c) \
    $(wildcard kernel/mm/*.c) \
    $(wildcard kernel/sched/*.c) \
    $(wildcard fs/*.c) \
    $(wildcard drivers/char/*.c) \
    $(wildcard drivers/mtd/*.c) \
    $(wildcard drivers/timer/timercore.c) \
    $(wildcard drivers/serial/serial*.c) \
    $(wildcard user/*.c) \
    libc/piko/stubs.c \
    libc/piko/mman.c \
    $(LIBPIKO_CSRC)

OBJS += $(SSRC:.S=.o) $(CSRC:.c=.o)
OBJS := $(sort $(OBJS))

.PHONY: all check clean distclean

all: $(CMSIS)/$(TARGET) $(NAME).lds $(NAME).bin

# generic build rules
include mk/flags.mk
include mk/rules.mk

prebuild: $(CMSIS)/$(TARGET)

$(CMSIS):
	mkdir -p $@

check:
	python3 tests/runner.py

clean:
	find . -name "*.o" -type f -delete
ifneq "$(wildcard $(CMSIS) )" ""
	find $(CMSIS) -name "*.o" -type f -delete
endif
	rm -f $(NAME).map $(NAME).lds
	rm -f $(NAME).elf $(NAME).bin

distclean: clean
	rm -f kernel/syscall.c include/kernel/syscalls.h fs/version
	rm -rf $(CMSIS)

# platform build contains flashing and running rules
include target/$(TARGET)/build.mk

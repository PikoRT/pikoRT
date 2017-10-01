NAME = piko

# select QEMU when the platform is unspecified
PLAT ?= f429disco
CMSIS = external/cmsis

# The platform Makefile contains hw details and flags
include platform/$(PLAT)/Makefile

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

deps := $(OBJS:%.o=.%.o.d)

.PHONY: all check clean distclean

all: $(CMSIS)/$(PLAT) $(NAME).lds $(NAME).bin

# generic build rules
include mk/flags.mk
include mk/rules.mk
include mk/cmsis.mk

prebuild: $(CMSIS)/$(PLAT)

check:
	$(PYTHON) -m tests -p $(PLAT)

clean:
	find . -name "*.o" -type f -delete
	find . -name "*.o.d" -type f -delete
	rm -f $(deps)
ifneq "$(wildcard $(CMSIS) )" ""
	find $(CMSIS) -name "*.o" -type f -delete
endif
	rm -f $(NAME).map $(NAME).lds
	rm -f $(NAME).elf $(NAME).bin
# Remove GEN files
	rm -f include/kernel/syscalls.h
	rm -f kernel/syscall.c
	rm -f fs/version

distclean: clean
	rm -f kernel/syscall.c include/kernel/syscalls.h fs/version
	rm -rf $(CMSIS)

# platform build contains flashing and running rules
include platform/$(PLAT)/build.mk

-include $(deps)

include mk/opt.mk
include mk/flags.mk

NAME = piko

# select QEMU when the target is unspecified
TARGET ?= stm32f429
CMSIS = ../cmsis

# The platform Makefile contains hw details and flags
include target/$(TARGET)/build.mk

# arch-specific
SSRC += arch/v7m-head.S arch/v7m-entry.S arch/v7m-svcall.S
CSRC += arch/v7m-faults.c

LIBPIKO_CSRC = $(wildcard libc/*.c)
LIBPIKO_SSRC = $(wildcard libc/*.S) $(wildcard libc/piko/*.S)

SSRC += $(LIBPIKO_SSRC)

CSRC += kernel/syscall.c
CSRC += $(wildcard kernel/*.c)		\
	$(wildcard kernel/fs/*.c)	\
	$(wildcard kernel/mm/*.c)	\
	$(wildcard kernel/sched/*.c)	\
	$(wildcard drivers/char/*.c)	\
	$(wildcard drivers/mtd/*.c)	\
	$(wildcard drivers/timer/timercore.c) \
	$(wildcard drivers/serial/serial*.c) \
	$(wildcard user/*.c)		\
	libc/piko/stubs.c		\
	libc/piko/mman.c		\
	$(LIBPIKO_CSRC)			\

OBJS += $(SSRC:.S=.o) $(CSRC:.c=.o)
OBJS := $(sort $(OBJS))

.PHONY: all check clean distclean

all: $(CMSIS)/$(TARGET) $(NAME).lds $(NAME).hex $(NAME).bin

prebuild: $(CMSIS)/$(TARGET)

$(NAME).elf: $(OBJS) kernel/fs/version.o
	$(VECHO) "  LD\t\t$@\n"
	$(Q)$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(VECHO) "  CC\t\t$@\n"
	$(Q)$(CC) -o $@ $(CFLAGS) -c -W -Wall -std=c11 -D__KERNEL__ $<

%.o: %.S
	$(VECHO) "  AS\t\t$@\n"
	$(Q)$(CC) -o $@ $(CFLAGS) -c $<

%.lds: %.lds.S
	$(VECHO) "  HOSTCC\t$@\n"
	$(Q)$(HOSTCC) -E -P -Iinclude -DROMSZ=$(ROMSZ) -DRAMSZ=$(RAMSZ) -o $@ $<

include/cmsis/arm/core_cm4.h:
	git submodule init
	git submodule update

kernel/syscall.c: include/kernel/syscalls.h
	$(VECHO) "  GEN\t\t$@\n"
	$(Q)python scripts/gen-syscalls.py --source > $@

include/kernel/syscalls.h:
	$(VECHO) "  GEN\t\t$@\n"
	$(Q)python scripts/gen-syscalls.py --header > $@

kernel/fs/version:
	$(VECHO) "  GEN\t\t$@\n"
	$(Q)python3 scripts/gen-proc-version.py --cc-version    \
	--user $(shell whoami) --host $(shell hostname)		\
	-a $(ARCH) -c $(CPU) -n 'Piko' > $@

kernel/fs/version.o: kernel/fs/version
	$(VECHO) "  OBJCOPY\t$@\n"
	$(Q)$(OBJCOPY) -I binary -O elf32-littlearm -B arm		\
	--rename-section .data=.rodata					\
        --redefine-sym _binary_$(subst /,_,$<)_start=_version_ptr	\
        --redefine-sym _binary_$(subst /,_,$<)_size=_version_len	\
	$< $@

%.hex: %.elf
	$(VECHO) "  OBJCOPY\t$@\n"
	$(Q)$(OBJCOPY) -O ihex $< $@

%.bin: %.elf
	$(VECHO) "  OBJCOPY\t$@\n"
	$(Q)$(OBJCOPY) -Obinary $< $@

check:
	python3 tests/runner.py

clean:
	find . -name "*.o" -type f -delete
	find $(CMSIS) -name "*.o" -type f -delete
	rm -f $(NAME).map $(NAME).lds
	rm -f $(NAME).elf $(NAME).hex
	rm -f $(NAME).bin

distclean: clean
	rm -f kernel/syscall.c include/kernel/syscalls.h kernel/fs/version
	rm -rf $(CMSIS)

astyle:
	astyle --style=linux --indent=tab --indent-switches --suffix=none --recursive *.c *.h

# platform Makefile.rules contains flashing and running rules
include target/$(TARGET)/Makefile.rules

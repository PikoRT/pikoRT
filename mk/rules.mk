# Control the build verbosity
ifeq ("$(VERBOSE)","1")
    Q :=
    VECHO = @true
else
    Q := @
    VECHO = @printf
endif

$(NAME).elf: $(OBJS) fs/version.o
	$(VECHO) "  LD\t\t$@\n"
	$(Q)$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(VECHO) "  CC\t\t$@\n"
	$(Q)$(CC) -o $@ $(CFLAGS) -c -D__KERNEL__ -MMD -MF $@.d $<

%.o: %.S
	$(VECHO) "  AS\t\t$@\n"
	$(Q)$(CC) -o $@ $(CFLAGS) -c $<

%.lds: %.lds.S
	$(VECHO) "  HOSTCC\t$@\n"
	$(Q)$(HOSTCC) -E -P -Iinclude -DROMSZ=$(ROMSZ) -DRAMSZ=$(RAMSZ) -o $@ $<

kernel/syscall.c: include/kernel/syscalls.h
	$(VECHO) "  GEN\t\t$@\n"
	$(Q)$(PYTHON) scripts/gen-syscalls.py --source > $@

include/kernel/syscalls.h:
	$(VECHO) "  GEN\t\t$@\n"
	$(Q)$(PYTHON) scripts/gen-syscalls.py --header > $@

fs/version:
	$(VECHO) "  GEN\t\t$@\n"
	$(Q)python3 scripts/gen-proc-version.py --cc-version    \
	--user $(shell whoami) --host $(shell hostname)		\
	-a $(ARCH) -c $(CPU) -n 'Piko' > $@

fs/version.o: fs/version
	$(VECHO) "  OBJCOPY\t$@\n"
	$(Q)$(OBJCOPY) -I binary -O elf32-littlearm -B arm \
	    --rename-section .data=.rodata \
	    --redefine-sym _binary_$(subst /,_,$<)_start=_version_ptr \
	    --redefine-sym _binary_$(subst /,_,$<)_size=_version_len \
	    $< $@

%.bin: %.elf
	$(VECHO) "  OBJCOPY\t$@\n"
	$(Q)$(OBJCOPY) -Obinary $< $@

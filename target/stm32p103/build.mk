ifeq ($(shell lsb_release -c -s),trusty)
    REDIRECT_SERIAL = -serial stdio
endif

run: $(NAME).bin
	$(Q)qemu-system-arm \
	    -semihosting \
	    $(REDIRECT_SERIAL) \
	    -nographic \
	    -cpu cortex-m3 \
	    -machine stm32-p103	\
	    -kernel $<

dbg: $(NAME).bin
	$(Q)$(GDB) -q \
	    -semihosting \
	    $(REDIRECT_SERIAL) \
	    -nographic \
	    -cpu cortex-m3 \
	    -machine stm32-p103 \
	    -kernel $< \
	    -S -s

gdb: $(NAME).elf
	$(Q)$(GDB) -q \
	    $< -ex "target remote :1234"

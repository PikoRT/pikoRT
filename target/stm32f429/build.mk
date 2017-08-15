run: $(NAME).bin
	openocd -f interface/stlink-v2.cfg \
	    -f target/stm32f4x_stlink.cfg \
	    -c "init" \
	    -c "reset init" \
	    -c "reset halt"
	    -c "reset run"

st-flash: $(NAME).bin
	st-flash --reset write \
	    $(NAME).bin 0x08000000

dbg: $(NAME).bin
	openocd -f board/stm32f429discovery.cfg

gdb: $(NAME).elf
	$(Q)$(GDB) -q \
	    $< -ex "target remote :3333" \
	    -ex "monitor reset halt"

wakeup:
	openocd -f interface/stlink-v2.cfg \
	    -f target/stm32f4x_stlink.cfg \
	    -c "init" \
	    -c "reset init" \
	    -c "reset run" \
	    -c "shutdown"

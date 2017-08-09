ifeq ($(shell lsb_release -c -s),trusty)
  REDIRECT_SERIAL = -serial stdio
endif

$(CMSIS)/$(TARGET): $(CMSIS)/arm $(CMSIS)/TARGET_STM $(CMSIS)/util

$(CMSIS)/arm:
	svn export --force https://github.com/ARMmbed/mbed-os/trunk/cmsis/ $(CMSIS)/arm

$(CMSIS)/TARGET_STM:
	svn export --force https://github.com/ARMmbed/mbed-os/trunk/targets/TARGET_STM/ $(CMSIS)/TARGET_STM

$(CMSIS)/util:
	mkdir -p $(CMSIS)/util
	wget https://raw.github.com/ARMmbed/mbed-os/master/platform/mbed_preprocessor.h -P $(CMSIS)/util
	wget https://raw.github.com/ARMmbed/mbed-os/master/platform/mbed_assert.h -P $(CMSIS)/util

run: $(NAME).bin
	$(Q)qemu-system-arm \
	    -semihosting \
	    $(REDIRECT_SERIAL) \
	    -nographic \
	    -cpu cortex-m3 \
	    -machine stm32-p103 \
	    -kernel $<

st-flash: $(NAME).bin
	st-flash --reset write \
	    $(NAME).bin 0x08000000

dbg: $(NAME).bin
	openocd -f board/stm32f429discovery.cfg

gdb: $(NAME).elf
	$(Q)/usr/bin/arm-none-eabi-gdb \
	    $< -ex "target remote :3333" \
	    -ex "monitor reset halt"

wakeup:
	openocd -f interface/stlink-v2.cfg \
	    -f target/stm32f4x_stlink.cfg \
	    -c "init" \
	    -c "reset init" \
	    -c "reset run" \
	    -c "shutdown"

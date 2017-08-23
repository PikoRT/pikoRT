SVN_REV = 27441

$(CMSIS):
	@mkdir -p $@

$(CMSIS)/$(TARGET): $(CMSIS)/arm $(CMSIS)/TARGET_STM $(CMSIS)/util

$(CMSIS)/arm:
	svn export -r$(SVN_REV) --force https://github.com/ARMmbed/mbed-os/trunk/cmsis/ $(CMSIS)/arm

$(CMSIS)/TARGET_STM:
	svn export -r$(SVN_REV) --force https://github.com/ARMmbed/mbed-os/trunk/targets/TARGET_STM/ $(CMSIS)/TARGET_STM

ARM_CMSIS_UTIL = \
	'mbed_preprocessor.h' \
	'mbed_assert.h'

$(CMSIS)/util: $(ARM_CMSIS_UTIL)
$(ARM_CMSIS_UTIL):
	$(VECHO) "  WGET\t\t$@\n"
	$(Q)$(WGET) -q https://raw.github.com/ARMmbed/mbed-os/master/platform/$@ -P $(CMSIS)/util

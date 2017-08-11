$(CMSIS):
	@mkdir -p $@

$(CMSIS)/$(TARGET): $(CMSIS)/arm $(CMSIS)/TARGET_STM $(CMSIS)/util

$(CMSIS)/arm:
	svn export --force https://github.com/ARMmbed/mbed-os/trunk/cmsis/ $(CMSIS)/arm

$(CMSIS)/TARGET_STM:
	svn export --force https://github.com/ARMmbed/mbed-os/trunk/targets/TARGET_STM/ $(CMSIS)/TARGET_STM

$(CMSIS)/util:
	@mkdir -p $(CMSIS)/util
# Reference: https://stackoverflow.com/questions/1125476/retrieve-a-single-file-from-a-repository
#            https://stackoverflow.com/questions/1078524/how-to-specify-the-location-with-wget
	wget https://raw.github.com/ARMmbed/mbed-os/master/platform/mbed_preprocessor.h -P $(CMSIS)/util
	wget https://raw.github.com/ARMmbed/mbed-os/master/platform/mbed_assert.h -P $(CMSIS)/util

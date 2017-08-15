$(HEADER_BUILD)/qstr.i.last: $(SRC_QSTR) | $(HEADER_BUILD)/mpversion.h
	$(ECHO) "GEN $@"
	$(Q)$(CPP) $(QSTR_GEN_EXTRA_CFLAGS) $(CFLAGS) $(if $?,$?,$^) >$(HEADER_BUILD)/qstr.i.last;

$(HEADER_BUILD)/qstr.split: $(HEADER_BUILD)/qstr.i.last
	$(ECHO) "GEN $@"
	$(Q)$(PYTHON) $(PY_SRC)/makeqstrdefs.py split $(HEADER_BUILD)/qstr.i.last $(HEADER_BUILD)/qstr $(QSTR_DEFS_COLLECTED)
	$(Q)touch $@

$(QSTR_DEFS_COLLECTED): $(HEADER_BUILD)/qstr.split
	$(ECHO) "GEN $@"
	$(Q)$(PYTHON) $(PY_SRC)/makeqstrdefs.py cat $(HEADER_BUILD)/qstr.i.last $(HEADER_BUILD)/qstr $(QSTR_DEFS_COLLECTED)

$(BUILD)/_frozen_mpy.c: user/mpy/forzentest.mpy $(BUILD)/genhdr/qstrdefs.generated.h
	@echo "MISC freezing bytecode"
	user/mpy/tools/mpy-tool.py -f -q $(BUILD)/genhdr/qstrdefs.preprocessed.h -mlongint-impl=none $< > $@

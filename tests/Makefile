CSRC = $(wildcard tests/$(TEST)/*.c)
SSRC = $(wildcard tests/$(TEST)/*.S)

TEST_OBJ = $(wildcard tests/$(TEST)/*.o)
LIB_OBJ = $(wildcard tests/lib/*.o)

CFLAGS = -Itests/lib

# Build a romFS drive if there is a /data directory at the root
# of the testcase directory.
ifneq ("$(wildcard tests/$(TEST)/data)","")
  OBJS = tests/$(TEST)/sda1.o
endif

include Makefile

%sda1:
	$(VECHO) "GENROMFS $@"
	$(Q)genromfs -f tests/$(TEST)/sda1 -d tests/$(TEST)/data -V sda1

%sda1.o: %sda1
	$(VECHO) "BUILDFS\t$@"
	$(Q)$(OBJCOPY) -I binary -O elf32-littlearm -B arm \
	    --rename-section .data=.rodata \
	    --redefine-sym _binary_$(subst /,_,$<)_start=_binary_sda1_start \
	    --redefine-sym _binary_$(subst /,_,$<)_end=_binary_sda1_end \
	    --redefine-sym _binary_$(subst /,_,$<)_size=_binary_sda1_size \
	    tests/$(TEST)/sda1 tests/$(TEST)/sda1.o

clean:
	rm -f $(TEST_OBJ) $(LIB_OBJ) tests/$(TEST)/sda1 tests/$(TEST)/sda1.o

clean_test:
	rm -f $(NAME).elf $(TEST_OBJ) tests/$(TEST)/sda1 tests/$(TEST)/sda1.o

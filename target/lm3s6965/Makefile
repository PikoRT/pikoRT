CPU	= cortex-m3
ARCH	= armv7-m

ROMSZ	= 256k
RAMSZ	= 64k

CFLAGS	+= -Itarget/lm3s6965

CSRC	+=				\
	target/lm3s6965/halt.c		\
	target/lm3s6965/init.c	\

# CMSIS files
CSRC	+= ../cmsis/lm3s6965/system_lm3s.c

# Timer driver files
CSRC	+= drivers/timer/systick.c

# Serial driver files
CSRC	+= drivers/serial/lm3s6965.c

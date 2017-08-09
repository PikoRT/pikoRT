# Piko/RT

This is `Piko/RT`, a tiny Linux-like real-time operating system kernel, optimized for ARM Cortex-M series microprocessors.


Run STM32-p103 emulator
-----------------------

You will need to prepare QEMU STM32 emulator from: [QEMU STM32 v0.1.3](https://github.com/beckus/qemu_stm32/releases)

After download, compile the qemu_stm32 by:

```
$ ./configure --disable-werror --enable-debug \
        --target-list="arm-softmmu" \
        --extra-cflags=-DDEBUG_CLKTREE \
        --extra-cflags=-DDEBUG_STM32_RCC \
        --extra-cflags=-DDEBUG_STM32_UART \
        --extra-cflags=-DSTM32_UART_NO_BAUD_DELAY \
        --extra-cflags=-DSTM32_UART_ENABLE_OVERRUN --python=python2
$ make -j8
```

Make sure you have export the `arm-softmmu` to `$PATH`

```
$ export PATH=$PATH:~/qemu_stm32-stm32_v0.1.3/arm-softmmu
```

You will need to modify the target from `stm32f429` to
`stm32p103` at `Makefile`:

```
include Makefile.opt
include Makefile.inc

NAME = piko

# select QEMU when the target is unspecified
TARGET ?= stm32p103
CMSIS = ../cmsis

# The platform Makefile contains hw details and flags
include target/$(TARGET)/Makefile
```

Then make it:

```
$ make prebuild
$ make -j8
$ make run
```

If your setting is correct, you will see:

```
$ make run
(process:11889): GLib-WARNING **: gmem.c:483: custom memory allocation vtable not supported
STM32_UART: UART1 clock is set to 0 Hz.
STM32_UART: UART1 BRR set to 0.
STM32_UART: UART1 Baud is set to 0 bits per sec.
STM32_UART: UART2 clock is set to 0 Hz.
STM32_UART: UART2 BRR set to 0.
STM32_UART: UART2 Baud is set to 0 bits per sec.
STM32_UART: UART3 clock is set to 0 Hz.
STM32_UART: UART3 BRR set to 0.
STM32_UART: UART3 Baud is set to 0 bits per sec.
STM32_UART: UART4 clock is set to 0 Hz.
STM32_UART: UART4 BRR set to 0.
STM32_UART: UART4 Baud is set to 0 bits per sec.
STM32_UART: UART5 clock is set to 0 Hz.
STM32_UART: UART5 BRR set to 0.
STM32_UART: UART5 Baud is set to 0 bits per sec.
STM32_UART: UART5 clock is set to 0 Hz.
STM32_UART: UART5 BRR set to 0.
STM32_UART: UART5 Baud is set to 0 bits per sec.
STM32_UART: UART4 clock is set to 0 Hz.
STM32_UART: UART4 BRR set to 0.
STM32_UART: UART4 Baud is set to 0 bits per sec.
STM32_UART: UART3 clock is set to 0 Hz.
STM32_UART: UART3 BRR set to 0.
STM32_UART: UART3 Baud is set to 0 bits per sec.
STM32_UART: UART2 clock is set to 0 Hz.
STM32_UART: UART2 BRR set to 0.
STM32_UART: UART2 Baud is set to 0 bits per sec.
STM32_UART: UART1 clock is set to 0 Hz.
STM32_UART: UART1 BRR set to 0.
STM32_UART: UART1 Baud is set to 0 bits per sec.
LED Off
CLKTREE: HSI Output Change (SrcClk:None InFreq:8000000 OutFreq:8000000 Mul:1 Div:1 Enabled:1)
CLKTREE: HSI/2 Output Change (SrcClk:HSI InFreq:8000000 OutFreq:4000000 Mul:1 Div:2 Enabled:1)
CLKTREE: SYSCLK Output Change (SrcClk:HSI InFreq:8000000 OutFreq:8000000 Mul:1 Div:1 Enabled:1)
CLKTREE: HCLK Output Change (SrcClk:SYSCLK InFreq:8000000 OutFreq:8000000 Mul:1 Div:1 Enabled:1)
STM32_RCC: Cortex SYSTICK frequency set to 8000000 Hz (scale set to 125).
STM32_RCC: Cortex SYSTICK ext ref frequency set to 1000000 Hz (scale set to 1000).
CLKTREE: PCLK1 Output Change (SrcClk:HCLK InFreq:8000000 OutFreq:8000000 Mul:1 Div:1 Enabled:1)
CLKTREE: PCLK2 Output Change (SrcClk:HCLK InFreq:8000000 OutFreq:8000000 Mul:1 Div:1 Enabled:1)
CLKTREE: GPIOA Output Change (SrcClk:PCLK2 InFreq:8000000 OutFreq:8000000 Mul:1 Div:1 Enabled:1)
CLKTREE: AFIO Output Change (SrcClk:PCLK2 InFreq:8000000 OutFreq:8000000 Mul:1 Div:1 Enabled:1)
CLKTREE: UART2 Output Change (SrcClk:PCLK1 InFreq:8000000 OutFreq:8000000 Mul:1 Div:1 Enabled:1)
STM32_UART: UART2 clock is set to 8000000 Hz.
STM32_UART: UART2 BRR set to 0.
STM32_UART: UART2 Baud is set to 0 bits per sec.
HelloMemory map:
  .text   = 00000140--000031ba   12410 Bytes
  .rodata = 000031ec--00003ab2    2246 Bytes
  .data   = 20000000--20000514    1300 Bytes
  .bss    = 20000518--20001298    3456 Bytes
  .heap   = 200012a0--200022a0    4096 Bytes
  .pgmem  = 20008000--2000f000   28672 Bytes
Order  Bitmap
    0  00000000  00000000  00000000  00000000
    1  00000000  00000000
    2  00000000
    3  00007fff
Created idle_thread at <0x20008200>
Created main_thread at <0x20008800> with priority=31
Reclaim early stack's physical memory (2048 Bytes, order=3).
Creating /proc/version
Creating /proc/meminfo
Creating /dev/mem
Creating /dev/null
Creating /dev/zero
Creating /dev/random
Creating MTD device mtd0
Kernel bootstrap done.
--
Piko version 0.0.0 (uscc@uscc-Inspiron-7559) (armv7-m, cortex-m3) #Sun Apr 30 17:58:03 2017
arm-none-eabi-gcc (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)

$
```

There is your minishell!

# Piko/RT

This is `Piko/RT`, a tiny Linux-like real-time operating system kernel, optimized for ARM Cortex-M series microprocessors.


## Run test suite

```
# Run all test
$ make PLAT=stm32p103 check

# Run all test with command line tools
$ python -m tests

# Run partial test with command line tools
$ python -m tests fs_1 cond_2
```

## Makefile

You can switch scheduler by makefile flags:

```
# Using bitmap scheduler

$ make SCHED=BITMAP

# Using RR scheduler

$ make SCHED=RR
```

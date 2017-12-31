# Piko/RT

This is `Piko/RT`, a tiny Linux-like real-time operating system kernel, optimized for ARM Cortex-M series microprocessors.


## Run test suite

* Run all test
```shell
$ make PLAT=stm32p103 check
```

* Run all test with command line tools
```shell
$ python -m tests
```

* Run specific test cases
```shell
$ python -m tests fs_1 cond_2
```

## External Source

* `scripts/rstlint.py`: written by Georg Brandl

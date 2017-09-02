.. _test:


Piko/RT Test Suite
==================

Piko/RT test suite is under ``tests`` directory, it contain a makefile, many test
folder and it self is a python package. Each test folder represent a test case.


Run Tests
---------

Test suite can be run by top directory Makefile::

   $ make PLAT=stm32p103 check

This will run all test in the test suite, also, you can use ``tests`` as a python
package, to run all test via python command line::

   $ python -m tests

You can also run partial via python::

   $ python -m tests fs_1 stat_1 thread_5

Python command line tool verbose default is disable, you can pass the flag ``-v``
to enable::

   $ python -m tests -v
   $ python -m tests fs_1 -v

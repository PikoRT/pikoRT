.. _build:

Build Piko/RT
=============

This chapter will help you start to build ``Piko/RT``, you will need to prepare
some pacakge and settings.


Prerequisites
-------------

* svn: used to checkout `CMSIS <https://github.com/ARMmbed/mbed-os/cmsis>`_.
* wget: used to download required files.
* arm-none-eabi-gcc arm-none-eabi-newlib: cross-compile toolchain.

Arch Linux::

   $ pacman -S base-devel svn lsb-release wget
   $ pacman -S arm-none-eabi-gcc arm-none-eabi-newlib
   $ pacman -S python

Ubuntu::

   $ add-apt-repository -y ppa:team-gcc-arm-embedded/ppa
   $ apt update -qq
   $ apt install build-essential svn gcc-arm-embedded

macOS::

   # If you didn't build any thing on macOS before
   # please install xcode-select first
   $ xcode-select --install

   # install brew from https://brew.sh
   $ brew install subversion wget
   $ brew tap PX4/homebrew-px4
   $ brew install gcc-arm-none-eabi-49


Build Piko/RT on Linux and macOS
--------------------------------

Default make will build for ``f429disco``::

   $ make

You can change the target via ``PLAT``::

   $ make PLAT=stm32p103

You can build faster via passing ``-j`` flag::

   $ make PLAT=stm32p103 -j8

You can build with verbose output::

   $ make PLAT=stm32p103 -j8 VERBOSE=1


Troubleshooting
---------------

If you concur problem when building Piko/RT, please help to fill the issue
on `GitHub <https://github.com/piko-rt/pikoRT/issues>`_,
and maybe help to improve the build process, or improve this troubleshooting
part!

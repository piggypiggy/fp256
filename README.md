[![linux-x64](https://github.com/piggypiggy/fp256/actions/workflows/linux-x64.yml/badge.svg?branch=master)](https://github.com/piggypiggy/fp256/actions/workflows/linux-x64.yml)
[![macos-x64](https://github.com/piggypiggy/fp256/actions/workflows/macos-x64.yml/badge.svg?branch=master)](https://github.com/piggypiggy/fp256/actions/workflows/macos-x64.yml)

[![windows-x64-vs2017](https://github.com/piggypiggy/fp256/actions/workflows/windows-x64-vs2017.yml/badge.svg?branch=master)](https://github.com/piggypiggy/fp256/actions/workflows/windows-x64-vs2017.yml)
[![windows-x64-vs2019](https://github.com/piggypiggy/fp256/actions/workflows/windows-x64-vs2019.yml/badge.svg?branch=master)](https://github.com/piggypiggy/fp256/actions/workflows/windows-x64-vs2019.yml)

[![qemu-aarch64](https://github.com/piggypiggy/fp256/actions/workflows/qemu-aarch64.yml/badge.svg?branch=master)](https://github.com/piggypiggy/fp256/actions/workflows/qemu-aarch64.yml)
[![qemu-riscv64](https://github.com/piggypiggy/fp256/actions/workflows/qemu-riscv64.yml/badge.svg?branch=master)](https://github.com/piggypiggy/fp256/actions/workflows/qemu-riscv64.yml)

# introduction
fp256 is an efficient library implemented in C and assembly for 256 bit integer arithmetic.  

It supports Windows, Linux and Macosx on x86_64 and arm64 processor, and can be compiled with MSVC, mingw-w64, gcc and clang.  

# implementation
* Use 4×64bit to represent 256 bit integer.

* Leveled api :  
low level api operates directly on integer array, most low level api are specific for 256 bit integer, there are also a few api for arbitrary length array.  
high level api operates on 256 bit integer.

* Most low level arithmetic are implemented in assembly, including `add`, `mul`, `u256 mul`, `div`, `shift`, `exgcd`, `mont mul`, etc, they are very efficient and (maybe)well commented, see [ll/x64](https://github.com/piggypiggy/fp256/tree/master/src/ll/x64), [ll/aarch64](https://github.com/piggypiggy/fp256/tree/master/src/ll/aarch64) and [ll/riscv64](https://github.com/piggypiggy/fp256/tree/master/src/ll/riscv64).

* 

# compilation
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make -j
    $ make test

Or cross compilation (cmake toolchain files are [here](https://github.com/piggypiggy/fp256/tree/master/cmake/toolchain)):

    $ mkdir build
    $ cd build
    $ cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain/aarch64-linux-gcc.cmake ..
    $ make -j

### configuration
`-DCMAKE_BUILD_TYPE` : possible values are empty, Debug, Release, RelWithDebInfo and MinSizeRel, default is `Release`.  
`-DCMAKE_INSTALL_PREFIX` : where to install fp256 library, default is `/usr/local`.  
`-DBUILD_STATIC` : build static library, default is `ON`.  
`-DBUILD_SHARED` : build shared library, default is `ON`.  
`-DUSE_DEV_RANDOM` : use /dev/random to get random number, default is `OFF`(use /dev/urandom).  
`-DUSE_ASM` : use assembly code, default is `ON`. it can not be turned off since some algorithms have no C implementation to fall back yet.  
`-DUSE_ASAN` : use AddressSanitizer, default is `OFF`.  

# TODO
* Add prime test, modular exponential, square root related, etc.
* Constant-time implementation of division and exgcd.
* more tests.

# benchmark
Some of benchmark results are listed below. Build and run build/bench/bench to get full result.
### AMD Ryzen 5 4600U 2.1GHz, Ubuntu 20.04 LTS, gcc-9.3.0
arithmetic(256 bit)     |      cycles / op      |      op / s      |
------------------------|-----------------------|------------------|
naive div               |           428         |     4890000      |
gcd                     |          3784         |      554000      |
mod add                 |           330         |     6340000      |
mod sqr                 |           491         |     4260000      |
mod mul                 |           509         |     4110000      |
mod inv                 |          4628         |      453000      |
mod exp                 |         17000         |      123000      |
mont mul                |           130         |    16000000      |
mont sqr                |           117         |    17800000      |

# license
Apache 2.0

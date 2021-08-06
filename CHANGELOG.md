## v0.6.0
- implement u256mulhi, u256mullo, u256sqrhi, u256sqrlo, mont_reduce, etc in c, x64, aarch64 and riscv64.
- implement add, shift, div, lehmer exgcd in c and riscv64.
- all riscv64 tests, more test vectors.
- improve build on visual studio.
- remove unused functions(fmod).

## v0.5.0
- implement faster division algorithm(ll_div) in x64 and aarch64 assembly.
- implement modular exponentiation(ll_u256_mont_exp) for odd modulus.
- implement conditional select(ll_u256_select).
- implement montgomery multiplication for arbitrary integer size(mainly for size about 256 bit).
- implement shift in aarch64 assembly.
- add modular exponentiation benchmark.
- add test for shift, mont mul and u256 sub.
- add benchmark code for riscv64.
- improve test vectors.
- migrate ci to github actions, add riscv64 build.
- improve cmake scripts.
- implement ll_u256_add and ll_u256_sub in riscv64 assembly.
- drop support for negative 256 bit integer.

## v0.4.0
- use doxygen to generate document.
- fix bugs in ll_add-aarch64.pl, ll_mul-aarch64.pl, ll_mulsub_limb-aarch64.pl etc.
- implement faster division algorithm in C.
- add arm64 build in travis-ci.
- implement lehmer exgcd algorithm, montgomery multiplication in aarch64 assembly.
- better argument parsing for test.

## v0.3.0
- implementing low level routines in aarch64 assembly.
- add C implementation of some low level routines.
- add test and bench for some low level routines.
- fix bugs in ll_add, ll_sub and ll_rshift.
- ll_add-x64.pl, ll_mul-x64.pl: simpler code without counting nlimbs.
- use AddressSanitizer and MemorySanitizer in travis-ci build.

## v0.2.0
- implement montgomery multiplication(assembly).

## v0.1.0
- Init.
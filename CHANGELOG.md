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
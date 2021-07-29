# it doesn't 'generate' anything...
if(USE_ASM)
    set(USE_ASM_ADD        1)    # ll_add-riscv64.S
    # set(USE_ASM_CPUID      1)    # ll_cpuid-x86.S
    set(USE_ASM_DIV        1)    # ll_div-riscv64.S
    # set(USE_ASM_LEHMER     1)    # ll_lehmer-riscv64.S
    set(USE_ASM_MUL        1)    # ll_mul-riscv64.S
    # set(USE_ASM_SHIFT      1)    # ll_shift-riscv64.S
    set(USE_ASM_U256_ADD   1)    # ll_u256_add-riscv64.S
    set(USE_ASM_U256_MONT  1)    # ll_u256_mont-riscv64.S
    set(USE_ASM_U256_MUL   1)    # ll_u256_mul-riscv64.S
    set(USE_ASM_U256_SHIFT 1)    # ll_u256_shift-riscv64.S
    # set(USE_ASM_UTILS      1)    # ll_utils-riscv64.S

    set(LL_ASM_DIR ${LL_DIR}/riscv64)
    enable_language(ASM)

    macro(ll_add_asm asm_src use_it)
        if(${use_it})
            # add each asm file to LL_ASM_SRC
            list(APPEND LL_ASM_SRC ${asm_src})
        endif()
    endmacro()

    ll_add_asm(${LL_ASM_DIR}/ll_add-riscv64.S         USE_ASM_ADD)
    ll_add_asm(${LL_ASM_DIR}/ll_div-riscv64.S         USE_ASM_DIV)
    ll_add_asm(${LL_ASM_DIR}/ll_mul-riscv64.S         USE_ASM_MUL)
    # ll_add_asm(${LL_ASM_DIR}/ll_shift-riscv64.S       USE_ASM_SHIFT)
    # ll_add_asm(${LL_ASM_DIR}/ll_lehmer-riscv64.S      USE_ASM_LEHMER)
    ll_add_asm(${LL_ASM_DIR}/ll_u256_add-riscv64.S    USE_ASM_U256_ADD)
    ll_add_asm(${LL_ASM_DIR}/ll_u256_mont-riscv64.S   USE_ASM_U256_MONT)
    ll_add_asm(${LL_ASM_DIR}/ll_u256_mul-riscv64.S    USE_ASM_U256_MUL)
    ll_add_asm(${LL_ASM_DIR}/ll_u256_shift-riscv64.S  USE_ASM_U256_SHIFT)
    # ll_add_asm(${LL_ASM_DIR}/ll_utils-riscv64.S       USE_ASM_UTILS)
endif()

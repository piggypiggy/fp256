if(USE_ASM)
    set(USE_ASM_ADD        1)    # ll_add-aarch64.pl
    # set(USE_ASM_CPUID      1)    # ll_cpuid-x86.pl
    set(USE_ASM_DIV        1)    # ll_div-aarch64.pl
    set(USE_ASM_LEHMER     1)    # ll_lehmer-aarch64.pl
    set(USE_ASM_MUL        1)    # ll_mul-aarch64.pl
    set(USE_ASM_SHIFT      1)    # ll_shift-aarch64.pl
    set(USE_ASM_U256_ADD   1)    # ll_u256_add-aarch64.pl
    set(USE_ASM_U256_MONT  1)    # ll_u256_mont-aarch64.pl
    set(USE_ASM_U256_MUL   1)    # ll_u256_mul-aarch64.pl
    set(USE_ASM_U256_SHIFT 1)    # ll_u256_shift-aarch64.pl
    set(USE_ASM_UTILS      1)    # ll_utils-aarch64.pl

    set(LL_ASM_DIR ${LL_DIR}/aarch64)
    enable_language(ASM)

    if(IOS)
        set(asm_flavour ios)
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(asm_flavour linux)
    endif()

    macro(ll_add_asm perl_src use_it)
        if(${use_it})
            get_filename_component(perl_name ${perl_src} NAME)
            set(asm_src ${LL_DIR}/${perl_name})
            string(REPLACE ".pl" ".S" asm_src "${asm_src}")

            # generate asm file
            add_custom_command(
                OUTPUT ${asm_src}
                COMMAND ${PERL} ${perl_src} ${asm_flavour} ${asm_src}
                DEPENDS ${perl_src}
            )

            # add each asm file to LL_ASM_SRC
            list(APPEND LL_ASM_SRC ${asm_src})
        endif()
    endmacro()

    ll_add_asm(${LL_ASM_DIR}/ll_add-aarch64.pl         USE_ASM_ADD)
    ll_add_asm(${LL_ASM_DIR}/ll_div-aarch64.pl         USE_ASM_DIV)
    ll_add_asm(${LL_ASM_DIR}/ll_mul-aarch64.pl         USE_ASM_MUL)
    ll_add_asm(${LL_ASM_DIR}/ll_shift-aarch64.pl       USE_ASM_SHIFT)
    ll_add_asm(${LL_ASM_DIR}/ll_lehmer-aarch64.pl      USE_ASM_LEHMER)
    ll_add_asm(${LL_ASM_DIR}/ll_u256_add-aarch64.pl    USE_ASM_U256_ADD)
    ll_add_asm(${LL_ASM_DIR}/ll_u256_mont-aarch64.pl   USE_ASM_U256_MONT)
    ll_add_asm(${LL_ASM_DIR}/ll_u256_mul-aarch64.pl    USE_ASM_U256_MUL)
    ll_add_asm(${LL_ASM_DIR}/ll_u256_shift-aarch64.pl  USE_ASM_U256_SHIFT)
    ll_add_asm(${LL_ASM_DIR}/ll_utils-aarch64.pl       USE_ASM_UTILS)
endif()

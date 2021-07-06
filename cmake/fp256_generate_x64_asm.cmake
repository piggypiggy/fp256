if(USE_ASM)
    set(USE_ASM_ADD        1)    # ll_add-x64.pl
    set(USE_ASM_CPUID      1)    # ll_cpuid-x86.pl
    set(USE_ASM_DIV        1)    # ll_div-x64.pl
    set(USE_ASM_LEHMER     1)    # ll_lehmer-x64.pl
    set(USE_ASM_MUL        1)    # ll_mul-x64.pl
    set(USE_ASM_SHIFT      1)    # ll_shift-x64.pl
    set(USE_ASM_U256_ADD   1)    # ll_u256_add-x64.pl
    set(USE_ASM_U256_DIV   1)    # ll_u256_div-x64.pl
    set(USE_ASM_U256_MONT  1)    # ll_u256_mont-x64.pl
    set(USE_ASM_U256_MUL   1)    # ll_u256_mul-x64.pl
    set(USE_ASM_U256_SHIFT 1)    # ll_u256_shift-x64.pl
    set(USE_ASM_UTILS      1)    # ll_utils-x64.pl

    set(LL_ASM_DIR ${LL_DIR}/x64)

    # choose assembler
    # asm_flavour, ENV{CC} and ENV{ASM} are used in x86_64-xlate.pl to generate ...
    if(USE_NASM)
        enable_language(ASM_NASM)
        set(asm_flavour nasm)
        set(ENV_FOR_PERL
            ${CMAKE_COMMAND} -E env "CC=${CMAKE_C_COMPILER}"
            ${CMAKE_COMMAND} -E env "ASM=${CMAKE_ASM_NASM_COMPILER}"
        )
    elseif(USE_MASM)
        enable_language(ASM_MASM)
        set(asm_flavour masm)
        set(ENV_FOR_PERL
            ${CMAKE_COMMAND} -E env "CC=${CMAKE_C_COMPILER}"
            ${CMAKE_COMMAND} -E env "ASM=${CMAKE_ASM_MASM_COMPILER}"
        )
    else()
        enable_language(ASM)
        if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
            set(asm_flavour macosx)
        elseif(MINGW)
            set(asm_flavour mingw64)
        else()
            set(asm_flavour elf)
        endif()
        set(ENV_FOR_PERL
            ${CMAKE_COMMAND} -E env "CC=${CMAKE_C_COMPILER}"
            ${CMAKE_COMMAND} -E env "ASM=${CMAKE_ASM_COMPILER}"
        )
    endif()

    macro(ll_add_asm perl_src use_it)
        if(${use_it})
            # ll/x64/z.pl -> ll/z.s or ll/z.asm
            get_filename_component(perl_name ${perl_src} NAME)
            set(asm_src ${LL_DIR}/${perl_name})

            # nasm or masm
            if (CMAKE_ASM_NASM_COMPILER_ID STREQUAL "NASM" OR 
                CMAKE_ASM_MASM_COMPILER_ID STREQUAL "MSVC")
                string(REPLACE ".pl" ".asm" asm_src "${asm_src}")
            else()
                string(REPLACE ".pl" ".s" asm_src "${asm_src}")
            endif()

            # generate asm file
            add_custom_command(
                OUTPUT ${asm_src}
                COMMAND ${ENV_FOR_PERL} ${PERL} ${perl_src} ${asm_flavour} ${asm_src}
                DEPENDS ${perl_src}
            )

            # add each asm file to LL_ASM_SRC
            list(APPEND LL_ASM_SRC ${asm_src})
        endif()
    endmacro()

    ll_add_asm(${LL_ASM_DIR}/ll_add-x64.pl        USE_ASM_ADD)
    ll_add_asm(${LL_ASM_DIR}/ll_cpuid-x86.pl      USE_ASM_CPUID)
    ll_add_asm(${LL_ASM_DIR}/ll_div-x64.pl        USE_ASM_DIV)
    ll_add_asm(${LL_ASM_DIR}/ll_lehmer-x64.pl     USE_ASM_LEHMER)
    ll_add_asm(${LL_ASM_DIR}/ll_mul-x64.pl        USE_ASM_MUL)
    ll_add_asm(${LL_ASM_DIR}/ll_shift-x64.pl      USE_ASM_SHIFT)
    ll_add_asm(${LL_ASM_DIR}/ll_u256_add-x64.pl   USE_ASM_U256_ADD)
    ll_add_asm(${LL_ASM_DIR}/ll_u256_div-x64.pl   USE_ASM_U256_DIV)
    ll_add_asm(${LL_ASM_DIR}/ll_u256_mont-x64.pl  USE_ASM_U256_MONT)
    ll_add_asm(${LL_ASM_DIR}/ll_u256_mul-x64.pl   USE_ASM_U256_MUL)
    ll_add_asm(${LL_ASM_DIR}/ll_u256_shift-x64.pl USE_ASM_U256_SHIFT)
    ll_add_asm(${LL_ASM_DIR}/ll_utils-x64.pl      USE_ASM_UTILS)
endif()

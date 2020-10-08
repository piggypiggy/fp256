cmake_minimum_required(VERSION 3.1.0)

set(ENV_FOR_PERL
    ${CMAKE_COMMAND} -E env "CC=${CMAKE_C_COMPILER}" 
    ${CMAKE_COMMAND} -E env "ASM=${CMAKE_ASM_COMPILER}"
)

set(LL_DIR ${FP256_SRC_DIR}/ll)

if(ARCH_X86_64 AND USE_ASM)
    # x64 assembly directory
    set(LL_ASM_DIR ${LL_DIR}/x64)

    macro(ll_add_asm perl_src)
        # ll/x64/z.pl -> ll/z.s or ll/z.asm
        get_filename_component(perl_name ${perl_src} NAME)
        set(asm_src ${LL_DIR}/${perl_name})
        if (COMPILER_MSVC)
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
    endmacro()

    ll_add_asm(${LL_ASM_DIR}/ll_add-x64.pl)
    ll_add_asm(${LL_ASM_DIR}/ll_cpuid-x86.pl)
    ll_add_asm(${LL_ASM_DIR}/ll_lehmer-x64.pl)
    ll_add_asm(${LL_ASM_DIR}/ll_mul-x64.pl)
    ll_add_asm(${LL_ASM_DIR}/ll_shift-x64.pl)
    ll_add_asm(${LL_ASM_DIR}/ll_utils-x64.pl)
    ll_add_asm(${LL_ASM_DIR}/ll_u256_add-x64.pl)
    ll_add_asm(${LL_ASM_DIR}/ll_u256_div-x64.pl)
    ll_add_asm(${LL_ASM_DIR}/ll_u256_fmod-x64.pl)
    # ll_add_asm(${LL_ASM_DIR}/ll_u256_mont-x64.pl)
    ll_add_asm(${LL_ASM_DIR}/ll_u256_mul-x64.pl)
    ll_add_asm(${LL_ASM_DIR}/ll_u256_shift-x64.pl)
endif()

# source file
set(FP256_HEADER
    ${FP256_DIR}/config.h
    ${FP256_INCLUDE_DIR}/fp256/fp256.h
    ${FP256_INCLUDE_DIR}/fp256/fp256_ll.h
    ${FP256_SRC_DIR}/fp256_local.h
)

set(FP256_SRC
    ${FP256_HEADER}
    ${LL_ASM_SRC}
    ${LL_DIR}/ll_add.c
    ${LL_DIR}/ll_convert.c
    ${LL_DIR}/ll_cpuid.c
    ${LL_DIR}/ll_div.c
    ${LL_DIR}/ll_lehmer.c
    ${LL_DIR}/ll_mul.c
    ${LL_DIR}/ll_print.c
    ${LL_DIR}/ll_rand.c
    ${LL_DIR}/ll_shift.c
    ${LL_DIR}/ll_sys_rand.c
    ${LL_DIR}/ll_u256_add.c
    ${LL_DIR}/ll_u256_fmod.c
    ${LL_DIR}/ll_u256_mul.c
    ${LL_DIR}/ll_u256_shift.c
    ${LL_DIR}/ll_utils.c
    ${FP256_SRC_DIR}/cpuid.c
    ${FP256_SRC_DIR}/fp256_add.c
    ${FP256_SRC_DIR}/fp256_cmp.c
    ${FP256_SRC_DIR}/fp256_naive_div.c
    ${FP256_SRC_DIR}/fp256_gcd.c
    ${FP256_SRC_DIR}/fp256_init.c
    ${FP256_SRC_DIR}/fp256_mod.c
    ${FP256_SRC_DIR}/fp256_mont.c
    ${FP256_SRC_DIR}/fp256_mul.c
    ${FP256_SRC_DIR}/fp256_print.c
    ${FP256_SRC_DIR}/fp256_rand.c
    ${FP256_SRC_DIR}/fp256_shift.c
    ${FP256_SRC_DIR}/fp256_utils.c
)

# header file directory
include_directories(${FP256_DIR}/)
include_directories(${FP256_INCLUDE_DIR}/)

if(BUILD_STATIC)
    # build static object
    add_library(fp256_static_object OBJECT ${FP256_SRC})
    target_compile_definitions(fp256_static_object PRIVATE BUILD_STATIC)
    #
    add_library(fp256_static STATIC $<TARGET_OBJECTS:fp256_static_object>)
    target_compile_definitions(fp256_static PRIVATE BUILD_STATIC)
    # static library output directory
    set_target_properties(fp256_static PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY_STATIC})
    # rename
    set_target_properties(fp256_static PROPERTIES OUTPUT_NAME "fp256")
    # install library
    INSTALL(TARGETS fp256_static ARCHIVE DESTINATION lib)
endif()

if(BUILD_SHARED)
    # build shared object
    add_library(fp256_shared_object OBJECT ${FP256_SRC})
    target_compile_definitions(fp256_shared_object PRIVATE BUILD_SHARED)
    # PIC
    set_property(TARGET fp256_shared_object PROPERTY POSITION_INDEPENDENT_CODE 1)

    #
    add_library(fp256_shared SHARED $<TARGET_OBJECTS:fp256_shared_object>)
    target_compile_definitions(fp256_shared PRIVATE BUILD_SHARED)
    # shared library output directory
    set_target_properties(fp256_shared PROPERTIES
        # msvc
        RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY_SHARED}
        ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY_SHARED}

        LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY_SHARED}
        VERSION ${FP256_VERSION}
        SOVERSION ${FP256_VERSION_MAJOR})
    # rename
    set_target_properties(fp256_shared PROPERTIES OUTPUT_NAME "fp256")
    # install library
    INSTALL(TARGETS fp256_shared LIBRARY DESTINATION lib)
endif()

# install headers
INSTALL(DIRECTORY ${FP256_INCLUDE_DIR}/fp256 DESTINATION include)
INSTALL(FILES ${FP256_DIR}/config.h DESTINATION include/fp256)


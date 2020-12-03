cmake_minimum_required(VERSION 3.1.0)

set(LL_DIR ${FP256_SRC_DIR}/ll)

# for arm build...
if(ARCH_AARCH64)
    set(USE_ASM_ADD        1)
    # set(USE_ASM_CPUID      1)
    set(USE_ASM_LEHMER     1)
    set(USE_ASM_MUL        1)
    # set(USE_ASM_SHIFT      1)
    set(USE_ASM_U256_ADD   1)
    # set(USE_ASM_U256_DIV   1)
    # set(USE_ASM_U256_FMOD  1)
    set(USE_ASM_U256_MONT  1)
    set(USE_ASM_U256_MUL   1)
    set(USE_ASM_U256_SHIFT 1)
    set(USE_ASM_UTILS      1)
else()
    set(USE_ASM_ADD        1)
    set(USE_ASM_CPUID      1)
    set(USE_ASM_LEHMER     1)
    set(USE_ASM_MUL        1)
    set(USE_ASM_SHIFT      1)
    set(USE_ASM_U256_ADD   1)
    set(USE_ASM_U256_DIV   1)
    set(USE_ASM_U256_FMOD  1)
    set(USE_ASM_U256_MONT  1)
    set(USE_ASM_U256_MUL   1)
    set(USE_ASM_U256_SHIFT 1)
    set(USE_ASM_UTILS      1)
endif()

if(USE_ASM)
    if(ARCH_X86_64)
        set(LL_ASM_DIR ${LL_DIR}/x64)
        set(ENV_FOR_PERL
            ${CMAKE_COMMAND} -E env "CC=${CMAKE_C_COMPILER}"
            ${CMAKE_COMMAND} -E env "ASM=${CMAKE_ASM_COMPILER}"
        )
    elseif(ARCH_AARCH64)
        set(LL_ASM_DIR ${LL_DIR}/aarch64)
    endif()

    macro(ll_add_asm perl_src use_it)
        if(${use_it})
            # ll/x64/z.pl -> ll/z.s or ll/z.asm
            get_filename_component(perl_name ${perl_src} NAME)
            set(asm_src ${LL_DIR}/${perl_name})

            if(ARCH_X86_64)
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
            elseif(ARCH_AARCH64)
                string(REPLACE ".pl" ".S" asm_src "${asm_src}")
                # generate asm file
                add_custom_command(
                    OUTPUT ${asm_src}
                    COMMAND ${PERL} ${perl_src} ${asm_flavour} ${asm_src}
                    DEPENDS ${perl_src}
                )
            endif()

            # add each asm file to LL_ASM_SRC
            list(APPEND LL_ASM_SRC ${asm_src})
        endif()
    endmacro()

    if(ARCH_X86_64)
        ll_add_asm(${LL_ASM_DIR}/ll_add-x64.pl        USE_ASM_ADD)
        ll_add_asm(${LL_ASM_DIR}/ll_cpuid-x86.pl      USE_ASM_CPUID)
        ll_add_asm(${LL_ASM_DIR}/ll_lehmer-x64.pl     USE_ASM_LEHMER)
        ll_add_asm(${LL_ASM_DIR}/ll_mul-x64.pl        USE_ASM_MUL)
        ll_add_asm(${LL_ASM_DIR}/ll_shift-x64.pl      USE_ASM_SHIFT)
        ll_add_asm(${LL_ASM_DIR}/ll_u256_add-x64.pl   USE_ASM_U256_ADD)
        ll_add_asm(${LL_ASM_DIR}/ll_u256_div-x64.pl   USE_ASM_U256_DIV)
        ll_add_asm(${LL_ASM_DIR}/ll_u256_fmod-x64.pl  USE_ASM_U256_FMOD)
        ll_add_asm(${LL_ASM_DIR}/ll_u256_mont-x64.pl  USE_ASM_U256_MONT)
        ll_add_asm(${LL_ASM_DIR}/ll_u256_mul-x64.pl   USE_ASM_U256_MUL)
        ll_add_asm(${LL_ASM_DIR}/ll_u256_shift-x64.pl USE_ASM_U256_SHIFT)
        ll_add_asm(${LL_ASM_DIR}/ll_utils-x64.pl      USE_ASM_UTILS)
    elseif(ARCH_AARCH64)
        ll_add_asm(${LL_ASM_DIR}/ll_add-aarch64.pl         USE_ASM_ADD)
        ll_add_asm(${LL_ASM_DIR}/ll_mul-aarch64.pl         USE_ASM_MUL)
        ll_add_asm(${LL_ASM_DIR}/ll_lehmer-aarch64.pl      USE_ASM_LEHMER)
        ll_add_asm(${LL_ASM_DIR}/ll_u256_add-aarch64.pl    USE_ASM_U256_ADD)
        ll_add_asm(${LL_ASM_DIR}/ll_u256_mont-aarch64.pl   USE_ASM_U256_MONT)
        ll_add_asm(${LL_ASM_DIR}/ll_u256_mul-aarch64.pl    USE_ASM_U256_MUL)
        ll_add_asm(${LL_ASM_DIR}/ll_u256_shift-aarch64.pl  USE_ASM_U256_SHIFT)
        ll_add_asm(${LL_ASM_DIR}/ll_utils-aarch64.pl       USE_ASM_UTILS)
    endif()
endif()

# generate config.h
configure_file(${FP256_DIR}/config.h.in ${FP256_DIR}/config.h @ONLY)
# it doesn't work...
# list(APPEND ADDITIONAL_CLEAN_FILES ${FP256_DIR}/config.h)

# source file
set(FP256_HEADER
    ${FP256_DIR}/config.h
    ${FP256_INCLUDE_DIR}/fp256/fp256.h
    ${FP256_INCLUDE_DIR}/fp256/fp256_ll.h
    ${FP256_SRC_DIR}/fp256_local.h
)

set(FP256_C_SRC
    ${FP256_HEADER}
    ${LL_DIR}/ll_add.c
    ${LL_DIR}/ll_convert.c
    ${LL_DIR}/ll_cpuid.c
    ${LL_DIR}/ll_div.c
    ${LL_DIR}/ll_naive_div.c
    ${LL_DIR}/ll_lehmer.c
    ${LL_DIR}/ll_mul.c
    ${LL_DIR}/ll_print.c
    ${LL_DIR}/ll_rand.c
    ${LL_DIR}/ll_shift.c
    ${LL_DIR}/ll_sys_rand.c
    ${LL_DIR}/ll_u256_add.c
    ${LL_DIR}/ll_u256_fmod.c
    ${LL_DIR}/ll_u256_mont.c
    ${LL_DIR}/ll_u256_mul.c
    ${LL_DIR}/ll_u256_shift.c
    ${LL_DIR}/ll_utils.c
    ${FP256_SRC_DIR}/cpuid.c
    ${FP256_SRC_DIR}/fp256_add.c
    ${FP256_SRC_DIR}/fp256_cmp.c
    ${FP256_SRC_DIR}/fp256_div.c
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
    add_library(fp256_static_object OBJECT ${FP256_C_SRC})
    add_library(fp256_asm_static_object OBJECT ${LL_ASM_SRC})
    target_compile_definitions(fp256_static_object PRIVATE BUILD_STATIC)
    target_compile_options(fp256_static_object PRIVATE ${EXTRA_C_FLAGS})
    # set_target_properties(fp256_static_object PROPERTIES COMPILE_FLAGS ${EXTRA_C_FLAGS})
    #
    add_library(fp256_static STATIC 
                $<TARGET_OBJECTS:fp256_static_object>
                $<TARGET_OBJECTS:fp256_asm_static_object>)
    target_compile_definitions(fp256_static PRIVATE BUILD_STATIC)
    target_link_libraries(fp256_static ${SANITIZER_FLAG})
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
    add_library(fp256_shared_object OBJECT ${FP256_C_SRC})
    add_library(fp256_asm_shared_object OBJECT ${LL_ASM_SRC})
    target_compile_definitions(fp256_shared_object PRIVATE BUILD_SHARED)
    target_compile_options(fp256_shared_object PRIVATE ${EXTRA_C_FLAGS})
    # set_target_properties(fp256_shared_object PROPERTIES COMPILE_FLAGS ${EXTRA_C_FLAGS})
    # PIC
    set_property(TARGET fp256_shared_object PROPERTY POSITION_INDEPENDENT_CODE 1)

    #
    add_library(fp256_shared SHARED 
                $<TARGET_OBJECTS:fp256_shared_object> 
                $<TARGET_OBJECTS:fp256_asm_shared_object>)
    target_compile_definitions(fp256_shared PRIVATE BUILD_SHARED)
    target_link_libraries(fp256_shared ${SANITIZER_FLAG})
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

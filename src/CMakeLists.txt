cmake_minimum_required(VERSION 3.1.0)

set(LL_DIR ${FP256_SRC_DIR}/ll)

# generate assembly file
if(USE_ASM)
    if(ARCH_X86_64)
        include(${CMAKE_MODULE_DIR}/fp256_generate_x64_asm.cmake)
    elseif(ARCH_AARCH64)
        include(${CMAKE_MODULE_DIR}/fp256_generate_aarch64_asm.cmake)
    elseif(ARCH_RISCV64)
        include(${CMAKE_MODULE_DIR}/fp256_generate_riscv64_asm.cmake)
    endif()
else()
    set(LL_ASM_SRC ${LL_DIR}/dummy.c)
endif()

# generate config.h
configure_file(${FP256_DIR}/config.h.in ${FP256_DIR}/config.h @ONLY)
# it doesn't work...
# list(APPEND ADDITIONAL_CLEAN_FILES ${FP256_DIR}/config.h)

# source file
set(FP256_HEADER
    ${FP256_INCLUDE_DIR}/fp256/fp256.h
    ${FP256_INCLUDE_DIR}/fp256/fp256_ll.h
)

set(FP256_HEADER_LOCAL
    ${FP256_DIR}/config.h
    ${FP256_SRC_DIR}/fp256_local.h
    ${LL_DIR}/ll_local.h
)

set(FP256_C_SRC
    ${FP256_HEADER}
    ${FP256_HEADER_LOCAL}
    ${LL_DIR}/ll_add.c
    ${LL_DIR}/ll_convert.c
    ${LL_DIR}/ll_cpuid.c
    ${LL_DIR}/ll_div.c
    ${LL_DIR}/ll_naive_div.c
    ${LL_DIR}/ll_lehmer.c
    ${LL_DIR}/ll_mont.c
    ${LL_DIR}/ll_mul.c
    ${LL_DIR}/ll_print.c
    ${LL_DIR}/ll_rand.c
    ${LL_DIR}/ll_shift.c
    ${LL_DIR}/ll_sys_rand.c
    ${LL_DIR}/ll_u256_add.c
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
    add_library(fp256_c_static_object OBJECT ${FP256_C_SRC})
    add_library(fp256_asm_static_object OBJECT ${LL_ASM_SRC})
    target_compile_definitions(fp256_c_static_object PRIVATE BUILD_STATIC)
    target_compile_options(fp256_c_static_object PRIVATE ${EXTRA_C_FLAGS})
    #
    add_library(fp256_static STATIC 
                $<TARGET_OBJECTS:fp256_c_static_object>
                $<TARGET_OBJECTS:fp256_asm_static_object>)
    target_compile_definitions(fp256_static PRIVATE BUILD_STATIC)
    target_link_libraries(fp256_static ${SANITIZER_FLAG})
    # static library output directory
    set_target_properties(fp256_static PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY_STATIC}
        VERSION ${FP256_VERSION}
        )
    # rename
    set_target_properties(fp256_static PROPERTIES OUTPUT_NAME "fp256")
    # install library
    INSTALL(TARGETS fp256_static ARCHIVE DESTINATION lib)
endif()

if(BUILD_SHARED)
    # build shared object
    add_library(fp256_c_shared_object OBJECT ${FP256_C_SRC})
    add_library(fp256_asm_shared_object OBJECT ${LL_ASM_SRC})
    target_compile_definitions(fp256_c_shared_object PRIVATE BUILD_SHARED)
    target_compile_options(fp256_c_shared_object PRIVATE ${EXTRA_C_FLAGS})
    # PIC
    set_property(TARGET fp256_c_shared_object PROPERTY POSITION_INDEPENDENT_CODE ON)

    #
    add_library(fp256_shared SHARED 
                $<TARGET_OBJECTS:fp256_c_shared_object> 
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
        SOVERSION ${FP256_VERSION_MAJOR}
        )
    # rename
    set_target_properties(fp256_shared PROPERTIES OUTPUT_NAME "fp256")
    # install library
    INSTALL(TARGETS fp256_shared LIBRARY DESTINATION lib)
endif()

# install headers
INSTALL(DIRECTORY ${FP256_INCLUDE_DIR}/fp256 DESTINATION include)

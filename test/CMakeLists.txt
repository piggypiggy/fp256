cmake_minimum_required(VERSION 3.1.0)

# header file directory
include_directories(${FP256_DIR}/)
include_directories(${FP256_INCLUDE_DIR}/)

SET(FP256_TEST_HEADER
    ${FP256_DIR}/test/test.h
)

set(FP256_TEST_SRC
    simple_thread.h
    simple_thread.c
    test.c
    ${FP256_TEST_HEADER}
)

set(fp256_shared_lib fp256_shared)
set(fp256_static_lib fp256_static)

# dependency
# prefer shared library
if(BUILD_SHARED)
    set(fp256_test_dep_lib ${fp256_shared_lib})
    add_definitions(-DBUILD_SHARED)
elseif(BUILD_STATIC)
    set(fp256_test_dep_lib ${fp256_static_lib})
    add_definitions(-DBUILD_STATIC)
else()
    message(FATAL_ERROR "no library compiled")
endif()

if(HAVE_PTHREAD)
    list(APPEND fp256_test_dep_lib pthread)
endif()

macro(fp256_add_test test_name test_exe test_src)
    add_executable(${test_exe} ${test_src} ${ARGN} ${FP256_TEST_SRC})
    target_compile_options(${test_exe} PRIVATE ${EXTRA_C_FLAGS})
    target_link_libraries(${test_exe} ${fp256_test_dep_lib} ${SANITIZER_FLAG})
    add_test(NAME ${test_name} COMMAND ${CMAKE_COMMAND} -DTEST_EXECUTABLE=$<TARGET_FILE:${test_exe}> -P ${CMAKE_MODULE_DIR}/run_test.cmake)
    if(MSVC)
        # path to search for shared library
        set_tests_properties(${test_name} PROPERTIES ENVIRONMENT "PATH=${OUTPUT_DIRECTORY_SHARED}")
    endif()
endmacro()

fp256_add_test(U256ADDLIMB_TEST   u256addlimb_test   u256addlimb_test.c)
fp256_add_test(U256ADD_TEST       u256add_test       u256add_test.c)
fp256_add_test(U256MULLIMB_TEST   u256mullimb_test   u256mullimb_test.c)
fp256_add_test(U256MUL_TEST       u256mul_test       u256mul_test.c)
fp256_add_test(U256SHIFT_TEST     u256shift_test     u256shift_test.c)
fp256_add_test(U256SQR_TEST       u256sqr_test       u256sqr_test.c)
fp256_add_test(U256SUBLIMB_TEST   u256sublimb_test   u256sublimb_test.c)
fp256_add_test(U256SUB_TEST       u256sub_test       u256sub_test.c)
fp256_add_test(LLADD_TEST         lladd_test         lladd_test.c)
fp256_add_test(LEADING_ZEROS_TEST leading_zeros_test leading_zeros_test.c)
fp256_add_test(LLSHIFT_TEST       llshift_test       llshift_test.c)
fp256_add_test(LLMONT_TEST        llmont_test        llmont_test.c)
fp256_add_test(LLMUL_TEST         llmul_test         llmul_test.c)
fp256_add_test(LLMULADDLIMB_TEST  llmuladdlimb_test  llmuladdlimb_test.c)
fp256_add_test(MUL_TEST           mul_test           mul_test.c)
fp256_add_test(DIV_TEST           div_test           div_test.c div_test_local.h)
fp256_add_test(GCD_TEST           gcd_test           gcd_test.c)
fp256_add_test(MODINV_TEST        modinv_test        modinv_test.c)
fp256_add_test(MODMUL_TEST        modmul_test        modmul_test.c)
fp256_add_test(MODSQR_TEST        modsqr_test        modsqr_test.c)
fp256_add_test(MODADD_TEST        modadd_test        modadd_test.c)
fp256_add_test(MODSUB_TEST        modsub_test        modsub_test.c)
fp256_add_test(MODEXP_TEST        modexp_test        modexp_test.c)
fp256_add_test(MONTMUL_TEST       montmul_test       montmul_test.c)
fp256_add_test(MONTSQR_TEST       montsqr_test       montsqr_test.c)
fp256_add_test(SHIFT_TEST         shift_test         shift_test.c)

if(ARCH_X86_64 AND USE_ASM_U256_DIV)
    fp256_add_test(NAIVEDIV_TEST      naivediv_test      naivediv_test.c div_test_local.h)
endif()

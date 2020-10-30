cmake_minimum_required(VERSION 3.1.0)

# header file directory
include_directories(${FP256_DIR}/)
include_directories(${FP256_INCLUDE_DIR}/)

SET(FP256_TEST_HEADER
    ${FP256_DIR}/test/test.h
    ${FP256_DIR}/test/test_local.h
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
    set(fp256_test_compile_definitions BUILD_SHARED)
elseif(BUILD_STATIC)
    set(fp256_test_dep_lib ${fp256_static_lib})
    set(fp256_test_compile_definitions BUILD_STATIC)
else()
    message(FATAL_ERROR "no library compiled")
endif()

if(HAVE_PTHREAD)
    list(APPEND fp256_test_dep_lib pthread)
endif()

macro(fp256_add_test test_name test_exe test_src)
    add_executable(${test_exe} ${test_src} ${FP256_TEST_SRC})
    target_compile_definitions(${test_exe} PRIVATE fp256_test_compile_definitions)
    target_link_libraries(${test_exe} ${fp256_test_dep_lib})
    add_test(${test_name} ${test_exe})
endmacro()

fp256_add_test(ADD_TEST     add_test     u256add_test.c)
fp256_add_test(MUL_TEST     mul_test     mul_test.c)
fp256_add_test(DIV_TEST     div_test     div_test.c)
fp256_add_test(GCD_TEST     gcd_test     gcd_test.c)
fp256_add_test(MODINV_TEST  modinv_test  modinv_test.c)
fp256_add_test(MODMUL_TEST  modmul_test  modmul_test.c)
fp256_add_test(MODSQR_TEST  modsqr_test  modsqr_test.c)
fp256_add_test(MODADD_TEST  modadd_test  modadd_test.c)
fp256_add_test(MODSUB_TEST  modsub_test  modsub_test.c)
fp256_add_test(MONTMUL_TEST montmul_test montmul_test.c)
fp256_add_test(MONTSQR_TEST montsqr_test montsqr_test.c)

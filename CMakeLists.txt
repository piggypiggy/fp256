cmake_minimum_required(VERSION 3.1.0)

# disable in-source build
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "In-source builds are not allowed.")
endif()

project(fp256 LANGUAGES C)

# version
set(FP256_VERSION_MAJOR 0)
set(FP256_VERSION_MINOR 6)
set(FP256_VERSION_PATCH 0)
set(FP256_VERSION "${FP256_VERSION_MAJOR}.${FP256_VERSION_MINOR}.${FP256_VERSION_PATCH}")

option(BUILD_STATIC "build static library" ON)
option(BUILD_SHARED "build shared library" ON)
option(BUILD_DOC "build api documentation" ON)
option(USE_DEV_RANDOM "use dev/random, default : use dev/urandom" OFF)
option(USE_ASM "use assembly code" ON)
option(USE_ASAN "use AddressSanitizer" OFF)
option(USE_MSAN "use MemorySanitizer" OFF)

if((NOT BUILD_STATIC) AND (NOT BUILD_SHARED))
    message(FATAL_ERROR "build static or shared or both")
endif()

# directory
set(FP256_DIR ${CMAKE_CURRENT_SOURCE_DIR})
# message("FP256_DIR : ${CMAKE_CURRENT_SOURCE_DIR}")
set(FP256_SRC_DIR ${FP256_DIR}/src)
# message("FP256_SRC_DIR : ${FP256_SRC_DIR}")
set(FP256_INCLUDE_DIR ${FP256_DIR}/include)
# message("FP256_INCLUDE_DIR : ${FP256_INCLUDE_DIR}")
set(CMAKE_MODULE_DIR ${FP256_DIR}/cmake)

if(USE_ASM)
    # need perl to generate asm file
    find_program(PERL perl)
    if(NOT PERL)
        message(FATAL_ERROR "perl not found")
    endif()
endif()

include(CheckIncludeFiles)
include(CheckFunctionExists)
include(${CMAKE_MODULE_DIR}/arch.cmake)
include(${CMAKE_MODULE_DIR}/platform.cmake)
include(${CMAKE_MODULE_DIR}/compiler.cmake)

# thread
find_package(Threads)
if(CMAKE_USE_PTHREADS_INIT)
    set(HAVE_PTHREAD 1)
elseif(CMAKE_USE_WIN32_THREADS_INIT)
    set(HAVE_WIN32_THREAD 1)
else()
    set(HAVE_NO_THREAD 1)
endif()

# library output directory
if(NOT DEFINED CMAKE_DEBUG_POSTFIX)
    set(CMAKE_DEBUG_POSTFIX "_d")
endif()
set(OUTPUT_DIRECTORY_STATIC ${FP256_DIR}/bin/$<CONFIG>/static)
set(OUTPUT_DIRECTORY_SHARED ${FP256_DIR}/bin/$<CONFIG>/shared)

# check_include_files(immintrin.h HAVE_IMMINTRIN_H)

# endianess
include(TestBigEndian)
TEST_BIG_ENDIAN(IS_BIG_ENDIAN)
if(IS_BIG_ENDIAN)
    set(ORDER 0) # config
else()
    set(ORDER 1) # config
endif()
set(ENDIANESS "O.O") # config

# document
if(BUILD_DOC)
    set(FP256_DOC_DIR ${FP256_DIR}/doc)
    include(${CMAKE_MODULE_DIR}/doxygen.cmake)
endif()

enable_testing()
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(bench)

# summary
set(BUILD_FLAGS CMAKE_C_FLAGS_${CMAKE_BUILD_TYPE})
string(TOUPPER ${BUILD_FLAGS} BUILD_FLAGS)
if(BUILD_SHARED)
    list(APPEND BUILD_LIBRARY shared)
endif()
if(BUILD_STATIC)
    list(APPEND BUILD_LIBRARY static)
endif()
message("================ summary ================")
message("os               : ${CMAKE_SYSTEM_NAME}")
message("processor        : ${CMAKE_SYSTEM_PROCESSOR}")
message("build            : ${BUILD_LIBRARY}")
message("build type       : ${CMAKE_BUILD_TYPE}")
message("C compiler       : ${CMAKE_C_COMPILER}")
message("C compiler flags : ${CMAKE_C_FLAGS} ${${BUILD_FLAGS}} ${EXTRA_C_FLAGS}")
if(USE_ASM)
    if(CMAKE_ASM_NASM_COMPILER_ID STREQUAL "NASM")
        message("assembler        : ${CMAKE_ASM_NASM_COMPILER}")
    elseif(CMAKE_ASM_MASM_COMPILER_ID STREQUAL "MASM")
        message("assembler        : ${CMAKE_ASM_MASM_COMPILER}")
    else()
        message("assembler        : ${CMAKE_ASM_COMPILER}")
    endif()
endif()
message("=========================================")

cmake_minimum_required(VERSION 3.1.0)

project(bench LANGUAGES C)

include_directories(${FP256_DIR}/)
include_directories(${FP256_INCLUDE_DIR}/)

SET(BENCH_HEADER
    ${FP256_DIR}/config.h
    ${FP256_DIR}/bench/bench.h
)

set(BENCH_SRC
    ${SPEED_HEADER}
    bench.c
    ../test/simple_thread.h
    ../test/simple_thread.c
    ../test/test.c
)

add_executable(bench ${BENCH_SRC})

set(static_lib fp256_static)
set(shared_lib fp256_shared)

# dependency
# prefer static library :)
if(BUILD_STATIC)
    set(dep_lib ${static_lib})
    add_definitions(-DBUILD_STATIC)
elseif(BUILD_SHARED)
    add_definitions(-DBUILD_SHARED)
    set(dep_lib ${shared_lib})
else()
    message(FATAL_ERROR "no library compiled")
endif()

set(bench_dep_lib ${dep_lib})
if(HAVE_PTHREAD)
    list(APPEND bench_dep_lib pthread)
endif()

target_link_libraries(bench ${bench_dep_lib} ${SANITIZER_FLAG})
target_compile_options(bench PRIVATE ${EXTRA_C_FLAGS})

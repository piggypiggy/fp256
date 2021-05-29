# cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain/aarch64-linux-gcc.cmake ..
# qemu-aarch64 -L /usr/aarch64-linux-gnu ./hello_world
# TESTS_EXECUTABLE_LOADER=qemu-aarch64 TESTS_EXECUTABLE_LOADER_ARGUMENTS="-L;/usr/aarch64-linux-gnu" ctest --output-on-failure

set(toolchain_path /usr/bin)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(CMAKE_C_COMPILER ${toolchain_path}/aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER ${toolchain_path}/aarch64-linux-gnu-g++)

set(CMAKE_FIND_ROOT_PATH /usr/aarch64-linux-gnu)

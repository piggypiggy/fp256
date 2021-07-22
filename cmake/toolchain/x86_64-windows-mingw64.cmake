# cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain/x86_64-windows-mingw64.cmake ..
# qemu-x86_64 -L /usr/x86_64-w64-mingw32 ./hello_world
# TESTS_EXECUTABLE_LOADER=qemu-x86_64 TESTS_EXECUTABLE_LOADER_ARGUMENTS="-L;/usr/x86_64-w64-mingw32" ctest --output-on-failure

set(toolchain_path /usr/bin)
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)
set(CMAKE_C_COMPILER ${toolchain_path}/x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER ${toolchain_path}/x86_64-w64-mingw32-g++)

set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)

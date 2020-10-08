# cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain/x86_64-windows-mingw64.cmake ..

set(toolchain_path /usr/bin)
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)
set(CMAKE_C_COMPILER ${toolchain_path}/x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER ${toolchain_path}/x86_64-w64-mingw32-g++)

set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)
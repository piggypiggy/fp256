# cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain/aarch64-linux-gcc.cmake ..

set(toolchain_path /usr/bin)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(CMAKE_C_COMPILER ${toolchain_path}/aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER ${toolchain_path}/aarch64-linux-gnu-g++)

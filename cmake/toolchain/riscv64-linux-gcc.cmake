# cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain/riscv64-linux-gcc.cmake ..

set(toolchain_path /usr/bin)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR riscv64)
set(CMAKE_C_COMPILER ${toolchain_path}/riscv64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER ${toolchain_path}/riscv64-linux-gnu-g++)

set(CMAKE_FIND_ROOT_PATH /usr/riscv64-linux-gnu)

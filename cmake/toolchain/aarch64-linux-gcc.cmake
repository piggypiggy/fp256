# cmake -DCMAKE_TOOLCHAIN_FILE=../aarch64-linux-gcc.cmake ..

set(toolchain_path /usr)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_COMPILER ${toolchain_path}/bin/aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER ${toolchain_path}/bin/aarch64-linux-gnu-g++)
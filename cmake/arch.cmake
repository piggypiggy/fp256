if(CMAKE_SYSTEM_PROCESSOR STREQUAL "mips64")
    set(ARCH_MIPS64 1)
    message(FATAL_ERROR "mips processor is not supported yet :(")

elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "(arm64|aarch64)")
    set(ARCH_AARCH64 1)

elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "riscv64")
    set(ARCH_RISCV64 1)

elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86_64)|(AMD64|amd64)")
    set(ARCH_X86_64 1)

else()
    message(FATAL_ERROR "Unsupported processor: ${CMAKE_SYSTEM_PROCESSOR}")

endif()

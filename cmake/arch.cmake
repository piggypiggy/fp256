if(CMAKE_SYSTEM_PROCESSOR STREQUAL "mips64")
    set(ARCH ARCH_MIPS64)
    set(ARCH_MIPS64 1)
    message(FATAL_ERROR "mips processor is not supported yet :(")

elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "(arm64|aarch64)")
    set(ARCH ARCH_AARCH64)
    set(ARCH_AARCH64 1)
    # message(FATAL_ERROR "aarch64 processor is not supported yet :(")

elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86_64)|(AMD64|amd64)")
    set(ARCH ARCH_X86_64)
    set(ARCH_X86_64 1)

elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "(powerpc64|ppc64)")
    set(ARCH ARCH_PPC64)
    set(ARCH_PPC64 1)
    message(FATAL_ERROR "powerpc64 processor is not supported yet :(")

else()
    message(FATAL_ERROR "unknown processor")

endif()


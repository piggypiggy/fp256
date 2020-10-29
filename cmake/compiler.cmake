# default : Release
if(NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE Release)
endif()

if(CMAKE_C_COMPILER_ID MATCHES ".*Clang")
    set(COMPILER_CLANG 1)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic")

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    endif()

    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fomit-frame-pointer")
    endif()

    # sanitizer
    if(USE_ASAN)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
    elseif(USE_MSAN AND NOT OS_MACOSX)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=memory")
    endif()

    if(ARCH_X86_64 AND USE_ASM)
        enable_language(ASM)
    endif()

elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(COMPILER_GCC 1)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic")

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    endif()

    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fomit-frame-pointer")
    endif()

    # sanitizer
    if(USE_ASAN)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
    endif()

    if(ARCH_X86_64 AND USE_ASM)
        enable_language(ASM)
        if(OS_LINUX)
            set(asm_flavour )
        elseif(MINGW)
            set(asm_flavour mingw64)
        endif()
    endif()

elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(COMPILER_MSVC 1)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Wall wd4668 /wd4820")
    add_definitions(/D_CRT_SECURE_NO_WARNINGS)
    if(ARCH_X86_64 AND USE_ASM)
        enable_language(ASM_MASM)
        set(asm_flavour masm)
    endif()

endif()

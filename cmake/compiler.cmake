# default : Release
if(NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE Release)
endif()

if(CMAKE_C_COMPILER_ID MATCHES ".*Clang")
    set(COMPILER_CLANG 1)
    set(EXTRA_C_FLAGS ${EXTRA_C_FLAGS} -Wall -Wextra -pedantic)

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(EXTRA_C_FLAGS ${EXTRA_C_FLAGS})
    endif()

    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(EXTRA_C_FLAGS ${EXTRA_C_FLAGS} -fomit-frame-pointer)
    endif()

    # sanitizer
    if(USE_ASAN)
        set(SANITIZER_FLAG -fsanitize=address)
    elseif(USE_MSAN AND NOT OS_MACOSX)
        set(SANITIZER_FLAG -fsanitize=memory)
    endif()
    set(EXTRA_C_FLAGS ${EXTRA_C_FLAGS} ${SANITIZER_FLAG})

    if(USE_ASM)
        enable_language(ASM)
        if(OS_LINUX)
            set(asm_flavour linux64)
        endif()
    endif()

elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(COMPILER_GCC 1)
    set(EXTRA_C_FLAGS ${EXTRA_C_FLAGS} -Wall -Wextra -pedantic)

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(EXTRA_C_FLAGS ${EXTRA_C_FLAGS})
    endif()

    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(EXTRA_C_FLAGS ${EXTRA_C_FLAGS} -fomit-frame-pointer)
    endif()

    # sanitizer
    if(USE_ASAN)
        set(SANITIZER_FLAG -fsanitize=address)
    endif()
    set(EXTRA_C_FLAGS ${EXTRA_C_FLAGS} ${SANITIZER_FLAG})

    if(USE_ASM)
        enable_language(ASM)
        if(OS_LINUX)
            set(asm_flavour linux64)
        elseif(MINGW)
            set(asm_flavour mingw64)
        endif()
    endif()

elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(COMPILER_MSVC 1)
    set(EXTRA_C_FLAGS ${EXTRA_C_FLAGS} /Wall /wd4668 /wd4820)
    add_definitions(/D_CRT_SECURE_NO_WARNINGS)
    if(USE_ASM)
        enable_language(ASM_MASM)
        set(asm_flavour masm)
    endif()

endif()

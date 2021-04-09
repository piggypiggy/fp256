# default : Release
if(NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE Release)
endif()

if(CMAKE_C_COMPILER_ID MATCHES ".*Clang")
    set(EXTRA_C_FLAGS ${EXTRA_C_FLAGS} -Wall -Wextra -pedantic)

    if(CMAKE_BUILD_TYPE MATCHES "(Debug|DEBUG|debug)")
        set(EXTRA_C_FLAGS ${EXTRA_C_FLAGS})
    endif()

    if(CMAKE_BUILD_TYPE MATCHES "(Release|RELEASE|release)")
        set(EXTRA_C_FLAGS ${EXTRA_C_FLAGS} -fomit-frame-pointer)
    endif()

    # sanitizer
    if(USE_ASAN)
        set(SANITIZER_FLAG -fsanitize=address)
    elseif(USE_MSAN AND NOT OS_MACOSX)
        set(SANITIZER_FLAG -fsanitize=memory)
    endif()
    set(EXTRA_C_FLAGS ${EXTRA_C_FLAGS} ${SANITIZER_FLAG})

elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(EXTRA_C_FLAGS ${EXTRA_C_FLAGS} -Wall -Wextra -pedantic)

    if(CMAKE_BUILD_TYPE MATCHES "(Debug|DEBUG|debug)")
        set(EXTRA_C_FLAGS ${EXTRA_C_FLAGS})
    endif()

    if(CMAKE_BUILD_TYPE MATCHES "(Release|RELEASE|release)")
        set(EXTRA_C_FLAGS ${EXTRA_C_FLAGS} -fomit-frame-pointer)
    endif()

    # sanitizer
    if(USE_ASAN)
        set(SANITIZER_FLAG -fsanitize=address)
    endif()
    set(EXTRA_C_FLAGS ${EXTRA_C_FLAGS} ${SANITIZER_FLAG})

elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(EXTRA_C_FLAGS ${EXTRA_C_FLAGS} /Wall /wd4668 /wd4820 /wd4255)
    if(USE_ASM)
        set(USE_MASM ON)
    endif()
    add_definitions(/D_CRT_SECURE_NO_WARNINGS)
endif()

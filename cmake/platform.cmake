

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(OS_LINUX 1)

elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(OS_WINDOWS 1)

elseif(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
    set(OS_FREEBSD 1)

elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(OS_MACOSX 1)
    if(ARCH_X86_64 AND USE_ASM)
        set(asm_flavour macosx)
    endif()

elseif(CMAKE_SYSTEM_NAME STREQUAL "iOS")
    set(OS_IOS 1)

elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
    set(OS_ANDROID 1)

else()
    message(FATAL_ERROR "Unknown OS")
endif()

if(NOT OS_WINDOWS)
    check_include_files(sys/random.h HAVE_SYS_RANDOM_H)
    check_function_exists(getrandom HAVE_GETRANDOM)
    check_function_exists(getentropy HAVE_GETENTROPY)
endif()
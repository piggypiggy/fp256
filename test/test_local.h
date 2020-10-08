#pragma once

#define RETURN_IF_ERROR(TEST)    \
    do {                         \
        if (TEST != FP256_OK) {  \
            fp256_deinit();      \
            return -1;           \
        }                        \
    } while(0);

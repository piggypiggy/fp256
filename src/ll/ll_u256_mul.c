#include <fp256/fp256_ll.h>
#include "ll_local.h"

#ifndef USE_ASM
void ll_u256_mul_limbs(u64 rd[4*2], const u64 ad[4], const u64 bd[4], size_t al, size_t bl)
{
    return 0;
}

void ll_u256_mul(u64 rd[4*2], const u64 ad[4], const u64 bd[4])
{
    return 0;
}

void void ll_u256_sqr(u64 rd[4*2], const u64 ad[4])
{
    return 0;
}
#endif

#include <fp256/fp256_ll.h>
#include "ll_local.h"

#ifndef USE_ASM
void ll_u256_fmod_neg(u64 r[4], const u64 a[4], const u64 m[4])
{
    return;
}

void ll_u256_fmod_double(u64 r[4], const u64 a[4], const u64 m[4])
{
    return;
}

void ll_u256_fmod_triple(u64 r[4], const u64 a[4], const u64 m[4])
{
    return;
}

void ll_u256_fmod_div_by_2(u64 r[4], const u64 a[4], const u64 m[4])
{
    return;
}

void ll_u256_fmod_add(u64 r[4], const u64 a[4], const u64 b[4], const u64 m[4])
{
    return;
}

void ll_u256_fmod_add_limb(u64 r[4], const u64 a[4], const u64 w, const u64 m[4])
{
    return;
}

void ll_u256_fmod_sub(u64 r[4], const u64 a[4], const u64 b[4], const u64 m[4])
{
    return;
}

void ll_u256_fmod_sub_limb(u64 r[4], const u64 a[4], const u64 w, const u64 m[4])
{
    return;
}
#endif
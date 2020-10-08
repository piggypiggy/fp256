#include <fp256/fp256_ll.h>

#ifndef USE_ASM
size_t ll_mul_limb(u64 *rd, const u64 *ad, u64 b, size_t al)
{
    return 0;
}

size_t ll_muladd_limb(u64 *rd, const u64 *ad, u64 b, size_t rl, size_t al)
{
    return 0;
}

size_t ll_mulsub_limb(u64 *rd, const u64 *ad, u64 b, size_t al)
{
    return 0;
}
#endif

size_t ll_mul(u64 *rd, const u64 *ad, const u64 *bd, size_t al, size_t bl)
{
    size_t i, rl;
    u64 *trd;

    rl = 0;
    trd = rd;
    ll_set_zero(rd, al + bl);
    for (i = 0; i < bl; i++) {
        rl = ll_muladd_limb(rd, ad, bd[i], rl, al);
        rd++;
        rl--;
    }

    rl = ll_num_limbs(trd, al + bl);
    return rl;
}
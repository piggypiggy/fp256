#include <fp256/fp256_ll.h>
#include "ll_local.h"

#ifndef USE_ASM
u64 ll_add_limb(u64 *rd, u64 *ad, u64 b, size_t al)
{
    return 0;
}

u64 ll_add_limbs(u64 *rd, const u64 *ad, const u64 *bd, size_t l)
{
    return 0;
}
#endif

size_t ll_add(u64 *rd, const u64 *ad, const u64 *bd, size_t al, size_t bl)
{
    size_t l, dif;
    u64 carry, *trd;

    trd = rd;
    if (al >= bl) {
        l = bl;
        dif = al - bl;
    }
    else {
        l = al;
        dif = bl - al;
    }

    carry = ll_add_limbs(rd, ad, bd, l);
    rd += l;
    carry = ll_add_limb(rd, rd, carry, dif);
    rd += dif;
    rd[0] = carry;

    l = ll_num_limbs(trd, l + dif + 1);
    return l;
}

#ifndef USE_ASM
u64 ll_sub_limb(u64 *rd, u64 *ad, u64 b, size_t al)
{
    return 0;
}

u64 ll_sub_limbs(u64 *rd, const u64 *ad, const u64 *bd, size_t l)
{
    return 0;
}
#endif

size_t ll_sub(u64 *rd, const u64 *ad, const u64 *bd, size_t al, size_t bl)
{
    size_t l, dif;
    u64 borrow, *trd;

    trd = rd;
    /* assume ad >= bd, so al >= bl */
    dif = al - bl;

    borrow = ll_sub_limbs(rd, ad, bd, bl);
    rd += bl;
    ll_sub_limb(rd, rd, borrow, dif);

    l = ll_num_limbs(trd, al);
    return l;
}

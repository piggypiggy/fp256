#include <fp256/fp256_ll.h>
#include "ll_local.h"

#ifndef USE_ASM
size_t ll_mul_limb(u64 *rd, const u64 *ad, u64 b, size_t al)
{
    u64 hi, lo, t;
    size_t i;

    t = 0;
    for (i = 0; i < al; i++) {
        LL_MUL64(hi, lo, ad[0], b);
        t += lo;
        ad++;
        rd[0] = t;
        hi += (t < lo);
        rd++;
        t = hi;
    }
    rd[0] = t;

    return al + (t != 0);
}

size_t ll_muladd_limb(u64 *rd, const u64 *ad, u64 b, size_t rl, size_t al)
{
    u64 hi, lo, t1, t2, carry;
    size_t i;

    /* rd[0, (al-1)] += ad * b */
    t1 = 0;
    for (i = 0; i < al; i++) {
        LL_MUL64(hi, lo, ad[0], b);
        t1 += lo;
        t2 = rd[0];
        hi += (t1 < lo);
        t1 += t2;
        ad++;
        rd[0] = t1;
        hi += (t1 < t2);
        rd++;
        t1 = hi;
    }

    /* rd[al, (rl-1)] += t1 */
    if (al < rl) {
        carry = 0;
        for (i = al; i < rl; i++) {
            t2 = rd[0] + carry;
            carry = (t2 < carry);
            t2 += t1;
            carry += (t2 < t1);
            rd[0] = t2;
            t1 = 0;
            rd++;
        }
        rd[0] = carry;
    }
    else {
        carry = 1;
        rd[0] = t1;
    }

    return (i + carry);
}

size_t ll_mulsub_limb(u64 *rd, const u64 *ad, u64 b, size_t rl, size_t al)
{
    u64 hi, lo, t1, t2, borrow;
    size_t i;

    /* rd[0, (al-1)] -= ad * b */
    t1 = 0;
    for (i = 0; i < al; i++) {
        LL_MUL64(hi, lo, ad[0], b);
        t1 += lo;
        t2 = rd[0];
        hi += (t1 < lo);
        hi += (t2 < t1);
        t2 -= t1;
        ad++;
        rd[0] = t2;
        rd++;
        t1 = hi;
    }

    /* rd[al, (rl-1)] -= t1 */
    borrow = 0;
    for (i = al; i < rl; i++) {
        t1 += borrow;
        t2 = rd[0];
        borrow = (t1 < borrow);
        borrow += (t2 < t1);
        t2 -= t1;
        t1 = 0;
        rd[0] = t2;
        rd++;
    }

    while (rl > 0) {
        rd--;
        if (rd[0] > 0)
            break;
        rl--;
    }

    return rl;
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

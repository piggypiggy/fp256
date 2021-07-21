/******************************************************************************
 *                                                                            *
 * Copyright 2020-2021 Jiang Mengshan                                         *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License");            *
 * you may not use this file except in compliance with the License.           *
 * You may obtain a copy of the License at                                    *
 *                                                                            *
 *    http://www.apache.org/licenses/LICENSE-2.0                              *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 *                                                                            *
 *****************************************************************************/

#include <fp256/fp256_ll.h>
#include "ll_local.h"

#ifndef USE_ASM_ADD
u64 ll_add_limb(u64 *rd, const u64 *ad, u64 b, size_t al)
{
    u64 t;
    size_t i;

    for (i = 0; i < al; i++) {
        t = ad[0] + b;
        ad++;
        b = (t < b);
        rd[0] = t;
        rd++;
    }

    return b;
}

u64 ll_add_limbs(u64 *rd, const u64 *ad, const u64 *bd, size_t l)
{
    u64 carry, t1, t2;
    size_t i;

    carry = 0;
    for (i = 0; i < l; i++) {
        t1 = ad[0] + carry;
        ad++;
        carry = (t1 < carry);
        t2 = bd[0] + t1;
        bd++;
        rd[0] = t2;
        carry += (t2 < t1);
        rd++;
    }

    return carry;
}
#endif

u64 ll_add(u64 *rd, const u64 *ad, const u64 *bd, size_t al, size_t bl)
{
    size_t l, dif;
    u64 carry;
    const u64 *tad, *tbd;

    /* let tad >= tbd */
    if (al >= bl) {
        l = bl;
        tad = ad;
        tbd = bd;
        dif = al - bl;
    }
    else {
        l = al;
        tad = bd;
        tbd = ad;
        dif = bl - al;
    }

    carry = ll_add_limbs(rd, tad, tbd, l);
    rd += l;
    tad += l;
    carry = ll_add_limb(rd, tad, carry, dif);
    rd += dif;
    rd[0] = carry;

    return carry;
}

#ifndef USE_ASM_ADD
u64 ll_sub_limb(u64 *rd, const u64 *ad, u64 b, size_t al)
{
    u64 borrow, t;
    size_t i;

    borrow = 0;
    for (i = 0; i < al; i++) {
        t = ad[0];
        ad++;
        borrow = (t < b);
        rd[0] = t - b;
        rd++;
        b = borrow;
    }

    return b;
}

u64 ll_sub_limbs(u64 *rd, const u64 *ad, const u64 *bd, size_t l)
{
    u64 borrow, t, t1, t2;
    size_t i;

    borrow = 0;
    for (i = 0; i < l; i++) {
        t1 = ad[0];
        t2 = bd[0];
        t = (t1 < borrow);
        t1 -= borrow;
        ad++;
        bd++;
        t += (t1 < t2);
        rd[0] = t1 - t2;
        borrow = t;
        rd++;
    }

    return borrow;
}
#endif

u64 ll_sub(u64 *rd, const u64 *ad, const u64 *bd, size_t al, size_t bl)
{
    size_t dif;
    u64 borrow;

    /* assume ad >= bd, so al >= bl */
    assert(al >= bl);
    dif = al - bl;

    borrow = ll_sub_limbs(rd, ad, bd, bl);
    rd += bl;
    ad += bl;
    borrow = ll_sub_limb(rd, ad, borrow, dif);

    assert(borrow == 0);
    return borrow;
}

#ifndef USE_ASM_ADD
void ll_mont_cond_sub_limbs(u64 *rd, const u64 *ad, const u64 *bd, size_t l)
{
    size_t i;
    u64 *td, borrow, mask_t, mask_a;

    td = (u64*)alloca(l * sizeof(u64));
    borrow = ll_sub_limbs(td, ad, bd, l);
    mask_t = (ad[l] >= borrow);
    mask_a = ~mask_t;

    for (i = 0; i < l; i++)
        rd[i] = (td[i] & mask_t) | (ad[i] & mask_a);

    return;
}
#endif

/******************************************************************************
 *                                                                            *
 * Copyright 2020 Meng-Shan Jiang                                             *
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

#ifndef USE_ASM
u64 ll_add_limb(u64 *rd, const u64 *ad, u64 b, size_t al)
{
    u64 carry, t1, t2;
    size_t i;

    carry = 0;
    for (i = 0; i < al; i++) {
        t1 = ad[0] + carry;
        ad++;
        carry = (t1 < carry);
        t2 = b + t1;
        rd[0] = t2;
        carry += (t2 < t1);
        b = 0;
        rd++;
    }

    return carry;
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
u64 ll_sub_limb(u64 *rd, const u64 *ad, u64 b, size_t al)
{
    u64 borrow, t1;
    size_t i;

    borrow = 0;
    for (i = 0; i < al; i++) {
        t1 = ad[0];
        ad++;
        rd[0] = t1 - b - borrow;
        if (t1 != b)
            borrow = (t1 < b);
        rd++;
        b = 0;
    }

    return borrow;
}

u64 ll_sub_limbs(u64 *rd, const u64 *ad, const u64 *bd, size_t l)
{
    u64 borrow, t1, t2;
    size_t i;

    borrow = 0;
    for (i = 0; i < l; i++) {
        t1 = ad[0];
        t2 = bd[0];
        ad++;
        bd++;
        rd[0] = t1 - t2 - borrow;
        if (t1 != t2)
            borrow = (t1 < t2);
        rd++;
    }

    return borrow;
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

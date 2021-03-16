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

#ifndef USE_ASM_MUL
u64 ll_mul_limb(u64 *rd, const u64 *ad, u64 b, size_t al)
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

    return t;
}

u64 ll_muladd_limb(u64 *rd, const u64 *ad, u64 b, size_t rl, size_t al)
{
    u64 hi, lo, t1, t2;
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
    for (i = al; i < rl; i++) {
        t2 = rd[0] + t1;
        t1 = (t2 < t1);
        rd[0] = t2;
        rd++;
    }

    rd[0] = t1;
    return t1;
}

u64 ll_mulsub_limb(u64 *rd, const u64 *ad, u64 b, size_t rl, size_t al)
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
        t2 = rd[0];
        borrow = (t2 < t1);
        t2 -= t1;
        t1 = borrow;
        rd[0] = t2;
        rd++;
    }

    return t1;
}
#endif

u64 ll_mul(u64 *rd, const u64 *ad, const u64 *bd, size_t al, size_t bl)
{
    size_t i;

    ll_set_zero(rd, al);

    for (i = 0; i < bl; i++) {
        /* rd[0:al-1] += ad[0:al-1]*b, rd[al] is set accordingly. */
        ll_muladd_limb(rd, ad, bd[i], al, al);
        rd++;
    }

    i += al;
    /* in case (al = bl = 0) */
    return (i > 0 ? rd[al - 1] : 0);
}

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

#ifndef USE_ASM_U256_MONT
void ll_u256_mont_mul(u64 rd[4], const u64 ad[4], const u64 bd[4], const u64 Nd[4], u64 k0)
{
    u64 hi, lo, t;
    u64 a0, a1, a2, a3;
    u64 n0, n1, n2, n3;
    u64 r0, r1, r2, r3, r4, r5;

    a0 = ad[0]; a1 = ad[1]; a2 = ad[2]; a3 = ad[3];
    n0 = Nd[0]; n1 = Nd[1]; n2 = Nd[2]; n3 = Nd[3];

    /* ad * bd[0] */
    r5 = 0;
    LL_U256_MUL_U64(r4, r3, r2, r1, r0, a3, a2, a1, a0, bd[0], hi, lo, t);
    /* reduction 1 */
    LL_U256_MONT_REDC1(r5, r4, r3, r2, r1, r0, n3, n2, n1, n0, k0, hi, lo, t);

    /* r0,r5,r4,r3,r2,r1 += ad * bd[1] */
    LL_U256_MONT_MUL_ADD(r0, r5, r4, r3, r2, r1, a3, a2, a1, a0, bd[1], hi, lo, t);
    /* reduction 2 */
    LL_U256_MONT_REDC1(r0, r5, r4, r3, r2, r1, n3, n2, n1, n0, k0, hi, lo, t);

    /* r1,r0,r5,r4,r3,r2 += ad * bd[2] */
    LL_U256_MONT_MUL_ADD(r1, r0, r5, r4, r3, r2, a3, a2, a1, a0, bd[2], hi, lo, t);
    /* reduction 3 */
    LL_U256_MONT_REDC1(r1, r0, r5, r4, r3, r2, n3, n2, n1, n0, k0, hi, lo, t);

    /* r2,r1,r0,r5,r4,r3 += ad * bd[3] */
    LL_U256_MONT_MUL_ADD(r2, r1, r0, r5, r4, r3, a3, a2, a1, a0, bd[3], hi, lo, t);
    /* reduction 4 */
    LL_U256_MONT_REDC1(r2, r1, r0, r5, r4, r3, n3, n2, n1, n0, k0, hi, lo, t);

    /* r2,r1,r0,r5,r4 conditional sub N */
    LL_U256_MONT_COND_SUB(a3, a2, a1, a0, r2, r1, r0, r5, r4, n3, n2, n1, n0, b, t);
    rd[0] = a0;
    rd[1] = a1;
    rd[2] = a2;
    rd[3] = a3;

    return;
}

void ll_u256_mont_sqr(u64 rd[4], const u64 ad[4], const u64 Nd[4], u64 k0)
{
    u64 hi, lo, t;
    u64 a0, a1, a2, a3;
    u64 n0, n1, n2, n3;
    u64 r0, r1, r2, r3, r4, r5, r6, r7;
    u64 z;

    a0 = ad[0]; a1 = ad[1]; a2 = ad[2]; a3 = ad[3];
    n0 = Nd[0]; n1 = Nd[1]; n2 = Nd[2]; n3 = Nd[3];

    /* emmm, a copy of ll_u256_sqr */

    /* ad[0] * ad[1] */
    LL_MUL64(r2, r1, a0, a1);

    /* + ad[0] * ad[2] */
    LL_MUL64(r3, lo, a0, a2);
    r2 += lo;
    r3 += (r2 < lo);

    /* + ad[0] * ad[3] */
    LL_MUL64(r4, lo, a0, a3);
    r3 += lo;
    r4 += (r3 < lo);

    /* + ad[1] * ad[2] */
    LL_MUL64(hi, lo, a1, a2);
    r3 += lo;
    hi += (r3 < lo);

    /* + ad[1] * ad[3] */
    LL_MUL64(r5, lo, a1, a3);
    r4 += hi;
    r5 += (r4 < hi);
    r4 += lo;
    r5 += (r4 < lo);

    /* + ad[2] * ad[3] */
    LL_MUL64(r6, lo, a2, a3);
    r5 += lo;
    r6 += (r5 < lo);

    /* ad[6]~ad[1] << 1 */
    r7 = r6 >> 63;
    r6 = (r6 << 1) | (r5 >> 63);
    r5 = (r5 << 1) | (r4 >> 63);
    r4 = (r4 << 1) | (r3 >> 63);
    r3 = (r3 << 1) | (r2 >> 63);
    r2 = (r2 << 1) | (r1 >> 63);
    r1 <<= 1;

    /* + ad[0]^2 */
    LL_SQR64(hi, r0, a0);
    r1 += hi;
    t = (r1 < hi);

    /* + ad[1]^2 */
    LL_SQR64(hi, lo, a1);
    r2 += t;
    hi += (r2 < t);
    r2 += lo;
    hi += (r2 < lo);
    r3 += hi;
    t = (r3 < hi);

    /* + ad[2]^2 */
    LL_SQR64(hi, lo, a2);
    r4 += t;
    hi += (r4 < t);
    r4 += lo;
    hi += (r4 < lo);
    r5 += hi;
    t = (r5 < hi);

    /* + ad[3]^2 */
    LL_SQR64(hi, lo, a3);
    r6 += t;
    hi += (r6 < t);
    r6 += lo;
    hi += (r6 < lo);
    r7 += hi;

    /* reduction 1 */
    LL_U256_MONT_REDC2(r3, r2, r1, r0, n3, n2, n1, n0, k0, hi ,lo, t);

    /* reduction 2 */
    LL_U256_MONT_REDC2(r0, r3, r2, r1, n3, n2, n1, n0, k0, hi ,lo, t);

    /* reduction 3 */
    LL_U256_MONT_REDC2(r1, r0, r3, r2, n3, n2, n1, n0, k0, hi ,lo, t);

    /* reduction 4 */
    LL_U256_MONT_REDC2(r2, r1, r0, r3, n3, n2, n1, n0, k0, hi ,lo, t);

    /* r7,r6,r5,r4 += r3,r2,r1,r0 */
    LL_U256_ADD(z, r7, r6, r5, r4, r3, r2, r1, r0, t);

    /* z,r7,r6,r5,r4 conditional sub N */
    LL_U256_MONT_COND_SUB(a3, a2, a1, a0, z, r7, r6, r5, r4, n3, n2, n1, n0, b, t);
    rd[0] = a0;
    rd[1] = a1;
    rd[2] = a2;
    rd[3] = a3;

    return;
}

void ll_u256_mont_reduce(u64 rd[4], const u64 ad[4], const u64 Nd[4], u64 k0)
{
    u64 hi, lo, t;
    u64 a0, a1, a2, a3;
    u64 n0, n1, n2, n3;
    u64 r0, r1, r2, r3;

    r0 = ad[0]; r1 = ad[1]; r2 = ad[2]; r3 = ad[3];
    n0 = Nd[0]; n1 = Nd[1]; n2 = Nd[2]; n3 = Nd[3];

    /* reduction 1 */
    LL_U256_MONT_REDC2(r3, r2, r1, r0, n3, n2, n1, n0, k0, hi ,lo, t);

    /* reduction 2 */
    LL_U256_MONT_REDC2(r0, r3, r2, r1, n3, n2, n1, n0, k0, hi ,lo, t);

    /* reduction 3 */
    LL_U256_MONT_REDC2(r1, r0, r3, r2, n3, n2, n1, n0, k0, hi ,lo, t);

    /* reduction 4 */
    LL_U256_MONT_REDC2(r2, r1, r0, r3, n3, n2, n1, n0, k0, hi ,lo, t);

    /* r3,r2,r1,r0 conditional sub N */
    LL_U256_MONT_COND_SUB(a3, a2, a1, a0, 0, r3, r2, r1, r0, n3, n2, n1, n0, b, t);
    rd[0] = a0;
    rd[1] = a1;
    rd[2] = a2;
    rd[3] = a3;

    return;
}
#endif

void ll_u256_mont_exp(u64 rd[4], const u64 ad[4], const u64 *ed, size_t el, const u64 RR[4], const u64 Nd[4], u64 k0)
{
    u32 i, k, window_size;
    u32 ebits, table_size;
    void *table;     // precompute table
    u64 *pre_table;  // aligned precompute table
    u64 *tmp, trd[4];
    u32 index, off;
    u8 eb[33], mask;

    /* bignum to bytes */
    memset(eb, 0, 33);
    ll_to_bytes(eb + 1, NULL, ed, el);

    /* get window size */
    ebits = el * 64;
    assert(ebits > 0);
    window_size = LL_EXP_WINDOW_SIZE(ebits);
    assert(window_size < 8);
    mask = (1 << window_size) - 1;

    /* allocate precompute table */
    table_size = 1 << window_size;
    table = malloc(table_size * 32 + 16 + 32);
    pre_table = (u64*)LL_ALIGN_PTR(table, 16);

    /* precompute (ad^i)*R mod Nd for i = [0, 2^window_size - 1] */
    tmp = pre_table;
    ll_set_limb(tmp, 4, 1);                  // tmp = 1
    ll_u256_mont_mul(tmp, tmp, RR, Nd, k0);  // tmp = 1 (mont)
    for (k = 1; k < (1U << window_size); k++) {
        ll_u256_mont_mul(tmp + 4, tmp, ad, Nd, k0);
        tmp += 4;
    }

    tmp += 4;
    /* first window */
    i = ebits % window_size;
    index = eb[1] >> (8 - i);
    ll_u256_select(trd, pre_table, table_size, index);

    while (i < ebits) {
        /* get bits in window */
        i += window_size;
        off = (i - 1) / 8;
        index = eb[off] << 8 | eb[off + 1];
        index = (index >> ((ebits - i) % 8)) & mask;

        /* sqr */
        for (k = 0; k < window_size; k++)
            ll_u256_mont_sqr(trd, trd, Nd, k0);

        /* mul */
        ll_u256_select(tmp, pre_table, table_size, index);
        ll_u256_mont_mul(trd, trd, tmp, Nd, k0);
    }

    ll_copy_limbs(rd, trd, 4);
    free(table);
}

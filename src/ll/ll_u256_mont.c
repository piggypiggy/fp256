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
    (void) rd;
    (void) ad;
    (void) bd;
    (void) Nd;
    (void) k0;
    return;
}

void ll_u256_mont_sqr(u64 rd[4], const u64 ad[4], const u64 Nd[4], u64 k0)
{
    (void) rd;
    (void) ad;
    (void) Nd;
    (void) k0;
    return;
}

void ll_u256_mont_reduce(u64 rd[4], const u64 ad[4], const u64 Nd[4], u64 k0)
{
    (void) rd;
    (void) ad;
    (void) Nd;
    (void) k0;
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

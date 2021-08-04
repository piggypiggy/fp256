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

/* fill l = (nbits/64 + 1) limbs with random data, then clear higher bits of ad[l-1] */
int ll_rand_bits(u64 *rd, size_t nbits)
{
    int ret;
    size_t l, b;
    u64 mask;

    if (nbits == 0)
        return FP256_OK;

    l = ((nbits - 1) >> 6) + 1;
    b = nbits & 0x3f;
    mask = (1 << b) - 1;
    if (b == 0) // b = 64, mask = 2^64 - 1
        mask = (u64)-1;

    /* l limbs random data */
    ret = ll_rand_buf((u8*)rd, sizeof(u64) * l);
    /* reserve lower b bits of rd[l-1] */
    rd[l - 1] &= mask;

    return ret;
}

int ll_rand_bytes(u64 *rd, size_t nbytes)
{
    return ll_rand_bits(rd, 8 * nbytes);
}

int ll_rand_limbs(u64 *rd, size_t nlimbs)
{
    return ll_rand_bits(rd, 64 * nlimbs);
}

int ll_rand_range(u64 *rd, const u64 *range, size_t rgl)
{
    int ret, count;
    size_t nbits;

    ret = FP256_ERR;
    /* maximum number of try */
    count = 50;

    if (rgl == 0)
        return FP256_OK;

    /* 2^{nbits-1} <= range < 2^nbits */
    nbits = ll_num_bits(range[rgl - 1]) + (rgl - 1) * 64;

    while (count > 0) {
        count--;
        if (count <= 0) {
            ll_set_zero(rd, rgl);
            return FP256_ERR;
        }

        /* rd < 2^nbits */
        ret = ll_rand_bits(rd, nbits);
        if (ret != FP256_OK)
            return ret;

        /* rd < range */
        if (ll_cmp_limbs(rd, range, rgl, rgl) < 0)
            return FP256_OK;
    }

    return ret;
}

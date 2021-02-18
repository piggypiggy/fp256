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

#include <fp256/fp256.h>
#include <fp256/fp256_ll.h>

int fp256_rand_limbs(fp256 *r, size_t nlimbs)
{
    int ret;

    if (r == NULL || nlimbs > 4)
        return FP256_ERR;

    ll_set_zero(r->d, 4);
    ret = ll_rand_limbs(r->d, nlimbs);
    r->nlimbs = ll_num_limbs(r->d, 4);

    return ret;
}

int fp256_rand_bytes(fp256 *r, size_t nbytes)
{
    int ret;

    if (r == NULL || nbytes > 32)
        return FP256_ERR;

    ll_set_zero(r->d, 4);
    ret = ll_rand_bytes(r->d, nbytes);
    r->nlimbs = ll_num_limbs(r->d, 4);

    return ret;
}

int fp256_rand_bits(fp256 *r, size_t nbits)
{
    int ret;

    if (r == NULL || nbits > 256)
        return FP256_ERR;

    ll_set_zero(r->d, 4);
    ret = ll_rand_bits(r->d, nbits);
    r->nlimbs = ll_num_limbs(r->d, 4);

    return ret;
}

int fp256_rand_range(fp256 *r, const fp256 *range)
{
    int ret;
    u64 td[4];

    if (r == NULL || range == NULL)
        return FP256_ERR;

    ll_set_zero(td, 4);
    ret = ll_rand_range(td, range->d, range->nlimbs);
    fp256_set_limbs(r, td, 4);
    return ret;
}

int fp256_rand_init(void)
{
    u8 buf[16];

    /* just call ll_rand_buf() once */
    return ll_rand_buf(buf, 16);
}

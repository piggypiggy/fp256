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

/* r = a * b (lower 256 bits), TODO : optimize mullo */ 
int fp256_mullo(fp256 *r, const fp256 *a, const fp256 *b)
{
    u64 rd[8];

    if (r == NULL || a == NULL || b == NULL)
        return FP256_ERR;

    ll_u256_mullo(rd, a->d, b->d);

    fp256_set_limbs(r, rd, FP256_LIMBS);
    return FP256_OK;
}

/* r = a * b (upper 256 bits) */ 
int fp256_mulhi(fp256 *r, const fp256 *a, const fp256 *b)
{
    u64 rd[8];

    if (r == NULL || a == NULL || b == NULL)
        return FP256_ERR;

    ll_u256_mul(rd, a->d, b->d);

    fp256_set_limbs(r, rd + 4, FP256_LIMBS);
    return FP256_OK;
}

/* rhi:rlo = a * b */ 
int fp256_mul(fp256 *rhi, fp256 *rlo, const fp256 *a, const fp256 *b)
{
    u64 rd[8];

    if (rhi == NULL || rlo == NULL || a == NULL || b == NULL)
        return FP256_ERR;

    ll_u256_mul(rd, a->d, b->d);

    fp256_set_limbs(rlo, rd, FP256_LIMBS);
    fp256_set_limbs(rhi, rd + 4, FP256_LIMBS);
    return FP256_OK;
}

/* r = a^2 (lower 256 bits) */ 
int fp256_sqrlo(fp256 *r, const fp256 *a)
{
    u64 rd[8];

    if (r == NULL || a == NULL)
        return FP256_ERR;

    ll_u256_sqrlo(rd, a->d);

    fp256_set_limbs(r, rd, FP256_LIMBS);
    return FP256_OK;
}

/* r = a^2 (upper 256 bits) */ 
int fp256_sqrhi(fp256 *r, const fp256 *a)
{
    u64 rd[8];

    if (r == NULL || a == NULL)
        return FP256_ERR;

    ll_u256_sqr(rd, a->d);

    fp256_set_limbs(r, rd + 4, FP256_LIMBS);
    return FP256_OK;
}

/* rhi:rlo = a^2 */ 
int fp256_sqr(fp256 *rhi, fp256 *rlo, const fp256 *a)
{
    u64 rd[8];

    if (rhi == NULL || rlo == NULL || a == NULL)
        return FP256_ERR;

    ll_u256_sqr(rd, a->d);

    fp256_set_limbs(rlo, rd, FP256_LIMBS);
    fp256_set_limbs(rhi, rd + 4, FP256_LIMBS);
    return FP256_OK;
}

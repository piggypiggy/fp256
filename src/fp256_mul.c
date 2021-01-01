/******************************************************************************
 *                                                                            *
 * Copyright 2020-2021 Meng-Shan Jiang                                        *
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

/* r = a * b
 * Because a*b might be larger than 2^256 which does not fit into fp256, 
 * r only stores lower 256 bit result. Call ll_u256_mul or ll_u256_mul_limbs
 * if you need higher 256 bit.
 */ 
int fp256_mul(fp256 *r, const fp256 *a, const fp256 *b)
{
    u64 rd[8];

    if (r == NULL || a == NULL || b == NULL)
        return FP256_ERR;

    /* clear rd */
    rd[0] = 0ULL; rd[1] = 0ULL; rd[2] = 0ULL; rd[3] = 0ULL;
    rd[4] = 0ULL; rd[5] = 0ULL; rd[6] = 0ULL; rd[7] = 0ULL;

    if (a->nlimbs != 0 && b->nlimbs != 0)
        // ll_u256_mul_limbs(rd, a->d, b->d, a->nlimbs, b->nlimbs);
        ll_u256_mul(rd, a->d, b->d);

    fp256_set_limbs(r, rd, FP256_LIMBS, a->neg ^ b->neg);
    return FP256_OK;
}

int fp256_sqr(fp256 *r, const fp256 *a)
{
    u64 rd[8];

    if (r == NULL || a == NULL)
        return FP256_ERR;

    /* clear rd */
    rd[0] = 0ULL; rd[1] = 0ULL; rd[2] = 0ULL; rd[3] = 0ULL;
    rd[4] = 0ULL; rd[5] = 0ULL; rd[6] = 0ULL; rd[7] = 0ULL;

    ll_u256_sqr(rd, a->d);

    fp256_set_limbs(r, rd, FP256_LIMBS, 0);
    return FP256_OK;
}

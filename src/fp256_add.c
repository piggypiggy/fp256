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

int fp256_add(fp256 *r, const fp256 *a, const fp256 *b)
{
    if (r == NULL || a == NULL || b == NULL)
        return FP256_ERR;

    /* carry is discarded */
    ll_u256_add(r->d, a->d, b->d);
    r->nlimbs = fp256_num_limbs(r);

    return FP256_OK;
}

int fp256_sub(fp256 *r, const fp256 *a, const fp256 *b)
{
    if (r == NULL || a == NULL || b == NULL)
        return FP256_ERR;

    /* borrow is discarded */
    ll_u256_sub(r->d, a->d, b->d);
    r->nlimbs = fp256_num_limbs(r);

    return FP256_OK;
}

int fp256_add_limb(fp256 *r, const fp256 *a, const u64 b)
{
    if (r == NULL || a == NULL)
        return FP256_ERR;

    ll_u256_add_limb(r->d, a->d, b);
    r->nlimbs = fp256_num_limbs(r);

    return FP256_OK;
}

int fp256_sub_limb(fp256 *r, const fp256 *a, const u64 b)
{
    if (r == NULL || a == NULL)
        return FP256_ERR;

    ll_u256_sub_limb(r->d, a->d, b);
    r->nlimbs = fp256_num_limbs(r);

    return FP256_OK;
}

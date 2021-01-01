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

int fp256_add(fp256 *r, const fp256 *a, const fp256 *b)
{
    if (r == NULL || a == NULL || b == NULL)
        return FP256_ERR;

    if (a->neg == b->neg) {
        /* the 257's bit of r is discarded */
        ll_u256_add(r->d, a->d, b->d);
        r->neg = a->neg;
        r->nlimbs = fp256_num_limbs(r);
    }
    else {
        if (fp256_cmp_abs(a, b) >= 0) {
            ll_u256_sub(r->d, a->d, b->d);
            r->neg = a->neg;
            r->nlimbs = fp256_num_limbs(r);
        }
        else {
            ll_u256_sub(r->d, b->d, a->d);
            r->neg = b->neg;
            r->nlimbs = fp256_num_limbs(r);
        }
    }

    return FP256_OK;
}

int fp256_sub(fp256 *r, const fp256 *a, const fp256 *b)
{
    if (r == NULL || a == NULL || b == NULL)
        return FP256_ERR;

    if (a->neg != b->neg) {
        /* the 257's bit of r is discarded */
        ll_u256_add(r->d, a->d, b->d);
        r->neg = a->neg;
        r->nlimbs = fp256_num_limbs(r);
    }
    else {
        if (fp256_cmp_abs(a, b) >= 0) {
            ll_u256_sub(r->d, a->d, b->d);
            r->neg = a->neg;
            r->nlimbs = fp256_num_limbs(r);
        }
        else {
            ll_u256_sub(r->d, b->d, a->d);
            r->neg = a->neg ^ 1;
            r->nlimbs = fp256_num_limbs(r);
        }
    }

    return FP256_OK;
}

int fp256_add_limb(fp256 *r, const fp256 *a, const u64 b)
{
    fp256 fb;
    fp256_set_limb(&fb, b);
    return fp256_add(r, a, &fb);
}

int fp256_sub_limb(fp256 *r, const fp256 *a, const u64 b)
{
    fp256 fb;
    fp256_set_limb(&fb, b);
    return fp256_sub(r, a, &fb);
}

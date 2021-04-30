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

#if 0
/* euclid algorithm */
int fp256_gcd(fp256 *r, const fp256 *a, const fp256 *b)
{
    fp256 tmpa, tmpb, *pa, *pb, *pt;

    /* a = 0 or b = 0 */
    if (fp256_is_zero(a) || fp256_is_zero(b)) {
        fp256_set_zero(r);
        return FP256_OK;
    }

    /* if a < b, swap a, b */
    if (fp256_cmp(a, b) < 0) {
        fp256_copy(&tmpa, b);
        fp256_copy(&tmpb, a);
    }
    else {
        fp256_copy(&tmpa, a);
        fp256_copy(&tmpb, b);
    }

    /* euclid algorithm */
    pa = &tmpa; pb = &tmpb;
    do {
        fp256_div(pa, NULL, pa, pb);
        pt = pa;
        pa = pb;
        pb = pt;
    } while(fp256_is_zero(pt) == 0);

    fp256_copy(r, pa);
    return FP256_OK;
}
#else
int fp256_gcd(fp256 *r, const fp256 *a, const fp256 *b)
{
    size_t gl;
    u64 gcd[4];

    if ((fp256_is_zero(a) == 1) || (fp256_is_zero(b) == 1)) {
        fp256_set_zero(r);
        return FP256_OK;
    }

    gl = ll_lehmer_exgcd(gcd, NULL, NULL, NULL, NULL, a->d, b->d, a->nlimbs, b->nlimbs, 0);
    fp256_set_limbs(r, gcd, gl);
    return FP256_OK;
}
#endif

int fp256_is_coprime(const fp256 *a, const fp256 *b)
{
    fp256 r;

    if (a == NULL || b == NULL)
        return -1;

    if (fp256_gcd(&r, a, b) != FP256_OK)
        return -1;

    return fp256_is_one(&r);
}

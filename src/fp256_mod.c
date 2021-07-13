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

int fp256_mod(fp256 *r, const fp256 *a, const fp256 *m)
{
    return fp256_div(r, NULL, a, m);
}

int fp256_mod_neg(fp256 *r, const fp256 *a, const fp256 *m)
{
    if (fp256_div(r, NULL, a, m) != FP256_OK)
        return FP256_ERR;

    return fp256_sub(r, m, r);
}

int fp256_mod_add(fp256 *r, const fp256 *a, const fp256 *b, const fp256 *m)
{
    u64 rd[5], remd[4];
    size_t rl, reml;

    // memory sanitizer...
    // rd[0] = 0ULL; rd[1] = 0ULL; rd[2] = 0ULL; rd[3] = 0ULL; rd[4] = 0ULL;
    rd[4] = ll_u256_add(rd, a->d, b->d);
    rl = ll_num_limbs(rd, 5);

    if (ll_div(remd, NULL, &reml, NULL, rd, m->d, rl, m->nlimbs) != FP256_OK)
        return FP256_ERR;

    fp256_set_limbs(r, remd, reml);

    return FP256_OK;
}

int fp256_mod_sub(fp256 *r, const fp256 *a, const fp256 *b, const fp256 *m)
{
    int neg;
    u64 rd[5], remd[4];
    size_t rl, reml;

    // memory sanitizer...
    // rd[0] = 0ULL; rd[1] = 0ULL; rd[2] = 0ULL; rd[3] = 0ULL; rd[4] = 0ULL;
    if (fp256_cmp(a, b) >= 0) {
        /* a >= b */
        neg = 0;
        ll_u256_sub(rd, a->d, b->d);
    }
    else {
        /* a < b */
        neg = 1;
        ll_u256_sub(rd, b->d, a->d);
    }
    rl = ll_num_limbs(rd, 4);

    if (ll_div(remd, NULL, &reml, NULL, rd, m->d, rl, m->nlimbs) != FP256_OK)
        return FP256_ERR;

    fp256_set_limbs(r, remd, reml);
    if (neg && reml)
        fp256_sub(r, m, r);

    return FP256_OK;
}

int fp256_mod_mul(fp256 *r, const fp256 *a, const fp256 *b, const fp256 *m)
{
    size_t rl, reml;
    u64 rd[8];
    u64 remd[4];

    ll_set_zero(rd, 8);

    /* rd = a->d * b->d */
    ll_u256_mul(rd, a->d, b->d);
    rl = ll_num_limbs(rd, 8);

    /* remd = rd mod md */
    if (ll_div(remd, NULL, &reml, NULL, rd, m->d, rl, m->nlimbs) != FP256_OK)
        return FP256_ERR;

    fp256_set_limbs(r, remd, reml);

    return FP256_OK;
}

int fp256_mod_sqr(fp256 *r, const fp256 *a, const fp256 *m)
{
    size_t rl, reml;
    u64 rd[8];
    u64 remd[4];

    ll_set_zero(rd, 8);

    /* rd = (a->d) ^ 2 */
    ll_u256_sqr(rd, a->d);
    rl = ll_num_limbs(rd, 8);

    /* remd = rd mod md */
    if (ll_div(remd, NULL, &reml, NULL, rd, m->d, rl, m->nlimbs) != FP256_OK)
        return FP256_ERR;

    fp256_set_limbs(r, remd, reml);
    return FP256_OK;
}

int fp256_mod_inv(fp256 *r, const fp256 *a, const fp256 *m)
{
    ssize_t sl;
    u64 sd[5];

    if (fp256_is_zero(a) || fp256_is_zero(m))
        return FP256_ERR;

    ll_lehmer_exgcd(NULL, sd, NULL, &sl, NULL, a->d, m->d, a->nlimbs, m->nlimbs, 1);

    if (sl < 0) {
        fp256_set_limbs(r, sd, -sl);
        fp256_sub(r, m, r);
    }
    else
        fp256_set_limbs(r, sd, sl);

    return FP256_OK;
}

int fp256_mod_exp(fp256 *r, const fp256 *a, const fp256 *e, const fp256 *m)
{
    fp256 A, R;
    mont_ctx mctx;

    if (fp256_is_odd(m)) {
        /* odd modulus */
        fp256_mont_ctx_init(&mctx, 4, m);

        /* A = a % m */
        if (fp256_cmp(a, m) >= 0)
            fp256_mod(&A, a, m);
        else
            fp256_copy(&A, a);

        fp256_to_mont(&A, &A, &mctx);
        fp256_mont_exp(&R, &A, e, &mctx);
        fp256_from_mont(r, &R, &mctx);
    }
    else {
        /* TODO : even modulus */
        
    }

    return FP256_OK;
}

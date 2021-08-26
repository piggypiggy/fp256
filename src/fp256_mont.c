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

int fp256_mont_ctx_init(mont_ctx *mctx, size_t w, const fp256 *N)
{
    u64 RRN[4], tmp[9];
    size_t RRNl;

    if (mctx == NULL || N == NULL || w == 0 || w >= 5)
        return FP256_ERR;

    if (fp256_is_even(N) == 1)
        return FP256_ERR;

    mctx->w = w;
    fp256_copy(&mctx->N, N);

    /* tmp = 2^(2 * 64 * w) */
    ll_clear_set_bit(tmp, 2 * 64 * w, 9);
    /* RRm = tmp mod N */
    ll_div(RRN, NULL, &RRNl, NULL, tmp, N->d, 9, N->nlimbs);
    fp256_set_limbs(&mctx->RR, RRN, RRNl);

#if 0
    ssize_t kl;
    /* tmp = 2^64 */
    ll_clear_set_bit(tmp, 64, 2);
    /* k0 = N^(-1) mod 2^64 */
    ll_lehmer_exgcd(NULL, &k0, NULL, &kl, NULL, N->d, tmp, 1, 2, 1);
    assert(kl == 1 || kl == -1);
    if (kl == 1) {
        /* k0 = 2^64 - k0 */
        k0 = (~k0) + 1;
    }
    mctx->k0 = k0;
#else
    /* inversion mod 2^64 */
    mctx->k0 = ll_invert_limb(N->d[0]);
#endif

    return FP256_OK;
}

int fp256_mont_mul(fp256 *r, const fp256 *A, const fp256 *B, const mont_ctx *mctx)
{
    u64 rd[4];

    if (r == NULL || A == NULL || B == NULL || mctx == NULL)
        return FP256_ERR;

    //rd[0] = 0ULL; rd[1] = 0ULL; rd[2] = 0ULL; rd[3] = 0ULL;
    ll_u256_mont_mul(rd, A->d, B->d, mctx->N.d, mctx->k0);
    fp256_set_limbs(r, rd, 4);

    return FP256_OK;
}

int fp256_mont_sqr(fp256 *r, const fp256 *A, const mont_ctx *mctx)
{
    u64 rd[4];

    if (r == NULL || A == NULL || mctx == NULL)
        return FP256_ERR;

    //rd[0] = 0ULL; rd[1] = 0ULL; rd[2] = 0ULL; rd[3] = 0ULL;
    ll_u256_mont_sqr(rd, A->d, mctx->N.d, mctx->k0);
    fp256_set_limbs(r, rd, 4);

    return FP256_OK;
}

int fp256_mont_exp(fp256 *r ,const fp256 *A, const fp256 *e, const mont_ctx *mctx)
{
    u64 rd[4];

    if (r == NULL || A == NULL || e == NULL || mctx == NULL)
        return FP256_ERR;

    ll_u256_mont_exp(rd, A->d, e->d, e->nlimbs, mctx->RR.d, mctx->N.d, mctx->k0);
    fp256_set_limbs(r, rd, 4);

    return FP256_OK;
}

int fp256_to_mont(fp256 *A, const fp256 *a, const mont_ctx *mctx)
{
    if (A == NULL || a == NULL || mctx == NULL)
        return FP256_ERR;

    return fp256_mont_mul(A, a, &mctx->RR, mctx);
}

int fp256_from_mont(fp256 *a, const fp256 *A, const mont_ctx *mctx)
{
    u64 rd[4];

    if (a == NULL || A == NULL || mctx == NULL)
        return FP256_ERR;

    ll_u256_mont_reduce(rd, A->d, mctx->N.d, mctx->k0);
    fp256_set_limbs(a, rd, 4);

    return FP256_OK;
}

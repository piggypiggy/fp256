/******************************************************************************
 *                                                                            *
 * Copyright 2020 Meng-Shan Jiang                                             *
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
    u64 RRN[4], tmp[9], k0;
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
    fp256_set_limbs(&mctx->RR, RRN, RRNl, 0);

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
    /* faster inversion mod 2^64 
     * k*N = 1 mod 2^n
     * => (k*N - 1)^2 = 0 mod 2^(2n)
     * => k * (2 - k*N) * N = 1 mod 2^(2n)
     */
    u64 N0;
    N0 = N->d[0];
    k0 = (((N0 + 2u) & 4u) << 1) + N0; /* k0 * N = 1 mod 2^4 */
    k0 *= (2 - k0 * N0); /* k0 * N = 1 mod 2^8 */
    k0 *= (2 - k0 * N0); /* k0 * N = 1 mod 2^16 */
    k0 *= (2 - k0 * N0); /* k0 * N = 1 mod 2^32 */
    k0 *= (2 - k0 * N0); /* k0 * N = 1 mod 2^64 */
    k0 = (~k0) + 1; /* -k0 mod 2^64 */
    mctx->k0 = k0;

#endif

    return FP256_OK;
}

int fp256_mont_mul(fp256 *r, const fp256 *a, const fp256 *b, const mont_ctx *mctx)
{
    u64 rd[4];

    if (r == NULL || a == NULL || b == NULL || mctx == NULL)
        return FP256_ERR;

    ll_u256_mont_mul(rd, a->d, b->d, mctx->N.d, mctx->k0);
    fp256_set_limbs(r, rd, 4, 0);

    return FP256_OK;
}

int fp256_mont_sqr(fp256 *r, const fp256 *a, const mont_ctx *mctx)
{
    u64 rd[4];

    if (r == NULL || a == NULL || mctx == NULL)
        return FP256_ERR;

    ll_u256_mont_sqr(rd, a->d, mctx->N.d, mctx->k0);
    fp256_set_limbs(r, rd, 4, 0);

    return FP256_OK;
}

int fp256_to_mont(fp256 *r, const fp256 *a, const mont_ctx *mctx)
{
    if (r == NULL || a == NULL || mctx == NULL)
        return FP256_ERR;

    return fp256_mont_mul(r, a, &mctx->RR, mctx);
}

int fp256_from_mont(fp256 *r, const fp256 *a, const mont_ctx *mctx)
{
    fp256 one;

    if (r == NULL || a == NULL || mctx == NULL)
        return FP256_ERR;

    fp256_set_one(&one);
    return fp256_mont_mul(r, a, &one, mctx);
}

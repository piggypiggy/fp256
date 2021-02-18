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

#ifndef USE_ASM_U256_DIV
void ll_naive_div_1_limb(u64 *rd, u64 *qd, const u64 *nd, const u64 *dd, const size_t nl)
{
    (void) rd;
    (void) qd;
    (void) nd;
    (void) dd;
    (void) nl;
    return;
}

void ll_naive_div_2_limbs(u64 *rd, u64 *qd, const u64 *nd, const u64 *dd, const size_t nl)
{
    (void) rd;
    (void) qd;
    (void) nd;
    (void) dd;
    (void) nl;
    return;
}

void ll_naive_div_3_limbs(u64 *rd, u64 *qd, const u64 *nd, const u64 *dd, const size_t nl)
{
    (void) rd;
    (void) qd;
    (void) nd;
    (void) dd;
    (void) nl;
    return;
}

void ll_naive_div_4_limbs(u64 *rd, u64 *qd, const u64 *nd, const u64 *dd, const size_t nl)
{
    (void) rd;
    (void) qd;
    (void) nd;
    (void) dd;
    (void) nl;
    return;
}
#endif

int ll_naive_div(u64 *rd, u64 *qd, size_t *rl, size_t *ql, const u64 *nd, const u64 *dd, size_t nl, size_t dl)
{
    size_t shift, nl1, tnl, t;
    u64 *tmp, *trd, *tqd, *tnd, *tdd;

    nl = ll_num_limbs(nd, nl);
    dl = ll_num_limbs(dd, dl);

    /* ll_naive_div does not support divisor >= 2^256 */
    if (dl > 4 || dl == 0)
        return FP256_ERR;

    nl1 = (nl > dl ? nl : dl);
    nl1++;
    tmp = (u64*)calloc(1, sizeof(u64) * 4 * nl1);
    trd = tmp;
    tqd = tmp + nl1;

    ll_set_zero(trd, nl1);
    ll_set_zero(tqd, nl1);

    if (ll_cmp_limbs(nd, dd, nl, dl) < 0) {
        /* if nd < dd, rd = nd and qd = 0 */
        ll_copy_limbs(trd, nd, nl);
        ll_set_zero(tqd, nl);
        goto end;
    }

    assert(nl >= dl);
    if (dl == 1)
        ll_naive_div_1_limb(trd, tqd, nd, dd, nl);
    else {
        /* left shift div so that the most significant bit of dd[dl-1] is 1, 
         * left shift num by the same number.
         */
        shift = 64 - ll_num_bits(dd[dl - 1]);
        tnd = tmp + 2 * nl1;
        tdd = tmp + 3 * nl1;
        /* */
        t = ll_lshift(tnd, nd, nl, shift);
        tnl = nl + (t != 0);
        ll_lshift(tdd, dd, dl, shift);

        if (dl == 4)
            ll_naive_div_4_limbs(trd, tqd, tnd, tdd, tnl);
        else if (dl == 3)
            ll_naive_div_3_limbs(trd, tqd, tnd, tdd, tnl);
        else
            ll_naive_div_2_limbs(trd, tqd, tnd, tdd, tnl);

        ll_rshift(trd, trd, nl1, shift);
    }

end:
    if (rd != NULL)
        ll_copy_limbs(rd, trd, dl);
    if (rl != NULL) 
        *rl = ll_num_limbs(trd, dl);

    if (qd != NULL)
        /* if nl >= dl, qd has at most nl + 1 - dl limbs,
         * if nl < dl, qd has exactly nl limbs.
         */
        ll_copy_limbs(qd, tqd, (nl >= dl ? (nl + 1 - dl) : nl));
    if (ql != NULL)
        *ql = ll_num_limbs(tqd, nl);

    free(tmp);
    return FP256_OK;
}

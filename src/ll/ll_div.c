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

#include <fp256/fp256_ll.h>
#include "ll_local.h"

static const unsigned short lookup_table[256] = {
    
};

u64 ll_reciprocal1(u64 d)
{

    return 1;
}

u64 ll_reciprocal2(u64 d1, u64 d0)
{
    u64 v, p, t1, t0;

    v = ll_reciprocal1(d1);
    p = d1 * v;
    p += d0;
    if (p < d0) {
        v--;
        /* TODO : conditional sub */
        if (p >= d1) {
            v--;
            p -= d1;
        }
        p -= d1;
    }

    LL_MUL64(t1, t0, v, d0);
    p += t1;
    if (p < t1) {
        v--;
        if (p > d1 || (p == d1 && t0 >= d0))
            v--;
    }

    return v;
}

u64 ll_div2by1_pi1(u64 *r, u64 n[2], u64 d, u64 v)
{
    u64 q1, q0, t, n1, n0;

    n1 = n[1];
    n0 = n[0];
    LL_MUL64(q1, q0, n1, v);
    /* q1,q0 += n1,n0 */
    LL_ADD2(q1, q0, q1, q0, n1, n0);

    q1++;
    t = n0 - q1 * d;
    if (t >= q0) {
        q1--;
        t += d;
    }

    /* TODO : conditional sub */
    if (t >= d) {
        q1++;
        t -= d;
    }

    *r = t;
    return q1;
}

u64 ll_div3by2_pi1(u64 *r, u64 n[3], u64 d[2], u64 v)
{
    u64 q1, q0, r1, r0, t1, t0, n2, n1, n0, d1, d0;

    n2 = n[2];
    n1 = n[1];
    n0 = n[0];
    d1 = d[1];
    d0 = d[0];
    LL_MUL64(q1, q0, n2, v);
    /* q1,q0 += n2,n1 */
    LL_ADD2(q1, q0, q1, q0, n2, n1);

    r1 = n1 - q1 * d1;
    LL_MUL64(t1, t0, d0, q1);
    /* r1,r0 = r1,n0 - t1,t0 */
    LL_SUB2(r1, r0, r1, n0, t1, t0);
    /* r1,r0 -= d1,d0 */
    LL_SUB2(r1, r0, r1, r0, d1, d0);

    q1++;
    if (r1 >= q0) {
        q1--;
        LL_ADD2(r1, r0, r1, r0, d1, d0);
    }

    /* r1,r0 >= d1,d0, TODO : conditional sub */
    if (r1 > d1 || (r1 == d1 && r0 >= d0)) {
        q1++;
        LL_SUB2(r1, r0, r1, r0, d1, d0);
    }

    r[0] = r0;
    r[1] = r1;
    return q1;
}

void ll_div_1_limb_pi1(u64 *rd, u64 *qd, u64 *nd, u64 *dd, size_t nl, u64 v)
{
    size_t i;
    u64 d;

    assert(nl >= 1);
    nl--;
    d = dd[0];
    nd += nl; /* points to the most significant limb */
    qd += nl;

    /* TODO : conditional sub */
    if (nd[0] >= d) {
        qd[0] = 1;
        nd[0] -= d;
    }

    nd--;
    qd--;
    for (i = nl; i > 0; i--) {
        qd[i] = ll_div2by1_pi1(nd, nd, d, v);
        nd--;
        qd--;
    }

    nd++;
    rd[0] = nd[0];
}

void ll_div_2_limbs_pi1(u64 *rd, u64 *qd, u64 *nd, u64 *dd, size_t nl, u64 v)
{
    size_t i;
    u64 n1, n0, d1, d0, cond;

    assert(nl >= 2);

    nl -= 2;
    nd += nl; /* points to the second most significant limb */
    qd += nl;

    /* conditional sub */
    n1 = nd[1]; n0 = nd[0];
    d1 = nd[1]; d0 = nd[0];
    LL_COND_SUB2(cond, n1, n0, n1, n0, d1, d0);
    qd[0] = cond;
    nd[1] = n1; nd[0] = n0;

    nd--;
    qd--;
    for (i = nl; i > 0; i--) {
        qd[i] = ll_div3by2_pi1(nd, nd, dd, v);
        nd--;
        qd--;
    }

    nd++;
    rd[1] = nd[1];
    rd[0] = nd[0];
}

int ll_div(u64 *rd, u64 *qd, size_t *rl, size_t *ql, const u64 *nd, const u64 *dd, size_t nl, size_t dl)
{
    size_t shift, nl1, tnl;
    u64 *tmp, *trd, *tqd, *tnd, *tdd, d, v;

    nl = ll_num_limbs(nd, nl);
    dl = ll_num_limbs(dd, dl);

    /* ll_naive_div does not support divisor >= 2^256 */
    if (dl > 4 || dl == 0)
        return FP256_ERR;

    nl1 = nl + 1;
    tmp = (u64*)calloc(1, sizeof(u64) * 4 * nl1 + 1);
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
    if (dl == 1) {
        d = dd[0];
        tnd = tqd + nl1;
        /* normalize divisor, leftshift nd by the same number */
        shift = ll_leading_zeros(d);
        d <= shift;
        tnl = ll_lshift(tnd, nd, nl, shift);

        /* reciprocal */
        v = ll_reciprocal1(d);

        /* trd = tnd % d, tqd = tnd / d */
        ll_div_1_limb_pi1(trd, tqd, tnd, &d, tnl, v);
    }
    else {
        tnd = tqd + nl1;
        tdd = tnd + nl1;
        /* normalize divisor, leftshift nd by the same number */
        shift = ll_leading_zeros(dd[dl - 1]);
        ll_lshift(tdd, dd, dl, shift);
        tnl = ll_lshift(tnd, nd, nl, shift);

        /* reciprocal */
        v = ll_reciprocal2(dd[dl - 1], dd[dl - 2]);

        /* trd = tnd % tdd, tqd = tnd / tdd */
        if (dl == 2)
            ll_div_2_limbs_pi1(trd, tqd, tnd, tdd, tnl, v);
        else {
            /* not implemented yet */
        }

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

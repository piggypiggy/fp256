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

#ifndef USE_ASM_DIV
/** lookup_table[i] = floor(0x7fd00 / (256 + i)) */
static const unsigned short lookup_table[256] = {
    0x7fd, 0x7f5, 0x7ed, 0x7e5, 0x7dd, 0x7d5, 0x7ce, 0x7c6, 
    0x7bf, 0x7b7, 0x7b0, 0x7a8, 0x7a1, 0x79a, 0x792, 0x78b, 
    0x784, 0x77d, 0x776, 0x76f, 0x768, 0x761, 0x75b, 0x754, 
    0x74d, 0x747, 0x740, 0x739, 0x733, 0x72c, 0x726, 0x720, 
    0x719, 0x713, 0x70d, 0x707, 0x700, 0x6fa, 0x6f4, 0x6ee, 
    0x6e8, 0x6e2, 0x6dc, 0x6d6, 0x6d1, 0x6cb, 0x6c5, 0x6bf, 
    0x6ba, 0x6b4, 0x6ae, 0x6a9, 0x6a3, 0x69e, 0x698, 0x693, 
    0x68d, 0x688, 0x683, 0x67d, 0x678, 0x673, 0x66e, 0x669, 
    0x664, 0x65e, 0x659, 0x654, 0x64f, 0x64a, 0x645, 0x640, 
    0x63c, 0x637, 0x632, 0x62d, 0x628, 0x624, 0x61f, 0x61a, 
    0x616, 0x611, 0x60c, 0x608, 0x603, 0x5ff, 0x5fa, 0x5f6, 
    0x5f1, 0x5ed, 0x5e9, 0x5e4, 0x5e0, 0x5dc, 0x5d7, 0x5d3, 
    0x5cf, 0x5cb, 0x5c6, 0x5c2, 0x5be, 0x5ba, 0x5b6, 0x5b2, 
    0x5ae, 0x5aa, 0x5a6, 0x5a2, 0x59e, 0x59a, 0x596, 0x592, 
    0x58e, 0x58a, 0x586, 0x583, 0x57f, 0x57b, 0x577, 0x574, 
    0x570, 0x56c, 0x568, 0x565, 0x561, 0x55e, 0x55a, 0x556, 
    0x553, 0x54f, 0x54c, 0x548, 0x545, 0x541, 0x53e, 0x53a, 
    0x537, 0x534, 0x530, 0x52d, 0x52a, 0x526, 0x523, 0x520, 
    0x51c, 0x519, 0x516, 0x513, 0x50f, 0x50c, 0x509, 0x506, 
    0x503, 0x500, 0x4fc, 0x4f9, 0x4f6, 0x4f3, 0x4f0, 0x4ed, 
    0x4ea, 0x4e7, 0x4e4, 0x4e1, 0x4de, 0x4db, 0x4d8, 0x4d5, 
    0x4d2, 0x4cf, 0x4cc, 0x4ca, 0x4c7, 0x4c4, 0x4c1, 0x4be, 
    0x4bb, 0x4b9, 0x4b6, 0x4b3, 0x4b0, 0x4ad, 0x4ab, 0x4a8, 
    0x4a5, 0x4a3, 0x4a0, 0x49d, 0x49b, 0x498, 0x495, 0x493, 
    0x490, 0x48d, 0x48b, 0x488, 0x486, 0x483, 0x481, 0x47e, 
    0x47c, 0x479, 0x477, 0x474, 0x472, 0x46f, 0x46d, 0x46a, 
    0x468, 0x465, 0x463, 0x461, 0x45e, 0x45c, 0x459, 0x457, 
    0x455, 0x452, 0x450, 0x44e, 0x44b, 0x449, 0x447, 0x444, 
    0x442, 0x440, 0x43e, 0x43b, 0x439, 0x437, 0x435, 0x432, 
    0x430, 0x42e, 0x42c, 0x42a, 0x428, 0x425, 0x423, 0x421, 
    0x41f, 0x41d, 0x41b, 0x419, 0x417, 0x414, 0x412, 0x410, 
    0x40e, 0x40c, 0x40a, 0x408, 0x406, 0x404, 0x402, 0x400,
};

u64 ll_reciprocal1(u64 d)
{
    u64 v0, v1, v2, v3, v4, d0, d9, d40, d63, e;

    d0 = d & 1;
    d9 = d >> 55;
    d40 = (d >> 24) + 1;
    v0 = lookup_table[d9 - 256];

    /* v1 = (v0 << 11) - ((v0^2 * d40) >> 40) - 1 */
    v1 = v0 << 11;
    v0 = v0 * v0 * d40;
    v0 >>= 40;
    v1--;
    v1 -= v0;

    /* v2 = (v1 << 13) - ((v1 * (2^60 - v1 * d40)) >> 47) */
    v0 = 0x1000000000000000ULL;
    v2 = v1 << 13;
    v0 -= (v1 * d40);
    v1 *= v0;
    v2 += (v1 >> 47);

    /* e = 2^96 - v2 * d63 + (v2 >> 1) * d0 */
    d63 = (d >> 1) + d0;
    e = (v2 >> 1) & (-d0);
    e -= v2 * d63;

    /* v3 = ((v2 << 31) + ((v2 * e) >> 65)) mod 2^64 */
    v3 = (v2 << 31);
    LL_MUL64(v1, v0, v2, e);
    v3 += (v1 >> 1);

    /* v4 = (v3 - (((v3 + 2^64 + 1) * d) >> 64)) mod 2^64 */
    LL_MUL64(v1, v0, v3, d);
    LL_ADD2(v1, v0, v1, v0, d, d);
    v4 = v3 - v1;

    return v4;
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
        /* p,t0 >= d1,d0 */
        if (p > d1 || (p == d1 && t0 >= d0))
            v--;
    }

    return v;
}

u64 ll_div2by1_pi1(u64 *r, const u64 n[2], u64 d, u64 v)
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

    r[0] = t;
    r[1] = 0;
    return q1;
}

u64 ll_div3by2_pi1(u64 *r, const u64 n[3], const u64 d[2], u64 v)
{
    u64 q1, q0, r1, r0, t1, t0, n2, n1, n0, d1, d0;

    n2 = n[2];
    n1 = n[1];
    n0 = n[0];
    d1 = d[1];
    d0 = d[0];
    LL_MUL64(q1, q0, v, n2);
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
    r[2] = 0;
    return q1;
}
#endif

void ll_div_1_limb_pi1(u64 *qd, u64 *nd, u64 *dd, size_t nl, u64 v)
{
    size_t i;
    u64 d;

    assert(nl >= 1);
    nl--;
    d = dd[0];
    nd += nl; /* points to the most significant limb */
    qd += nl;
    qd[0] = 0;

    /* TODO : conditional sub */
    if (nd[0] >= d) {
        qd[0] = 1;
        nd[0] -= d;
    }

    nd--;
    qd--;
    for (i = nl; i > 0; i--) {
        qd[0] = ll_div2by1_pi1(nd, nd, d, v);
        nd--;
        qd--;
    }
}

void ll_div_2_limbs_pi1(u64 *qd, u64 *nd, u64 *dd, size_t nl, u64 v)
{
    size_t i;
    u64 n1, n0, d1, d0, cmp;

    assert(nl >= 2);

    nl -= 2;
    nd += nl; /* points to the second most significant limb */
    qd += nl;

    /* conditional sub */
    n1 = nd[1]; n0 = nd[0];
    d1 = dd[1]; d0 = dd[0];
    LL_CMP_SUB2(cmp, n1, n0, n1, n0, d1, d0);
    // if (n1 > d1 || (n1 == d1 && n0 >= d0)) {
    //     qd[0] = 1;
    //     LL_SUB2(n1, n0, n1, n0, d1, d0);
    // }
    qd[0] = cmp;
    nd[1] = n1; nd[0] = n0;

    nd--;
    qd--;
    for (i = nl; i > 0; i--) {
        qd[0] = ll_div3by2_pi1(nd, nd, dd, v);
        nd--;
        qd--;
    }
}

void ll_div_n_limbs_pi1(u64 *qd, u64 *nd, u64 *dd, size_t nl, size_t dl, u64 v)
{
    size_t i;
    u64 d1, d0, borrow, q;
    u64 *dd2;

    assert((nl >= dl) && (dl >= 3));
    nl -= dl;
    qd += nl;
    nd += nl;
    qd[0] = 0;
    if (ll_cmp_limbs(nd, dd, dl, dl) >= 0) {
        ll_sub_limbs(nd, nd, dd, dl);
        qd[0] = 1;
    }

    dl -= 2;
    dd2 = dd + dl;
    nd += (dl - 1);
    qd--;
    d1 = dd2[1]; d0 = dd2[0];
    for (i = nl; i > 0; i--) {
        if (nd[2] == d1 && nd[1] == d0) {
            q = -1ULL;
            ll_mulsub_limb(nd - dl, dd, q, dl + 2, dl + 2);
        }
        else {
            /* divrem((nd[2],nd[1],nd[0]), (d1,d0)) */
            q = ll_div3by2_pi1(nd, nd, dd2, v);
            /* nd[1],nd[0],...,nd[-dl] -= (q * (dd[dl-1],dd[dl-2],...dd[0])) */
            borrow = ll_mulsub_limb(nd - dl, dd, q, dl + 2, dl);

            if (borrow) {
                assert(borrow == 1);
                /* q is 1 larger than the real quotient */
                ll_add_limbs(nd - dl, nd - dl, dd, dl + 2);
                q--;
            }
        }

        qd[0] = q;
        nd--;
        qd--;
    }
}

int ll_div(u64 *rd, u64 *qd, size_t *rl, size_t *ql, const u64 *nd, const u64 *dd, size_t nl, size_t dl)
{
    size_t shift, nl1, tnl;
    u64 *tmp, *tqd, *tnd, *tdd, d, v, t;

    assert(dl > 0);
    nl = ll_num_limbs(nd, nl);
    dl = ll_num_limbs(dd, dl);

    if (ll_cmp_limbs(nd, dd, nl, dl) < 0) {
        /* if nd < dd, rd = nd and qd = 0 */
        if (rd != NULL)
            ll_copy_limbs(rd, nd, nl);
        if (qd != NULL)
            ll_set_zero(qd, nl);
        if (rl != NULL) 
            *rl = nl;
        if (ql != NULL) 
            *ql = 0;
        return FP256_OK;
    }

    nl1 = (nl > dl ? nl : dl);
    nl1++;
    tmp = (u64*)malloc(sizeof(u64) * 4 * nl1);
    tqd = tmp;
    tnd = tqd + nl1;

    assert(nl >= dl);
    if (dl == 1) {
        d = dd[0];
        /* normalize divisor, leftshift nd by the same number */
        shift = ll_leading_zeros(d);
        d <<= shift;
        t = ll_lshift(tnd, nd, nl, shift);
        tnl = nl + (t != 0);

        /* reciprocal */
        v = ll_reciprocal1(d);

        /* trd = tnd % d, tqd = tnd / d */
        ll_div_1_limb_pi1(tqd, tnd, &d, tnl, v);

        ll_rshift(tnd, tnd, nl1, shift);
    }
    else {
        /* give tnd 2*nl1 limbs */
        tdd = tnd + nl1 + nl1;
        /* normalize divisor, leftshift nd by the same number */
        shift = ll_leading_zeros(dd[dl - 1]);
        ll_lshift(tdd, dd, dl, shift);
        t = ll_lshift(tnd, nd, nl, shift);
        tnl = nl + (t != 0);

        /* reciprocal */
        v = ll_reciprocal2(tdd[dl - 1], tdd[dl - 2]);

        /* trd = tnd % tdd, tqd = tnd / tdd */
        if (dl == 2)
            ll_div_2_limbs_pi1(tqd, tnd, tdd, tnl, v);
        else
            ll_div_n_limbs_pi1(tqd, tnd, tdd, tnl, dl, v);

        ll_rshift(tnd, tnd, nl1, shift);
    }

    if (rd != NULL)
        ll_copy_limbs(rd, tnd, dl);
    if (rl != NULL) 
        *rl = ll_num_limbs(tnd, dl);

    if (qd != NULL)
        /* if nl >= dl, qd has at most nl + 1 - dl limbs */
        ll_copy_limbs(qd, tqd, nl + 1 - dl);
    if (ql != NULL)
        *ql = ll_num_limbs(tqd, nl + 1 - dl);

    free(tmp);
    return FP256_OK;
}

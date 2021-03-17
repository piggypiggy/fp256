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

size_t ll_lehmer_update_ab(u64 *t1d, u64 *t2d, const u64 *ad, const u64 *bd, size_t l, const u64 mat[4], u64 nc)
{
    size_t t1l, t2l;

    if (nc & 0x1ULL) {
        ll_mul_limb(t1d, bd, mat[1], l);
        ll_mulsub_limb(t1d, ad, mat[0], l + 1, l);
        ll_mul_limb(t2d, ad, mat[2], l);
        ll_mulsub_limb(t2d, bd, mat[3], l + 1, l);
    }
    else {
        ll_mul_limb(t1d, ad, mat[0], l);
        ll_mulsub_limb(t1d, bd, mat[1], l + 1, l);
        ll_mul_limb(t2d, bd, mat[3], l);
        ll_mulsub_limb(t2d, ad, mat[2], l + 1, l);
    }

    t1l = ll_num_limbs(t1d, l + 1);
    t2l = ll_num_limbs(t2d, l + 1);
    if (t1l > t2l)
        return t1l;
    else
        return t2l;
}

size_t ll_lehmer_update_v(u64 *t1d, u64 *t2d, const u64 *v0, const u64 *v1, size_t vl, const u64 mat[4])
{
    ll_mul_limb(t1d, v0, mat[0], vl);
    ll_muladd_limb(t1d, v1, mat[1], vl + 1, vl);
    ll_mul_limb(t2d, v0, mat[2], vl);
    ll_muladd_limb(t2d, v1, mat[3], vl + 1, vl);

    vl += ((t1d[vl] | t2d[vl]) > 0);
    // vl += ((t1d[vl] | t2d[vl]) > 0);
    return vl;
}

/* t2d = ad / bd
 * if extended, v0 += v1 * (ad / bd) 
 */
size_t ll_euclid_update(u64 *t1d, u64 *t2d, const u64 *ad, const u64 *bd, u64 *v0, u64 *v1, size_t l, size_t vl, int extended)
{
    size_t i, ql;

    ll_div(t1d, t2d, NULL, &ql, ad, bd, l, l);

    if (extended) {
        // v0 += v1 * qd
        for (i = 0; i < ql; i++)
            ll_muladd_limb(v0 + i, v1, t2d[i], vl, vl);

        vl = ll_num_limbs(v0, vl+l);
    }

    return vl;
}

#ifndef USE_ASM_LEHMER
/* q = a1:a0 / b1:b0,
 * a1:a0 = b1:b0,
 * b1:b0 = remainder.
 */
# define LL_DIV_22(q, a1, a0, b1, b0) do { \
    size_t __alz, __blz; \
    u64 __t1, __t0, __cmp; \
    __alz = ll_leading_zeros(a1); \
    __blz = ll_leading_zeros(b1); \
    __t1 = b1, __t0 = b0; \
    q = 0; \
    __blz -= __alz; \
    b1 = (b1 << __blz) | (b0 >> 1 >> (63 - __blz)); \
    b0 <<= __blz; \
    do { \
        q <<= 1; \
        LL_CMP_SUB2(__cmp, a1, a0, a1, a0, b1, b0); \
        q += __cmp; \
        b0 = (b1 << 63) | (b0 >> 1); \
        b1 >>= 1; \
    } while (__blz--); \
    b1 = a1; b0 = a0; \
    a1 = __t1; a0 = __t0; \
} while(0);

u64 ll_lehmer_exgcd11(u64 mat[4], u64 a0, u64 b0)
{
    u64 n; // number of matrix multiplication
    u64 t, q;
    u64 m00, m01, m10, m11;

    if (a0 >= b0) {
        m00 = 1; m01 = 0; m10 = 0; m11 = 1;
        n = 0;
    }
    else {
        /* if a0 < b0, swap a0 and b0 by a matrix multiplication, 
           i.e. [0, 1;1, 0]*[a0;b0] */
        m00 = 0; m01 = 1; m10 = 1; m11 = 0;
        n = 1;
        t = a0; a0 = b0; b0 = t;
    }

    while (b0 > 0) {
        q = a0 / b0;
        n++;
        t = b0;
        b0 = a0 - q * b0;
        a0 = t;
        /* mat = [0, 1:1, q] * mat */
        t = m00 + m10 * q;
        m00 = m10;
        m10 = t;
        t = m01 + m11 * q;
        m01 = m11;
        m11 = t;
    }

    mat[0] = m00;
    mat[1] = m01;
    mat[2] = m10;
    mat[3] = m11;
    return n;
}

u64 ll_lehmer_simulate(u64 mat[4], u64 a1, u64 a0, u64 b1, u64 b0)
{
    int cmp;
    u64 n; // number of matrix multiplication
    u64 t, q, thres;
    u64 r1, r0;
    u64 m00, m01, m10, m11;

    n = 0;
    thres = 0x100000000ULL;
    m00 = 1; m01 = 0; m10 = 0; m11 = 1;
    if (b1 == 0)
        goto Lehmer_done;

    LL_CMP2(cmp, a1, a0, b1, b0);
    if (cmp < 0) {
        /* if a0 < b0, swap a1:a0 and b1:b0 by a matrix multiplication, 
           i.e. [0, 1;1, 0]*[a1:a0;b1:b0] */
        m00 = 0; m01 = 1; m10 = 1; m11 = 0;
        n++;
        t = a1; a1 = b1; b1 = t;
        t = a0; a0 = b0; b0 = t;
    }

    while (a1 >= thres) {
        LL_SUB2(r1, r0, a1, a0, b1, b0);
        // a1:a0 > 2 * b1:b0
        if (r1 > b1) {
            LL_DIV_22(q, a1, a0, b1, b0);
            if (b1 == 0)
                goto Lehmer_done;

            n++;
            /* mat = [0, 1:1, q] * mat */
            t = m00 + m10 * q;
            m00 = m10;
            m10 = t;
            t = m01 + m11 * q;
            m01 = m11;
            m11 = t;
        }
        else {
            if (r1 == 0)
                goto Lehmer_done;

            n++;
            a1 = b1;
            a0 = b0;
            b1 = r1;
            b0 = r0;
            /* mat = [0, 1;1, 1] * mat */
            t = m00 + m10;
            m00 = m10;
            m10 = t;
            t = m01 + m11;
            m01 = m11;
            m11 = t;
        }
    }

    thres = 0x200000000ULL;
    a0 = (a1 << 32) | (a0 >> 32);
    b0 = (b1 << 32) | (b0 >> 32);

    while (1) {
        t = a0 - b0;
        if (t > b0) {
            q = a0 / b0;
            t = b0;
            b0 = a0 - q * b0;
            a0 = t;
            if (thres > b0)
                goto Lehmer_done;

            n++;
            /* mat = [0, 1:1, q] * mat */
            t = m00 + m10 * q;
            m00 = m10;
            m10 = t;
            t = m01 + m11 * q;
            m01 = m11;
            m11 = t;
        }
        else {
            if (thres > t)
                goto Lehmer_done;

            n++;
            a0 = b0;
            b0 = t;
            /* mat = [0, 1;1, 1] * mat */
            t = m00 + m10;
            m00 = m10;
            m10 = t;
            t = m01 + m11;
            m01 = m11;
            m11 = t;
        }
    }

Lehmer_done:
    mat[0] = m00;
    mat[1] = m01;
    mat[2] = m10;
    mat[3] = m11;
    return n;
}
#endif

size_t ll_lehmer_exgcd(u64 *gcd, u64 *sd, u64 *td, ssize_t *sl, ssize_t *tl, 
                       const u64 *ad, const u64 *bd, size_t al, size_t bl, int extended)
{
    size_t l, vl, tal, tbl, tsl, gl;
    u64 mat[4];
    u64 a1, a0, b1, b0, bits, shift;
    u64 nc, n;
    u64 *tmp, *tmp1, *tmp2, *tmp3, *tmp4, *t, *tad, *tbd, *v0, *v1, *tsd;
    ssize_t *tmpl;
    int swap;

    l = al > bl ? al : bl;
    l += 2;
    tmp = (u64*)calloc(8 + 2, sizeof(u64) * l);
    tmp1 = tmp;
    tmp2 = tmp1 + l;
    tmp3 = tmp2 + l;
    tmp4 = tmp3 + l;
    tad = tmp4 + l;
    tbd = tad + l;
    v0 = tbd + l;
    v1 = v0 + l;
    tsd = v1 + l;
    gl = 0;
    /* nc and n track number of matrix multiplication in simulate step */
    nc = 0; n = 0;
    /* v0 = 0, v1 = 1 */
    v1[0] = 1; vl = 1;
    tal = al; tbl = bl;
    ll_copy_limbs(tad, ad, al);
    ll_copy_limbs(tbd, bd, bl);

    /* make sure a >= b */
    swap = 0;
    if (ll_cmp_limbs(tad, tbd, al, bl) < 0) {
        t = tad; tad = tbd; tbd = t;
        tal = bl;
        tbl = al;
        swap = 1;
    }

    while (tbl >= 2) {
        /* extract most significant 128 bit of a and b */
        if ((tal - tbl) <= 1) {
            bits = ll_num_bits(tad[tal - 1] | tbd[tal - 1]);
            shift = 64 - bits;
            if (tal > 2 && shift > 0) {
                a1 = (tad[tal - 1] << shift) | (tad[tal - 2] >> bits);
                a0 = (tad[tal - 2] << shift) | (tad[tal - 3] >> bits);
                b1 = (tbd[tal - 1] << shift) | (tbd[tal - 2] >> bits);
                b0 = (tbd[tal - 2] << shift) | (tbd[tal - 3] >> bits);
            }
            else if (tal == 2 && shift > 0) {
                a1 = (tad[1] << shift) | (tad[0] >> bits);
                a0 =  tad[0] << shift;
                b1 = (tbd[1] << shift) | (tbd[0] >> bits);
                b0 =  tbd[0] << shift;
            }
            else {
                a1 = tad[tal - 1];
                a0 = tad[tal - 2];
                b1 = tbd[tal - 1];
                b0 = tbd[tal - 2];
            }
        }
        else {
            a1 = 0;
            a0 = 0;
            b1 = 0;
            b0 = 0;
        }

        n = ll_lehmer_simulate(mat, a1, a0, b1, b0);
        if (n > 0) {
            nc += n;
            tal = ll_lehmer_update_ab(tmp1, tmp2, tad, tbd, tal, mat, n);
            t = tad; tad = tmp1; tmp1 = t;
            t = tbd; tbd = tmp2; tmp2 = t;
            tbl = ll_num_limbs(tbd, tal);

            if (extended) {
                vl = ll_lehmer_update_v(tmp3, tmp4, v0, v1, vl, mat);
                t = v0; v0 = tmp3; tmp3 = t;
                t = v1; v1 = tmp4; tmp4 = t;
            }
        }
        else {
            nc++;
            vl = ll_euclid_update(tmp1, tmp2, tad, tbd, v0, v1, tal, vl, extended);
            t = tad; tad = tbd; tbd = tmp1; tmp1 = t;
            tal = tbl;
            tbl = ll_num_limbs(tbd, tbl);

            if (extended) {
                t = v0; v0 = v1; v1 = t;
            }
        }
    }
    assert(tbl <= 1);

    /* if tbd = 0, iteration is over */
    if (tbl > 0) {
        if (tal > 1) {
            nc++;
            vl = ll_euclid_update(tmp1, tmp2, tad, tbd, v0, v1, tal, vl, extended);
            t = tad; tad = tbd; tbd = tmp1; tmp1 = t;
            tal = tbl;
            tbl = ll_num_limbs(tbd, tbl);

            if (extended) {
                t = v0; v0 = v1; v1 = t;
            }
        }
        assert(tbl <= 1);

        n = ll_lehmer_exgcd11(mat, tad[0], tbd[0]);
        nc += n;
    }

    /* we need gcd(a, b) to calculate sd */
    if (gcd != NULL || sd != NULL) {
        gl = ll_lehmer_update_ab(tmp1, tmp2, tad, tbd, tal, mat, n);
        if (gcd != NULL)
            ll_copy_limbs(gcd, tmp1, (al > bl ? al : bl));
    }

    if (extended) {
        vl = ll_lehmer_update_v(tmp3, tmp4, v0, v1, vl, mat);
        t = v0; v0 = tmp3; tmp3 = t;
        t = v1; v1 = tmp4; tmp4 = t;
        /* correct vl */
        vl = ll_num_limbs(v0, vl);

        if (swap) {
            t = td; td = sd; sd = t;
            tmpl = sl; sl = tl; tl = tmpl;
        }

        if (nc & 1) {
            /* nc is odd, v0 > 0, u0 < 0 */
            if (td != NULL)
                ll_copy_limbs(td, v0, vl);
            if (tl != NULL)
                *tl = (ssize_t)vl;

            if (sd != NULL) {
                /* sd = (bd * v0 - gcd) / ad */
                ll_mul(tsd, bd, v0, bl, vl);
                ll_sub(tsd, tsd, tmp1, bl + vl, gl);
                ll_div(NULL, sd, NULL, &tsl, tsd, ad, bl + vl, al);
            }
            if (sl != NULL)
                *sl = (ssize_t)-tsl;
        }
        else {
            /* nc is even, v0 < 0, u0 > 0 */
            if (td != NULL)
                ll_copy_limbs(td, v0, vl);
            if (tl != NULL)
                *tl = (ssize_t)-vl;

            if (sd != NULL) {
                /* sd = (bd * v0 + gcd) / ad */
                ll_mul(tsd, bd, v0, bl, vl);
                ll_add(tsd, tsd, tmp1, bl + vl, gl);
                ll_div(NULL, sd, NULL, &tsl, tsd, ad, bl + vl, al);
            }
            if (sl != NULL)
                *sl = (ssize_t)tsl;
        }
    }

    free(tmp);
    return gl;
}

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

#pragma once

#include "../../config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* r1,r0 = a1,a0 + b1,b0 */
# define LL_ADD2(r1, r0, a1, a0, b1, b0) do { \
    u64 __c; \
    __c = (a0) + (b0); \
    (r1) = (a1) + (b1) + (__c < (a0)); \
    (r0) = __c; \
} while(0);

/* r1,r0 = a1,a0 - b1,b0 */
# define LL_SUB2(r1, r0, a1, a0, b1, b0) do { \
    u64 __c; \
    __c = (a0) - (b0); \
    (r1) = (a1) - (b1) - ((a0) < (b0)); \
    (r0) = __c; \
} while(0);

/* cmp = 1 : a1:a0 > b1:b0
 * cmp = 0 : a1:a0 = b1:b0
 * cmp =-1 : a1:a0 < b1:b0
 */
# define LL_CMP2(cmp, a1, a0, b1, b0) do { \
    if ((a1) != (b1)) \
        (cmp) = ((a1) > (b1) ? 1 : -1); \
    else if ((a0) != (b0)) \
        (cmp) = ((a0) > (b0) ? 1 : -1); \
    else \
        (cmp) = 0; \
} while(0);

/* r1,r0 = a1,a0 - b1,b0 and cmp = 1 if a1,a0 >= b1,b0
 * r1,r0 = a1,a0         and cmp = 0 if a1,a0 <  b1,b0
 */
# define LL_CMP_SUB2(cmp, r1, r0, a1, a0, b1, b0) do { \
    u64 __c, __c1, __c0, __mask1, __mask2; \
    __c = (a0) - (b0); \
    __mask1 = ((a0) < (b0)); \
    __c1 = (a1) - (b1) - __mask1; \
    __c0 = __c; \
    __mask1 &= ((a1) == (b1)); \
    __mask1 |= ((a1) < (b1)); \
    (cmp) = __mask1 ^ 1; \
    __mask1 = ~__mask1 + 1; \
    __mask2 = ~__mask1; \
    (r1) = ((a1) & __mask1) | (__c1 & __mask2); \
    (r0) = ((a0) & __mask1) | (__c0 & __mask2); \
} while(0);

/* very very very slow multiplication */
# define LL_MUL64(rh, rl, a, b) do { \
    u64 __t1, __t2, __ah, __al, __bh, __bl; \
    __ah = (a) >> 32; \
    __al = (a) & 0xffffffffULL; \
    __bh = (b) >> 32; \
    __bl = (b) & 0xffffffffULL; \
    __t1 = __al * __bh; \
    __t2 = __ah * __bl; \
    (rl) = __al * __bl; \
    (rh) = __ah * __bh; \
    __t1 += ((rl) >> 32); \
    (rl) &= 0xffffffffULL; \
    __t1 += __t2; \
    if (__t1 < __t2) \
        (rh) += 0x100000000ULL; \
    (rl) |= (__t1 << 32); \
    (rh) += (__t1 >> 32); \
} while(0);

# define LL_MULLO64(rl, a, b) do { \
    u64 __t1, __t2, __ah, __al, __bh, __bl; \
    __ah = (a) >> 32; \
    __al = (a) & 0xffffffffULL; \
    __bh = (b) >> 32; \
    __bl = (b) & 0xffffffffULL; \
    __t1 = __al * __bh; \
    __t2 = __ah * __bl; \
    (rl) = __al * __bl; \
    __t1 += ((rl) >> 32); \
    (rl) &= 0xffffffffULL; \
    __t1 += __t2; \
    (rl) |= (__t1 << 32); \
} while(0);

# define LL_SQR64(rh, rl, a) do { \
    u64 __t1, __t2, __ah, __al; \
    __ah = (a) >> 32; \
    __al = (a) & 0xffffffffULL; \
    __t1 = __al * __ah; \
    (rl) = __al * __al; \
    (rh) = __ah * __ah; \
    __t2 = (__t1 << 33); \
    (rl) = (rl) + __t2; \
    (rh) += (__t1 >> 31); \
    (rh) += (rl < __t2); \
} while(0);

# define LL_SQRLO64(rl, a) do { \
    u64 __t1, __t2, __ah, __al; \
    __ah = (a) >> 32; \
    __al = (a) & 0xffffffffULL; \
    __t1 = __al * __ah; \
    (rl) = __al * __al; \
    (rl) = (rl) + (__t1 << 33); \
} while(0);

/* y4,y3,y2,y1,y0 = y3,y2,y1,y0 + x3,x2,x1,x0 */
# define LL_U256_ADD(y4, y3, y2, y1, y0, x3, x2, x1, x0, t) do { \
    u64 __w; \
    y0 += x0; \
    __w = (y0 < x0); \
 \
    t = y1 + __w; \
    __w = (t < y1); \
    y1 = t + x1; \
    __w |= (y1 < t); \
 \
    t = y2 + __w; \
    __w = (t < y2); \
    y2 = t + x2; \
    __w |= (y2 < t); \
 \
    t = y3 + __w; \
    y4 = (t < y3); \
    y3 = t + x3; \
    y4 |= (y3 < t); \
} while(0);

# define LL_U256_MUL_U64(y4, y3, y2, y1, y0, x3, x2, x1, x0, b, hi, lo, t) do { \
    u64 __w; \
    __w = b; \
    LL_MUL64(hi, y0, x0, __w); \
 \
    LL_MUL64(t, y1, x1, __w); \
    y1 += hi; \
    t += (y1 < hi); \
 \
    LL_MUL64(hi, y2, x2, __w); \
    y2 += t; \
    hi += (y2 < t); \
 \
    LL_MUL64(y4, y3, x3, __w); \
    y3 += hi; \
    y4 += (y3 < hi); \
} while(0);

/* y5,y4,y3,y2,y1,y0 += x3,x2,x1,x0 * w */
# define LL_U256_MUL_ADD1(y5, y4, y3, y2, y1, y0, x3, x2, x1, x0, w, hi, lo, t) do { \
    LL_MUL64(hi, lo, x0, w); \
    y0 += lo; \
    hi += (y0 < lo); \
 \
    LL_MUL64(t, lo, x1, w); \
    y1 += hi; \
    t += (y1 < hi); \
    y1 += lo; \
    t += (y1 < lo); \
 \
    LL_MUL64(hi, lo, x2, w); \
    y2 += t; \
    hi += (y2 < t); \
    y2 += lo; \
    hi += (y2 < lo); \
 \
    LL_MUL64(t, lo, x3, w); \
    y3 += hi; \
    t += (y3 < hi); \
    y3 += lo; \
    t += (y3 < lo); \
 \
    y4 += t; \
    y5 += (y4 < t); \
} while(0);

# define LL_U256_MONT_MUL_ADD(y5, y4, y3, y2, y1, y0, x3, x2, x1, x0, b, hi, lo, t) do { \
    u64 __w; \
    __w = b; \
    y5 = 0; \
    LL_U256_MUL_ADD1(y5, y4, y3, y2, y1, y0, x3, x2, x1, x0, __w, hi, lo, t); \
} while(0);

# define LL_U256_MONT_REDC1(y5, y4, y3, y2, y1, y0, x3, x2, x1, x0, k0, hi, lo, t) do { \
    u64 __w; \
    __w = y0 * k0; \
    LL_U256_MUL_ADD1(y5, y4, y3, y2, y1, y0, x3, x2, x1, x0, __w, hi, lo, t); \
} while(0);

/* y0,y3,y2,y1 = (y3,y2,y1,y0 + x3,x2,x1,x0 * (y0 * k0)) / 2^64 */
# define LL_U256_MONT_REDC2(y3, y2, y1, y0, x3, x2, x1, x0, k0, hi, lo, t) do { \
    u64 __w; \
    __w = y0 * k0; \
    LL_MUL64(hi, lo, x0, __w); \
    y0 += lo; \
    hi += (y0 < lo); \
 \
    LL_MUL64(t, lo, x1, __w); \
    y1 += hi; \
    t += (y1 < hi); \
    y1 += lo; \
    t += (y1 < lo); \
 \
    LL_MUL64(hi, lo, x2, __w); \
    y2 += t; \
    hi += (y2 < t); \
    y2 += lo; \
    hi += (y2 < lo); \
 \
    LL_MUL64(y0, lo, x3, __w); \
    y3 += hi; \
    y0 += (y3 < hi); \
    y3 += lo; \
    y0 += (y3 < lo); \
} while(0);

/* t3,t2,t1,t0
 * = y4,y3,y2,y1,y0 - x3,x2,x1,x0 if y >= x
 * = y3,y2,y1,y0                  if y < x
 */
# define LL_U256_MONT_COND_SUB(t3, t2, t1, t0, y4, y3, y2, y1, y0, x3, x2, x1, x0, b, t) do { \
    u64 __b; \
    t0 = y0 - x0; \
    __b = (y0 < x0); \
 \
    t = y1 - __b; \
    __b = (y1 < __b); \
    t1 = t - x1; \
    __b |= (t < x1); \
 \
    t = y2 - __b; \
    __b = (y2 < __b); \
    t2 = t - x2; \
    __b |= (t < x2); \
 \
    t = y3 - __b; \
    __b = (y3 < __b); \
    t3 = t - x3; \
    __b |= (t < x3); \
 \
    t = -(u64)(y4 >= __b); \
    t0 = (t0 & t) | (y0 & (~t)); \
    t1 = (t1 & t) | (y1 & (~t)); \
    t2 = (t2 & t) | (y2 & (~t)); \
    t3 = (t3 & t) | (y3 & (~t)); \
} while(0);

# define LL_ALIGN_NUM(num, align) \
    ((num) + ((num) % (align) == 0 ? 0 : ((align) - (num) % (align))))

# define LL_ALIGN_PTR(ptr, align) \
    ((u8*)(ptr) + ((size_t)(ptr) % (align) == 0 ? 0 : ((align) - (size_t)(ptr) % (align))))

# define LL_EXP_WINDOW_SIZE(b) \
    ((b) > 937 ? 6 : \
    (b) > 306 ? 5 : \
    (b) >  89 ? 4 : \
    (b) >  22 ? 3 : 1)

/* conditional swap a and b */
# define LL_COND_SWAP_LIMB(a, b, cond) do { \
    u64 __t, __cond; \
    __cond = ((~(cond) & ((cond) - 1)) >> 63) - 1; \
    __t = (a) ^ (b) ^ __cond; \
    (a) ^= __t; \
    (b) ^= __t; \
} while(0);

/* swap a and b if a < b */
# define LL_CONDLO_SWAP_LIMB(a, b) do { \
    __cond = ((a) - (b)) >> 63; \
    LL_COND_SWAP_LIMB((a), (b), __cond); \
} while(0);

/* swap a and b if a > b */
# define LL_CONDHI_SWAP_LIMB(a, b) do { \
    __cond = ((b) - (a)) >> 63; \
    LL_COND_SWAP_LIMB((a), (b), __cond); \
} while(0);

/* number of limbs of a(u64 array) */
# define LL_NUM_LIMBS(a, max, nlimbs) do { \
    size_t __n = max; \
    nlimbs = 0; \
 \
    while (__n > 0) { \
        __n--; \
        if (a[__n] > 0ULL) { \
            nlimbs = 1; \
            break; \
        } \
    } \
    nlimbs += __n; \
} while(0);

/* number of bits of a(64 bit integer), from openssl. 
 * ...
 */
/*
# define LL_NUM_BITS(a, nbits) do { \
    u64 __t, __x, __mask; \
 \
    __t = a; \
    nbits = (__t != 0); \
 \
    __x = __t >> 32; \
    __mask = (0 - ((-__x) >> 63)); \
    nbits += 32 & __mask; \
    __t ^= (__x ^ __t) & __mask; \
 \
    __x = __t >> 16; \
    __mask = (0 - ((-__x) >> 63)); \
    nbits += 16 & __mask; \
    __t ^= (__x ^ __t) & __mask; \
 \
    __x = __t >> 8; \
    __mask = (0 - ((-__x) >> 63)); \
    nbits += 8 & __mask; \
    __t ^= (__x ^ __t) & __mask; \
 \
    __x = __t >> 4; \
    __mask = (0 - ((-__x) >> 63)); \
    nbits += 4 & __mask; \
    __t ^= (__x ^ __t) & __mask; \
 \
    __x = __t >> 2; \
    __mask = (0 - ((-__x) >> 63)); \
    nbits += 2 & __mask; \
    __t ^= (__x ^ __t) & __mask; \
 \
    __x = __t >> 1; \
    __mask = (0 - ((-__x) >> 63)); \
    nbits += 1 & __mask; \
} while(0);
*/

/* for _alloca, variable length array. */
# if defined(_MSC_VER)
#  include <malloc.h>
#  define alloca _alloca
# else
#  if defined(__GNUC__)
#   ifndef alloca
#    define alloca __builtin_alloca
#   endif
#  elif defined(HAVE_ALLOCA_H)
#   include <alloca.h>
#  else
#   error alloca not available.
#  endif
# endif

# if defined(ARCH_X86_64)
/* they are only implemented on x86_64 */
void ll_naive_div_1_limb(u64 *rd, u64 *qd, const u64 *nd, const u64 *dd, const size_t nl);
void ll_naive_div_2_limbs(u64 *rd, u64 *qd, const u64 *nd, const u64 *dd, const size_t nl);
void ll_naive_div_3_limbs(u64 *rd, u64 *qd, const u64 *nd, const u64 *dd, const size_t nl);
void ll_naive_div_4_limbs(u64 *rd, u64 *qd, const u64 *nd, const u64 *dd, const size_t nl);
# endif

/* reduce a1:a0, b1:b0 until they don't satisfy approximative condition,
 * mat is the product of matrices during reduction of a1:a0, b1:b0,
 * it will later be used to reduce a, b.
 * return number of matrix multiplication in simulate step.
 */
u64 ll_lehmer_simulate(u64 mat[4], u64 a1, u64 a0, u64 b1, u64 b0);

/* if nc is odd
 *   t1d = -m00 * ad + m01 * bd
 *   t2d =  m10 * ad - m11 * bd
 * if nc is even 
 *   t1d =  m00 * ad - m01 * bd
 *   t2d = -m10 * ad + m11 * bd
 * return max{nlimbs of t1d, nlimbs of t2d}
 */
size_t ll_lehmer_update_ab(u64 *t1d, u64 *t2d, const u64 *ad, const u64 *bd, size_t l, const u64 mat[4], u64 nc);

/* t1d = m00 * v0 + m01 * v1
 * t2d = m10 * v0 + m11 * v1
 * return max{nlimbs of t1d, nlimbs of t2d}
 */
size_t ll_lehmer_update_v(u64 *t1d, u64 *t2d, const u64 *v0, const u64 *v1, size_t l, const u64 mat[4]);

u64 ll_lehmer_exgcd11(u64 mat[4], u64 a0, u64 b0);

u64 ll_reciprocal1(u64 d);
u64 ll_reciprocal2(u64 d1, u64 d0);
u64 ll_div2by1_pi1(u64 *r, const u64 n[2], u64 d, u64 v);
u64 ll_div3by2_pi1(u64 *r, const u64 n[3], const u64 d[2], u64 v);

void ll_u256_select(u64 r[4], const u64 *table, size_t table_size, size_t index);
void ll_mont_cond_sub_limbs(u64 *rd, const u64 *ad, const u64 *bd, size_t l);

typedef struct cpu_flags_st {
    int initialized;
    /* arm */
    int has_neon;
    /* x64 */
    int has_sse2;
    int has_sse3;
    int has_avx;
    int has_avx2;
    int has_bmi2;
    int has_adx;
    int has_rdrand;
    int has_rdseed;
} CPU_FLAGS;

extern CPU_FLAGS cpu_flags;

# if defined(ARCH_X86_64)
void ll_cpuid_x86(u32 ieax, u32 iecx, u32 *oeax, u32 *oebx, u32 *oecx, u32 *oedx);
# endif

#ifdef __cplusplus
}
#endif

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

#include <fp256/fp256.h>
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

/* r1,r0 = a1,a0 - b1,b0 and cmp = 1 if a1,a0 >= b1,b0
 * r1,r0 = a1,a0         and cmp = 0 if a1,a0 <  b1,b0
 */
# define LL_CMP_SUB2(cmp, r1, r0, a1, a0, b1, b0) do { \
    u64 __c, __c1, __c0, __mask1, __mask2; \
    __c = (a0) - (b0); \
    __mask1 = ((a0) < (b0)); \
    __c1 = (a1) - (b1) - __mask1; \
    __c0 = __c; \
    __mask1 &= ((a0) == (b0)); \
    __mask1 |= ((a1) < (b1)); \
    (cmp) = __mask1 ^ 1; \
    __mask1 = ~__mask1 + 1; \
    __mask2 = ~__mask1; \
    (r1) = ((a1) & __mask1) | (__c1 & __mask2); \
    (r0) = ((a0) & __mask1) | (__c0 & __mask2); \
} while(0);

/* very slow multiplication */
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

# define LL_ALIGN_NUM(num, align) \
    ((num) + ((num) % (align) == 0 ? 0 : ((align) - (num) % (align))))

# define LL_ALIGN_PTR(ptr, align) \
    ((u8*)(ptr) + ((size_t)(ptr) % (align) == 0 ? 0 : ((align) - (size_t)(ptr) % (align))))

# define LL_EXP_WINDOW_SIZE(b) \
    ((b) > 937 ? 6 : \
    (b) > 306 ? 5 : \
    (b) >  89 ? 4 : \
    (b) >  22 ? 3 : 1)

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

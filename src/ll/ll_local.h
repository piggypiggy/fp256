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

#pragma once

#include <fp256/fp256.h>

#ifdef __cplusplus
extern "C" {
#endif

# ifndef USE_ASM
/* very slow multiplication */
# define LL_MUL64(rh, rl, a, b) do { \
    u64 t1, t2, ah, al, bh, bl; \
    ah = (a) >> 32; \
    al = (a) & 0xffffffffULL; \
    bh = (b) >> 32; \
    bl = (b) & 0xffffffffULL; \
    t1 = al * bh; \
    t2 = ah * bl; \
    (rl) = al * bl; \
    (rh) = ah * bh; \
    t1 += ((rl) >> 32); \
    (rl) &= 0xffffffffULL; \
    t1 += t2; \
    if (t1 < t2) \
        (rh) += 0x100000000ULL; \
    (rl) |= (t1 << 32); \
    (rh) += (t1 >> 32); \
} while(0);
# endif

/* rd = ad + b,
 * return carry */
u64 ll_add_limb(u64 *rd, const u64 *ad, u64 b, size_t al);

/* rd = ad + bd, ad and bd both have l limbs,
 * return carry */
u64 ll_add_limbs(u64 *rd, const u64 *ad, const u64 *bd, size_t l);

/* rd = ad - b,
 * return borrow */
u64 ll_sub_limb(u64 *rd, const u64 *ad, u64 b, size_t al);

/* rd = ad - bd, ad and bd both have l limbs,
 * return borrow */
u64 ll_sub_limbs(u64 *rd, const u64 *ad, const u64 *bd, size_t l);

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

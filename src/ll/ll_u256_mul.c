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

#ifndef USE_ASM_U256_MUL
u64 ll_u256_mul_limb(u64 rd[4], const u64 ad[4], u64 b)
{
    u64 hi, lo, t;

    LL_MUL64(hi, lo, ad[0], b);
    rd[0] = lo;

    LL_MUL64(t, lo, ad[1], b);
    lo += hi;
    t += (lo < hi);
    rd[1] = lo;

    LL_MUL64(hi, lo, ad[2], b);
    lo += t;
    hi += (lo < t);
    rd[2] = lo;

    LL_MUL64(t, lo, ad[3], b);
    lo += hi;
    t += (lo < hi);
    rd[3] = lo;

    return t;
}

/* need further simplification? */
void ll_u256_mul(u64 rd[8], const u64 ad[4], const u64 bd[4])
{
    u64 hi, lo, t;
    u64 a0, a1, a2, a3;
    u64 b;
    u64 r0, r1, r2, r3;

    a0 = ad[0]; a1 = ad[1]; a2 = ad[2]; a3 = ad[3];

    /* ad * bd[0] */
    b = bd[0];
    LL_MUL64(hi, lo, a0, b);
    rd[0] = lo;

    LL_MUL64(t, r1, a1, b);
    r1 += hi;
    t += (r1 < hi);

    LL_MUL64(hi, r2, a2, b);
    r2 += t;
    hi += (r2 < t);

    LL_MUL64(r0, r3, a3, b);
    r3 += hi;
    r0 += (r3 < hi);

    /* + ad * bd[1] */
    b = bd[1];
    LL_MUL64(hi, lo, a0, b);
    r1 += lo;
    hi += (r1 < lo);
    rd[1] = r1;

    LL_MUL64(t, lo, a1, b);
    r2 += hi;
    t += (r2 < hi);
    r2 += lo;
    t += (r2 < lo);

    LL_MUL64(hi, lo, a2, b);
    r3 += t;
    hi += (r3 < t);
    r3 += lo;
    hi += (r3 < lo);

    LL_MUL64(r1, lo, a3, b);
    r0 += hi;
    r1 += (r0 < hi);
    r0 += lo;
    r1 += (r0 < lo);

    /* + ad * bd[2] */
    b = bd[2];
    LL_MUL64(hi, lo, a0, b);
    r2 += lo;
    hi += (r2 < lo);
    rd[2] = r2;

    LL_MUL64(t, lo, a1, b);
    r3 += hi;
    t += (r3 < hi);
    r3 += lo;
    t += (r3 < lo);

    LL_MUL64(hi, lo, a2, b);
    r0 += t;
    hi += (r0 < t);
    r0 += lo;
    hi += (r0 < lo);

    LL_MUL64(r2, lo, a3, b);
    r1 += hi;
    r2 += (r1 < hi);
    r1 += lo;
    r2 += (r1 < lo);

    /* + ad * bd[3] */
    b = bd[3];
    LL_MUL64(hi, lo, a0, b);
    r3 += lo;
    hi += (r3 < lo);
    rd[3] = r3;

    LL_MUL64(t, lo, a1, b);
    r0 += hi;
    t += (r0 < hi);
    r0 += lo;
    t += (r0 < lo);

    LL_MUL64(hi, lo, a2, b);
    r1 += t;
    hi += (r1 < t);
    r1 += lo;
    hi += (r1 < lo);

    LL_MUL64(r3, lo, a3, b);
    r2 += hi;
    r3 += (r2 < hi);
    r2 += lo;
    r3 += (r2 < lo);

    rd[4] = r0;
    rd[5] = r1;
    rd[6] = r2;
    rd[7] = r3;

    return;
}

void ll_u256_mullo(u64 rd[4], const u64 ad[4], const u64 bd[4])
{
    u64 hi, lo, t;
    u64 a0, a1, a2, a3;
    u64 b;
    u64 r1, r2, r3;

    a0 = ad[0]; a1 = ad[1]; a2 = ad[2]; a3 = ad[3];

    /* ad * bd[0] */
    b = bd[0];
    LL_MUL64(hi, lo, a0, b);
    rd[0] = lo;

    LL_MUL64(t, r1, a1, b);
    r1 += hi;
    t += (r1 < hi);

    LL_MUL64(hi, r2, a2, b);
    r2 += t;
    hi += (r2 < t);

    LL_MULLO64(lo, a3, b);
    r3 = lo + hi;

    /* + ad * bd[1] */
    b = bd[1];
    LL_MUL64(hi, lo, a0, b);
    r1 += lo;
    hi += (r1 < lo);
    rd[1] = r1;

    LL_MUL64(t, lo, a1, b);
    r2 += hi;
    t += (r2 < hi);
    r2 += lo;
    t += (r2 < lo);

    LL_MULLO64(lo, a2, b);
    r3 += t;
    r3 += lo;

    /* + ad * bd[2] */
    b = bd[2];
    LL_MUL64(hi, lo, a0, b);
    r2 += lo;
    hi += (r2 < lo);
    rd[2] = r2;

    LL_MULLO64(lo, a1, b);
    r3 += hi;
    r3 += lo;

    /* + ad * bd[3] */
    b = bd[3];
    LL_MULLO64(lo, a0, b);
    r3 += lo;
    rd[3] = r3;

    return;
}

void ll_u256_sqr(u64 rd[8], const u64 ad[4])
{
    u64 hi, lo, t;
    u64 a0, a1, a2, a3;
    u64 r0, r1, r2, r3, r4, r5, r6, r7;

    a0 = ad[0]; a1 = ad[1]; a2 = ad[2]; a3 = ad[3];

    /* ad[0] * ad[1] */
    LL_MUL64(r2, r1, a0, a1);
    
    /* + ad[0] * ad[2] */
    LL_MUL64(r3, lo, a0, a2);
    r2 += lo;
    r3 += (r2 < lo);

    /* + ad[0] * ad[3] */
    LL_MUL64(r4, lo, a0, a3);
    r3 += lo;
    r4 += (r3 < lo);

    /* + ad[1] * ad[2] */
    LL_MUL64(hi, lo, a1, a2);
    r3 += lo;
    hi += (r3 < lo);

    /* + ad[1] * ad[3] */
    LL_MUL64(r5, lo, a1, a3);
    r4 += hi;
    r5 += (r4 < hi);
    r4 += lo;
    r5 += (r4 < lo);

    /* + ad[2] * ad[3] */
    LL_MUL64(r6, lo, a2, a3);
    r5 += lo;
    r6 += (r5 < lo);

    /* ad[6]~ad[1] << 1 */
    r7 = r6 >> 63;
    r6 = (r6 << 1) | (r5 >> 63);
    r5 = (r5 << 1) | (r4 >> 63);
    r4 = (r4 << 1) | (r3 >> 63);
    r3 = (r3 << 1) | (r2 >> 63);
    r2 = (r2 << 1) | (r1 >> 63);
    r1 <<= 1;

    /* + ad[0]^2 */
    LL_SQR64(hi, r0, a0);
    r1 += hi;
    t = (r1 < hi);
    rd[0] = r0;
    rd[1] = r1;

    /* + ad[1]^2 */
    LL_SQR64(hi, lo, a1);
    r2 += t;
    hi += (r2 < t);
    r2 += lo;
    hi += (r2 < lo);
    r3 += hi;
    t = (r3 < hi);
    rd[2] = r2;
    rd[3] = r3;

    /* + ad[2]^2 */
    LL_SQR64(hi, lo, a2);
    r4 += t;
    hi += (r4 < t);
    r4 += lo;
    hi += (r4 < lo);
    r5 += hi;
    t = (r5 < hi);
    rd[4] = r4;
    rd[5] = r5;

    /* + ad[3]^2 */
    LL_SQR64(hi, lo, a3);
    r6 += t;
    hi += (r6 < t);
    r6 += lo;
    hi += (r6 < lo);
    r7 += hi;
    rd[6] = r6;
    rd[7] = r7;

    return;
}

void ll_u256_sqrlo(u64 rd[4], const u64 ad[4])
{
    u64 hi, lo, t;
    u64 a0, a1, a2, a3;
    u64 r0, r1, r2, r3;

    a0 = ad[0]; a1 = ad[1]; a2 = ad[2]; a3 = ad[3];

    /* ad[0] * ad[1] */
    LL_MUL64(r2, r1, a0, a1);
    
    /* + ad[0] * ad[2] */
    LL_MUL64(r3, lo, a0, a2);
    r2 += lo;
    r3 += (r2 < lo);

    /* + ad[0] * ad[3] */
    LL_MULLO64(lo, a0, a3);
    r3 += lo;

    /* + ad[1] * ad[2] */
    LL_MUL64(hi, lo, a1, a2);
    r3 += lo;
    hi += (r3 < lo);

    /* ad[3]~ad[1] << 1 */
    r3 = (r3 << 1) | (r2 >> 63);
    r2 = (r2 << 1) | (r1 >> 63);
    r1 <<= 1;

    /* + ad[0]^2 */
    LL_SQR64(hi, r0, a0);
    r1 += hi;
    t = (r1 < hi);
    rd[0] = r0;
    rd[1] = r1;

    /* + ad[1]^2 */
    LL_SQR64(hi, lo, a1);
    r2 += t;
    hi += (r2 < t);
    r2 += lo;
    hi += (r2 < lo);
    r3 += hi;
    t = (r3 < hi);
    rd[2] = r2;
    rd[3] = r3;

    return;
}
#endif

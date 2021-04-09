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

void ll_mont_mul(u64 *rd, const u64 *Ad, const u64 *Bd, const u64 *Nd, u64 k0, size_t l)
{
    size_t i;
    u64 y, *td;

    td = (u64*)alloca((2*l+2) * sizeof(u64));
    ll_mul_limb(td, Ad, Bd[0], l);
    y = td[0] * k0;
    ll_muladd_limb(td, Nd, y, l + 1, l);
    Bd++;

    for (i = 1; i < l; i++) {
        ll_muladd_limb(&td[i], Ad, Bd[0], l + 1, l);
        y = td[i] * k0;
        ll_muladd_limb(&td[i], Nd, y, l + 2, l);
        Bd++;
    }

    ll_mont_cond_sub_limbs(rd, td + l, Nd, l);
    return;
}

void ll_mont_reduce(u64 *rd, const u64 *Ad2, const u64 *Nd, u64 k0, size_t l)
{
    size_t i;
    u64 y, *td;

    td = (u64*)alloca((2*l+2) * sizeof(u64));
    /* copy lower l limbs */
    ll_copy_limbs(td, Ad2, l);
    /* clear higher l limbs */
    ll_set_zero(td + l, l);

    /* reduce */
    for (i = 0; i < l; i++) {
        y = td[i] * k0;
        ll_muladd_limb(&td[i], Nd, y, l, l);
    }

    td[2*l] = ll_add_limbs(td + l, td + l, rd, l);
    ll_mont_cond_sub_limbs(rd, td + l, Nd, l);
    return;
}

void ll_mont_sqr(u64 *rd, const u64 *Ad, const u64 *Nd, u64 k0, size_t l)
{
    ll_mont_mul(rd, Ad, Ad, Nd, k0, l);
    return;
}

void ll_to_mont(u64 *Ad, const u64 *ad, const u64 *Nd, const u64 *RR, u64 k0, size_t l)
{
    ll_mont_mul(Ad, ad, RR, Nd, k0, l);
    return;
}

void ll_from_mont(u64 *ad, const u64 *Ad, const u64 *Nd, u64 k0, size_t l)
{
    ll_mont_reduce(ad, Ad, Nd, k0, l);
    return;
}

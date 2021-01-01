/******************************************************************************
 *                                                                            *
 * Copyright 2020-2021 Meng-Shan Jiang                                        *
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

#ifndef USE_ASM_U256_FMOD
void ll_u256_fmod_neg(u64 r[4], const u64 a[4], const u64 m[4])
{
    (void) r;
    (void) a;
    (void) m;
    return;
}

void ll_u256_fmod_double(u64 r[4], const u64 a[4], const u64 m[4])
{
    (void) r;
    (void) a;
    (void) m;
    return;
}

void ll_u256_fmod_triple(u64 r[4], const u64 a[4], const u64 m[4])
{
    (void) r;
    (void) a;
    (void) m;
    return;
}

void ll_u256_fmod_div_by_2(u64 r[4], const u64 a[4], const u64 m[4])
{
    (void) r;
    (void) a;
    (void) m;
    return;
}

void ll_u256_fmod_add(u64 r[4], const u64 a[4], const u64 b[4], const u64 m[4])
{
    (void) r;
    (void) a;
    (void) b;
    (void) m;
    return;
}

void ll_u256_fmod_add_limb(u64 r[4], const u64 a[4], const u64 w, const u64 m[4])
{
    (void) r;
    (void) a;
    (void) w;
    (void) m;
    return;
}

void ll_u256_fmod_sub(u64 r[4], const u64 a[4], const u64 b[4], const u64 m[4])
{
    (void) r;
    (void) a;
    (void) b;
    (void) m;
    return;
}

void ll_u256_fmod_sub_limb(u64 r[4], const u64 a[4], const u64 w, const u64 m[4])
{
    (void) r;
    (void) a;
    (void) w;
    (void) m;
    return;
}
#endif

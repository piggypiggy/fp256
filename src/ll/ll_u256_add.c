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

#ifndef USE_ASM
u64 ll_u256_add(u64 rd[4], const u64 ad[4], const u64 bd[4])
{
    u64 t, r, carry;

    t = ad[0];
    r = t + bd[0];
    carry = (r < t);
    rd[0] = r;

    t = ad[1];
    t += carry;
    carry = (t < carry);
    r = t + bd[1];
    carry |= (r < t);
    rd[1] = r;

    t = ad[2];
    t += carry;
    carry = (t < carry);
    r = t + bd[2];
    carry |= (r < t);
    rd[2] = r;

    t = ad[3];
    t += carry;
    carry = (t < carry);
    r = t + bd[3];
    carry |= (r < t);
    rd[3] = r;

    return carry;
}

u64 ll_u256_sub(u64 rd[4], const u64 ad[4], const u64 bd[4])
{
    u64 t, r, borrow;

    t = ad[0];
    r = t - bd[0];
    borrow = (r > t);
    rd[0] = r;

    t = ad[1];
    t -= borrow;
    borrow = (t > ad[1]);
    r = t - bd[1];
    borrow |= (r > t);
    rd[1] = r;

    t = ad[2];
    t -= borrow;
    borrow = (t > ad[2]);
    r = t - bd[2];
    borrow |= (r > t);
    rd[2] = r;

    t = ad[3];
    t -= borrow;
    borrow = (t > ad[3]);
    r = t - bd[3];
    borrow |= (r > t);
    rd[3] = r;

    return borrow;
}
#endif

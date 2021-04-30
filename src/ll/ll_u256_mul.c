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
void ll_u256_mul(u64 rd[8], const u64 ad[4], const u64 bd[4])
{
    (void) rd;
    (void) ad;
    (void) bd;
    return;
}

void ll_u256_mullo(u64 rd[4], const u64 ad[4], const u64 bd[4])
{
    (void) rd;
    (void) ad;
    (void) bd;
    return;
}

void ll_u256_sqr(u64 rd[8], const u64 ad[4])
{
    (void) rd;
    (void) ad;
    return;
}

void ll_u256_sqrlo(u64 rd[4], const u64 ad[4])
{
    (void) rd;
    (void) ad;
    return;
}
#endif

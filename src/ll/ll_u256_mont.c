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

#ifndef USE_ASM_U256_MONT
void ll_u256_mont_mul(u64 rd[4], const u64 ad[4], const u64 bd[4], const u64 Nd[4], u64 k0)
{
    return;
}

void ll_u256_mont_sqr(u64 rd[4], const u64 ad[4], const u64 Nd[4], u64 k0)
{
    return;
}
#endif

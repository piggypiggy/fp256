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

#include <fp256/fp256.h>
#include "ll/ll_local.h"

int fp256_has_neon(void)
{
    return cpu_flags.has_neon;
}

int fp256_has_sse2(void)
{
    return cpu_flags.has_sse2;
}

int fp256_has_sse3(void)
{
    return cpu_flags.has_sse3;
}

int fp256_has_avx(void)
{
    return cpu_flags.has_avx;
}

int fp256_has_avx2(void)
{
    return cpu_flags.has_avx2;
}

int fp256_has_bmi2(void)
{
    return cpu_flags.has_bmi2;
}

int fp256_has_adx(void)
{
    return cpu_flags.has_adx;
}

int fp256_has_rdrand(void)
{
    return 0;
    // return cpu_flags.has_rdrand;
}

int fp256_has_rdseed(void)
{
    return cpu_flags.has_rdseed;
}

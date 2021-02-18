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
#include <fp256/fp256_ll.h>

int fp256_div(fp256 *rem, fp256 *quo, const fp256 *num, const fp256 *div)
{
    size_t rl, ql;
    u64 rd[4], qd[4];

    if (num == NULL || div == NULL)
        return FP256_ERR;

    if (rem == NULL && quo == NULL)
        return FP256_ERR;

    if (ll_div(rd, qd, &rl, &ql, num->d, div->d, num->nlimbs, div->nlimbs) != FP256_OK)
        return FP256_ERR;

    if (rem != NULL)
        fp256_set_limbs(rem, rd, rl);

    if (quo != NULL)
        fp256_set_limbs(quo, qd, ql);

    return FP256_OK;
}

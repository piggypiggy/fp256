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

int fp256_lshift(fp256 *r, const fp256 *a, size_t n)
{
    size_t l, b;
    u64 rd[8];

    if (a == NULL || r == NULL)
        return FP256_ERR;

    /* if n >= 256, lower 256 bit of result will be 0 */
    if (n >= 256) {
        fp256_set_zero(r);
        return FP256_OK;
    }

    /* b = n % 64, number of bits to shift
     * l = n / 64, number of limbs to shift 
     */
    b = n & 0x3f;
    l = n >> 6;

    /* set rd[0~2] = 0, rd[3~7] will be set by ll_u256_lshift since 
     * left shift a by b bits will result in a 320 bit integer.
     * rd[7] is set but not used here.
     */
    rd[0] = 0ULL; rd[1] = 0ULL; rd[2] = 0ULL;
    ll_u256_lshift(rd + 3, a->d, b);

    /* set r->d according to l */
    fp256_set_limbs(r, rd + 3 - l, 4);
    return FP256_OK;
}

int fp256_rshift(fp256 *r, const fp256 *a, size_t n)
{
    size_t l, b;
    u64 rd[8];

    if (a == NULL || r == NULL)
        return FP256_ERR;

    /* if n >= 256, result will be 0 */
    if (n >= 256) {
        fp256_set_zero(r);
        return FP256_OK;
    }

    /* b = n % 64, number of bits to shift
     * l = n / 64, number of limbs to shift 
     */
    b = n & 0x3f;
    l = n >> 6;

    /* set rd[4~7] = 0 */
    rd[4] = 0ULL; rd[5] = 0ULL; rd[6] = 0ULL; rd[7] = 0ULL;
    ll_u256_rshift(rd, a->d, b);

    /* set r->d according to l */
    fp256_set_limbs(r, rd + l, 4);
    return FP256_OK;
}

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
#include "ll/ll_local.h"

size_t fp256_num_limbs(const fp256 *a)
{
    size_t l;
    LL_NUM_LIMBS(a->d, 4, l);
    return l;
}

size_t fp256_num_bits(const fp256 *a)
{
    size_t l, b;

    if (a == NULL)
        return -1;

    LL_NUM_LIMBS(a->d, 4, l);
    if (l == 0)
        return 0;

    l--;
    b = ll_num_bits(a->d[l]);
    // LL_NUM_BITS(a->d[l], b);

    return (l << 6) + b;
}

int fp256_copy(fp256 *r, const fp256 *a)
{
    if (r == NULL || a == NULL)
        return FP256_ERR;

    r->d[0] = a->d[0];
    r->d[1] = a->d[1];
    r->d[2] = a->d[2];
    r->d[3] = a->d[3];
    r->nlimbs = a->nlimbs;
    return FP256_OK;
}

int fp256_set_zero(fp256 *r)
{
    if (r == NULL)
        return FP256_ERR;

    r->d[0] = 0ULL;
    r->d[1] = 0ULL;
    r->d[2] = 0ULL;
    r->d[3] = 0ULL;
    r->nlimbs = 0;
    return FP256_OK;
}

int fp256_set_one(fp256 *r)
{
    if (r == NULL)
        return FP256_ERR;

    r->d[0] = 1ULL;
    r->d[1] = 0ULL;
    r->d[2] = 0ULL;
    r->d[3] = 0ULL;
    r->nlimbs = 1;
    return FP256_OK;
}

int fp256_set_limb(fp256 *r, const u64 limb)
{
    if (r == NULL)
        return FP256_ERR;

    r->d[0] = limb;
    r->d[1] = 0ULL;
    r->d[2] = 0ULL;
    r->d[3] = 0ULL;
    r->nlimbs = 1;
    return FP256_OK;
}

int fp256_set_limbs(fp256 *r, const u64 *limbs, size_t l)
{
    size_t i;

    if (l > FP256_LIMBS)
        return FP256_ERR;

    for (i = 0; i < l; i++)
        r->d[i] = limbs[i];
    for (i = l; i < FP256_LIMBS; i++)
        r->d[i] = 0ULL;

    r->nlimbs = fp256_num_limbs(r);
    return FP256_OK;
}

/* 1  : r = 0
 * 0  : r != 0
 * -1 : error
 */ 
int fp256_is_zero(const fp256 *a)
{
    if (a == NULL)
        return -1;

    return (a->nlimbs == 0);
}

/* 1  : r = 1
 * 0  : r != 1
 * -1 : error
 */ 
int fp256_is_one(const fp256 *a)
{
    if (a == NULL)
        return -1;

    return (a->nlimbs == 1 && a->d[0] == 1ULL);
}

/* 1  : r is odd
 * 0  : r is even
 * -1 : error
 */ 
int fp256_is_odd(const fp256 *a)
{
    if (a == NULL)
        return -1;

    return (a->d[0] & 1);
}

/* 1  : r is even
 * 0  : r is odd
 * -1 : error
 */ 
int fp256_is_even(const fp256 *a)
{
    if (a == NULL)
        return -1;

    return (a->d[0] & 1) ^ 1;
}

/* convert hex string to 256bit number */
int fp256_from_hex(fp256 *r, const u8 *hex, size_t hlen)
{
    size_t rl;

    if (r == NULL || hex == NULL || hlen > 64)
        return FP256_ERR;

    ll_set_zero(r->d, 4);
    ll_from_hex(r->d, &rl, hex, hlen);
    r->nlimbs = rl;
    return FP256_OK;
}

/* convert 256bit number to hex string */
int fp256_to_hex(u8 hex[64], const fp256 *a)
{
    if (hex == NULL || a == NULL)
        return FP256_ERR;

    ll_to_hex(hex, NULL, a->d, 4);
    return FP256_OK;
}

/* convert byte array to 256bit number */
int fp256_from_bytes(fp256 *r, const u8 *bytes, size_t blen)
{
    size_t rl;

    if (r == NULL || bytes == NULL || blen > 32)
        return FP256_ERR;

    ll_set_zero(r->d, 4);
    ll_from_bytes(r->d, &rl, bytes, blen);
    r->nlimbs = rl;
    return FP256_OK;
}

/* convert 256bit number to byte array */
int fp256_to_bytes(u8 bytes[32], const fp256 *a)
{
    if (bytes == NULL || a == NULL)
        return FP256_ERR;

    u64_to_u8(bytes,      a->d[3], ORDER_BIG_ENDIAN);
    u64_to_u8(bytes +  8, a->d[2], ORDER_BIG_ENDIAN);
    u64_to_u8(bytes + 16, a->d[1], ORDER_BIG_ENDIAN);
    u64_to_u8(bytes + 24, a->d[0], ORDER_BIG_ENDIAN);
    return FP256_OK;
}

int fp256_test_bit(const fp256 *a, size_t idx)
{
    size_t l, b;
    u64 bit;

    if (a == NULL || idx >= 256)
        return -1;

    b = idx & 0x3f;
    l = idx >> 6;

    /* n's bit of a */
    bit = a->d[l] >> b;
    return bit & 1;
}

int fp256_set_bit(fp256 *a, size_t idx)
{
    size_t l, b; 
    u64 z;

    if (a == NULL || idx >= 256)
        return FP256_ERR;

    b = idx & 0x3f;
    l = idx >> 6;
    z = 1ULL << b;
    a->d[l] |= z;
    a->nlimbs = ll_num_limbs(a->d, 4);

    return FP256_OK;
}

int fp256_clear_set_bit(fp256 *a, size_t idx)
{
    size_t l, b; 
    u64 z;

    if (a == NULL || idx >= 256)
        return FP256_ERR;

    fp256_set_zero(a);
    b = idx & 0x3f;
    l = idx >> 6;
    z = 1ULL << b;
    a->d[l] |= z;
    a->nlimbs = l + 1;

    return FP256_OK;
}

void fp256_cleanse(fp256 *a)
{
    size_t i;
    volatile u8 *p;
    
    i = 0;
    p = (volatile u8*)a;

    while (i < sizeof(fp256))
        p[i++] = 0;
}

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

#ifndef USE_ASM_UTILS

# define ROTL32(a, n) (((a) << n) | ((a) >> (32-n)))
# define ROTR32(a, n) (((a) >> n) | ((a) << (32-n)))
# define ROTL64(a, n) (((a) << n) | ((a) >> (64-n)))
# define ROTR64(a, n) (((a) >> n) | ((a) << (64-n)))

size_t ll_leading_zeros(const u64 a)
{
    return 0;
}

size_t ll_num_bits(const u64 a)
{
    return 0;
}

u32 inline ll_bswap4(const u32 in)
{
    u32 ret;

    ret = (ROTL32(in, 8) & 0x00ff00ff) | (ROTR32(in, 8) & 0xff00ff00);
    return ret;
}

u64 inline ll_bswap8(const u64 in)
{
    u64 ret;

    ret = (ROTL64(in,  8) & 0x000000ff000000ff) | 
          (ROTL64(in, 24) & 0x0000ff000000ff00) | 
          (ROTR64(in, 24) & 0x00ff000000ff0000) | 
          (ROTR64(in,  8) & 0xff000000ff000000);
    return ret;
}
#endif

int ll_cmp_limbs(const u64 *ad, const u64 *bd, size_t al, size_t bl)
{
    if (al > bl)
        return 1;

    if (al < bl)
        return -1;

    while (al > 0) {
        al--;
        if (ad[al] > bd[al])
            return 1;
        if (ad[al] < bd[al])
            return -1;
    }

    return 0;
}

void ll_copy_limbs(u64 *rd, const u64 *ad, size_t al)
{
    memcpy(rd, ad, sizeof(u64) * al);
}

void ll_set_zero(u64 *ad, size_t al)
{
    memset(ad, 0, sizeof(u64) * al);
}

int ll_is_zero(const u64 *ad, size_t al)
{
    size_t i;

    for (i = 0; i < al; i++) {
        if (ad[i] > 0)
            return 0;
    }

    return 1;
}

void ll_set_limb(u64 *ad, size_t al, u64 limb)
{
    memset(ad, 0, sizeof(u64) * al);
    ad[0] = limb;
}

size_t ll_num_limbs(const u64 *ad, size_t max)
{
    while (max > 0) {
        max--;
        if (ad[max] > 0ULL)
            return (max + 1);
    }

    return max;
}

void ll_normalize(u64 *ad, size_t al, size_t max)
{
    ll_set_zero(ad + al, max - al);
}

int ll_test_bit(const u64 *ad, size_t idx)
{
    size_t l, b;

    b = idx & 0x3f;
    l = idx >> 6;

    return (ad[l] >> b) & 1;
}

void ll_set_bit(u64 *ad, size_t idx)
{
    size_t l, b;

    b = idx & 0x3f;
    l = idx >> 6;
    ad[l] |= (1ULL << b);
}

void ll_clear_set_bit(u64 *ad, size_t idx, size_t max)
{
    size_t l, b;

    b = idx & 0x3f;
    l = idx >> 6;
    ll_set_zero(ad, max);
    ad[l] = (1ULL << b);
}

int ll_from_hex(u64 *rd, size_t *rl, const u8 *hex, size_t hexlen)
{
    u64 *trd;
    size_t trl;

    trd = rd;
    trl = 0;
    while (hexlen >= 16) {
        if (hex_to_u64(trd, hex + hexlen - 16, 16, ORDER_BIG_ENDIAN) != FP256_OK)
            return FP256_ERR;
        hexlen -= 16;
        trl++;
        trd++;
    }

    if (hexlen > 0) {
        if (hex_to_u64(trd, hex, hexlen, ORDER_BIG_ENDIAN) != FP256_OK)
            return FP256_ERR;
        trl++;
    }

    if (rl != NULL)
        *rl = ll_num_limbs(rd, trl);

    return FP256_OK;
}

int ll_to_hex(u8 *hex, size_t *hlen, const u64 *rd, size_t rl)
{
    size_t len;

    len = rl * 16;
    rd += rl;
    while (rl > 0) {
        rd--;
        u64_to_hex(hex, rd[0], ORDER_BIG_ENDIAN);
        hex += 16;
        rl--;
    }

    if (hlen != NULL)
        *hlen = len;

    return FP256_OK;
}

int ll_from_bytes(u64 *rd, size_t *rl, const u8 *bytes, size_t blen)
{
    u64 *trd;
    size_t trl;

    trd = rd;
    trl = 0;
    while (blen >= 8) {
        u8_to_u64(trd, bytes + blen - 8, 8, ORDER_BIG_ENDIAN);
        blen -= 8;
        trd++;
        trl++;
    }

    if (blen > 0) {
        u8_to_u64(trd, bytes, blen, ORDER_BIG_ENDIAN);
        trl++;
    }

    if (rl != NULL)
        *rl = ll_num_limbs(rd, trl);;

    return FP256_OK;
}

int ll_to_bytes(u8 *bytes, size_t *blen, const u64 *rd, size_t rl)
{
    size_t len;

    len = rl * 8;
    rd += rl;
    while (rl > 0) {
        rd--;
        u64_to_u8(bytes, rd[0], ORDER_BIG_ENDIAN);
        bytes += 8;
        rl--;
    }

    if (blen != NULL)
        *blen = len;

    return FP256_OK;
}

u64 ll_invert_limb(u64 a)
{
    u64 inv;

    inv = (((a + 2u) & 4u) << 1) + a;
    inv *= (2 - inv * a);
    inv *= (2 - inv * a);
    inv *= (2 - inv * a);
    inv *= (2 - inv * a);
    inv = -inv;
    return inv;
}

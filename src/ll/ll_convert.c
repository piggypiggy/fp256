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

/* 0->'0', 1->'1', ... , 15->'F' */
static const u8 ascii_table[16] = {
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
};

static const u8 inv_ascii_table[128] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
       0,    1,    2,    3,    4,    5,    6,    7,
       8,    9, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff,   10,   11,   12,   13,   14,   15, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff,   10,   11,   12,   13,   14,   15, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};

u32 to_be32(const u32 in)
{
    u32 ret;

#if (ENDIANESS == ORDER_LITTLE_ENDIAN)
    ret = ll_bswap4(in);
#else 
    ret = in;
#endif

    return ret;
}

u32 to_le32(const u32 in)
{
    u32 ret;

#if (ENDIANESS == ORDER_BIG_ENDIAN)
    ret = ll_bswap4(in);
#else 
    ret = in;
#endif

    return ret;
}

int u8_to_u32(u32 *out, const u8 *in, size_t inlen, int order)
{
    u8 t[8];
    u32 z;

    if (in == NULL || inlen > 4 || out == NULL)
        return FP256_ERR;
    
    memset(t, 0, 4);
    memcpy(t + 4, in, inlen);
    z = 0;

    if (order == ENDIANESS)
        z = *(u32*)(t + inlen);
    else {
        z = *(u32*)(t + inlen);
        z = ll_bswap4(z);
    }
    *out = z;

    return FP256_OK;
}

int u32_to_u8(u8 out[4], const u32 in, int order)
{
    if (out == NULL)
        return FP256_ERR;

    if (order == ENDIANESS)
        *(u32*)out = in;
    else
        *(u32*)out = ll_bswap4(in);

    return FP256_OK;
}

int u8_to_u64(u64 *out, const u8 *in, size_t inlen, int order)
{
    u8 t[16];
    u64 z;

    if (in == NULL || inlen > 8 || out == NULL)
        return FP256_ERR;
    
    memset(t, 0, 8);
    memcpy(t + 8, in, inlen);
    z = 0;

    if (order == ENDIANESS)
        z = *(u64*)(t + inlen);
    else {
        z = *(u64*)(t + inlen);
        z = ll_bswap8(z);
    }
    *out = z;

    return FP256_OK;
}

int u64_to_u8(u8 out[8], const u64 in, int order)
{
    if (out == NULL)
        return FP256_ERR;

    if (order == ENDIANESS)
        *(u64*)out = in;
    else
        *(u64*)out = ll_bswap8(in);

    return FP256_OK;
}

int u32_to_hex(u8 out[8], const u32 in, int order)
{
    if (out == NULL)
        return FP256_ERR;
    
    if (order == ORDER_BIG_ENDIAN) {
        out[0] = ascii_table[in >> 28];
        out[1] = ascii_table[(in >> 24) & 0xf];
        out[2] = ascii_table[(in >> 20) & 0xf];
        out[3] = ascii_table[(in >> 16) & 0xf];
        out[4] = ascii_table[(in >> 12) & 0xf];
        out[5] = ascii_table[(in >>  8) & 0xf];
        out[6] = ascii_table[(in >>  4) & 0xf];
        out[7] = ascii_table[in & 0xf];
    }

    if (order == ORDER_LITTLE_ENDIAN) {
        out[6] = ascii_table[in >> 28];
        out[7] = ascii_table[(in >> 24) & 0xf];
        out[4] = ascii_table[(in >> 20) & 0xf];
        out[5] = ascii_table[(in >> 16) & 0xf];
        out[2] = ascii_table[(in >> 12) & 0xf];
        out[3] = ascii_table[(in >>  8) & 0xf];
        out[0] = ascii_table[(in >>  4) & 0xf];
        out[1] = ascii_table[in & 0xf];
    }

    return FP256_OK;
}

int hex_to_u64(u64 *out, const u8 *in, size_t inlen, int order)
{
    u8 tmp[8];

    if (out == NULL || inlen > 16)
        return FP256_ERR;

    memset(tmp, 0, 8);
    if (hex_to_u8(tmp + 8 - (inlen + 1)/2, in, inlen) != FP256_OK)
        return FP256_ERR;

    u8_to_u64(out, tmp, 8, order);

    return FP256_OK;
}

int u64_to_hex(u8 out[16], const u64 in, int order)
{
    u8 tmp[8];

    if (out == NULL)
        return FP256_ERR;

    u64_to_u8(tmp, in, order);
    u8_to_hex(out, tmp, 8);

    return FP256_OK;
}

// TODO : constant time.
int u8_to_hex(u8 *out, const u8 *in, size_t inlen)
{
    size_t i;

    if (out == NULL)
        return FP256_ERR;

    for (i = 0; i < inlen; i++) {
        out[0] = ascii_table[in[i] >> 4];
        out[1] = ascii_table[in[i] & 0xf];
        out += 2;
    }

    return FP256_OK;
}

int hex_to_u8(u8 *out, const u8 *in, size_t inlen)
{
    u8 r1, r2;
    size_t i;

    if (out == NULL)
        return FP256_ERR;

    i = 0;
    if (inlen % 2 == 1) {
        if (in[i] >= 128U)
            return FP256_ERR;

        r1 = inv_ascii_table[in[i]];
        if (r1 == 0xff)
            return FP256_ERR;

        out[0] = r1;
        out++;
        i++;
    }

    for (; i < inlen; i += 2) {
        if (in[i] >= 128U || in[i+1] >= 128U)
            return FP256_ERR;

        r1 = inv_ascii_table[in[i]];
        r2 = inv_ascii_table[in[i+1]];
        if (r1 == 0xff || r2 == 0xff)
            return FP256_ERR;

        out[0] = (r1 << 4) | r2;
        out++;
    }

    return FP256_OK;
}

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

/**
 * @file fp256_ll.h
 * 
 * @defgroup ll Low level routines
 * @{
 *     @defgroup ll_utils Utility
 *     @defgroup ll_addition Low level addition routines
 *     @defgroup ll_subtraction Low level subtraction routines
 *     @defgroup ll_multiplication Low level multiplication routines
 *     @defgroup ll_division Low level division routines
 *     @defgroup ll_conversion Low level format conversion routines
 *     @defgroup ll_shift Low level shift routines
 *     @defgroup ll_mont_multiplication Low level montgomery multiplication routines
 * @}
 */

#pragma once

#include <fp256/fp256.h>

#ifdef __cplusplus
extern "C" {
#endif

//! @addtogroup ll_utils
//! @{

/**
 * set integer represented by an u64 array to 0.
 * 
 * @param[out] ad         - result.
 * @param[in] al          - length of ad array.
 */
FP256_EXPORT void ll_set_zero(u64 *ad, size_t al);

/**
 * set integer represented by an u64 array to a 64 bit integer limb.
 * 
 * @param[out] ad         - result.
 * @param[in] al          - length of ad array.
 * @param[in] limb        - 64 bit integer.
 */
FP256_EXPORT void ll_set_limb(u64 *ad, size_t al, u64 limb);

/**
 * copy u64 array from source(ad) to destination(rd).
 * 
 * @param[out] rd         - destination u64 array.
 * @param[in] ad          - source u64 array.
 * @param[in] al          - number of limbs to copy.
 */
FP256_EXPORT void ll_copy_limbs(u64 *rd, const u64 *ad, size_t al);

/* TODO : inline? */
/**
 * count number of limbs of ad.
 * 
 * @param[in] ad          - u64 array.
 * @param[in] al          - length of ad array.
 * @return number of limbs.
 */
FP256_EXPORT size_t ll_num_limbs(const u64 *ad, size_t max);

/**
 * set the most significant max-al limbs to 0.
 * 
 * @param[out] ad         - result.
 * @param[in] al          - number of limbs to reserve.
 * @param[in] max         - number of limbs ad has.
 */
FP256_EXPORT void ll_normalize(u64 *ad, size_t al, size_t max);

/**
 * count leading zero bits of a.
 * 
 * @param[in] a           - 64 bit integer.
 * @return number of leading zero bits.
 */
FP256_EXPORT size_t ll_leading_zeros(u64 a);

/**
 * index of the most significant bit of a.
 * 
 * @param[in] a           - 64 bit integer.
 * @return index of the most significant bit.
 */
FP256_EXPORT size_t ll_num_bits(u64 a);

/**
 * test if idx's bit of an integer represented by an u64 array is 1.
 * 
 * @param[in] ad          - the u64 array to test.
 * @param[in] ad          - which bit to test.
 * @return 
 * - 1  if idx's bit of ad is 1.
 * - 0  if idx's bit of ad is 0.
 */
FP256_EXPORT int ll_test_bit(const u64 *ad, size_t idx);

/**
 * set idx's bit of an integer represented by an u64 array to 1.
 * 
 * @param[out] ad         - result.
 * @param[in] idx         - which bit to set 1.
 */
FP256_EXPORT void ll_set_bit(u64 *ad, size_t idx);

/**
 * set ad = 0 first, then set idx's bit of an integer represented by an u64 array to 1.\n
 * or equivalently, set ad = 2^{idx}.
 * 
 * @param[out] ad         - result.
 * @param[in] idx         - which bit to set 1.
 * @param[in] max         - number of limbs ad has.
 */
FP256_EXPORT void ll_clear_set_bit(u64 *ad, size_t idx, size_t max);

/**
 * compare integer represented by an u64 array with 0.
 * 
 * @param[in] ad          - u64 array.
 * @param[in] al          - length of ad array.
 * @return
 * - 1 if ad is 0.
 * - 0 otherwise.
 */
FP256_EXPORT int ll_is_zero(const u64 *ad, size_t al);

/**
 * compare two integers represented by u64 array.
 * 
 * @param[in] ad          - the first u64 array to compare.
 * @param[in] bd          - the second u64 array to compare.
 * @param[in] al          - length of ad array.
 * @param[in] bl          - length of bd array.
 * @return
 * - 1  if ad > bd.
 * - 0  if ad = bd.
 * - -1 if ad < bd.
 */
FP256_EXPORT int ll_cmp_limbs(const u64 *ad, const u64 *bd, size_t al, size_t bl);

//! @} ll_utils

/**
 * reverse byte order of a 32 bit integer 
 * 
 * @param[in] in          - 32 bit integer.
 * @return result.
 */
FP256_EXPORT u32 ll_bswap4(u32 in);

/**
 * reverse byte order of a 64 bit integer 
 * 
 * @param[in] in          - 64 bit integer.
 * @return result.
 */
FP256_EXPORT u64 ll_bswap8(u64 in);

/**
 * change byte order of a 32 bit integer from native order to big endian order.
 * 
 * @param[in] in          - 32 bit integer.
 * @return result.
 */
FP256_EXPORT u32 to_be32(const u32 in);

/**
 * change byte order of a 32 bit integer from native order to little endian order.
 * 
 * @param[in] in          - 32 bit integer.
 * @return result.
 */
FP256_EXPORT u32 to_le32(const u32 in);

/**
 * convert u8 array to one 32 bit integer.
 * 
 * @param[out] out        - result.
 * @param[in] in          - the u8 array to convert.
 * @param[in] inlen       - length of u8 array.
 * @param[in] order       - byte order of u8 array.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int u8_to_u32(u32 *out, const u8 *in, size_t inlen, int order);

/**
 * convert one 32 bit integer to u8 array.
 * 
 * @param[out] out        - result.
 * @param[in] in          - the 32 bit integer to convert.
 * @param[in] order       - byte order of the result u8 array.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int u32_to_u8(u8 out[4], const u32 in, int order);

/**
 * convert u8 array to one 64 bit integer.
 * 
 * @param[out] out        - result.
 * @param[in] in          - the u8 array to convert.
 * @param[in] inlen       - length of u8 array.
 * @param[in] order       - byte order of u8 array.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int u8_to_u64(u64 *out, const u8 *in, size_t inlen, int order);

/**
 * convert one 64 bit integer to u8 array.
 * 
 * @param[out] out        - result.
 * @param[in] in          - the 64 bit integer to convert.
 * @param[in] order       - byte order of the result u8 array.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int u64_to_u8(u8 out[8], const u64 in, int order);

/**
 * convert hex array to one 64 bit integer.
 * 
 * @param[out] out        - result.
 * @param[in] in          - the hex array to convert.
 * @param[in] inlen       - length of hex array.
 * @param[in] order       - byte order of hex array.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int hex_to_u64(u64 *out, const u8 *in, size_t inlen, int order);

/**
 * convert one 32 bit integer to hex array.
 * 
 * @param[out] out        - result.
 * @param[in] in          - the 32 bit integer to convert.
 * @param[in] order       - byte order of the result hex array.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int u32_to_hex(u8 out[8], const u32 in, int order);

/**
 * convert one 64 bit integer to hex array.
 * 
 * @param[out] out        - result.
 * @param[in] in          - the 64 bit integer to convert.
 * @param[in] order       - byte order of the result hex array.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int u64_to_hex(u8 out[16], const u64 in, int order);

/**
 * convert u8 array to hex array.
 * 
 * @param[out] out        - result.
 * @param[in] in          - the u8 array to convert.
 * @param[in] inlen       - length of u8 array.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int u8_to_hex(u8 *out, const u8 *in, size_t inlen);

/**
 * convert hex array to u8 array.
 * 
 * @param[out] out        - result.
 * @param[in] in          - the hex array to convert.
 * @param[in] inlen       - length of hex array.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int hex_to_u8(u8 *out, const u8 *in, size_t inlen);

/**
 * convert u8 array to u64 array.
 * 
 * @param[out] rd         - result.
 * @param[out] rl         - length of result.
 * @param[in] bytes       - the u8 array to convert.
 * @param[in] blen        - length of u8 array.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int ll_from_bytes(u64 *rd, size_t *rl, const u8 *bytes, size_t blen);

/**
 * convert hex array to u64 array.
 * 
 * @param[out] rd         - result.
 * @param[out] rl         - length of result.
 * @param[in] bytes       - the hex array to convert.
 * @param[in] blen        - length of hex array.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int ll_from_hex(u64 *rd, size_t *rl, const u8 *hex, size_t hexlen);

/**
 * convert u64 array to u8 array.
 * 
 * @param[out] bytes      - result.
 * @param[out] blen       - length of result.
 * @param[in] rd          - the u64 array to convert.
 * @param[in] rl          - length of u64 array.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int ll_to_bytes(u8 *bytes, size_t *blen, const u64 *rd, size_t rl);

/**
 * convert u64 array to hex array.
 * 
 * @param[out] hex        - result.
 * @param[out] hlen       - length of result.
 * @param[in] rd          - the u64 array to convert.
 * @param[in] rl          - length of u64 array.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int ll_to_hex(u8 *hex, size_t *hlen, const u64 *rd, size_t rl);

/**
 * fast inversion mod 2^64.
 * 
 * @param[in] a           - 64 bit odd integer.
 * @return a^{-1} mod 2^64.
 */
FP256_EXPORT u64 ll_invert_limb(u64 a);

/**
 * add a 256 bit integer and a 64 bit integer, return carry.\n
 * rd = ad + b.
 * 
 * @param[out] rd         - result.
 * @param[in] ad          - the first 256 bit integer to add.
 * @param[in] b           - the second 64 bit integer to add.
 * @return carry.
 */
FP256_EXPORT u64 ll_u256_add_limb(u64 rd[4], const u64 ad[4], u64 b);

/**
 * add two 256 bit integers, return carry.\n
 * rd = ad + bd.
 * 
 * @param[out] rd         - result.
 * @param[in] ad          - the first 256 bit integer to add.
 * @param[in] bd          - the second 256 bit integer to add.
 * @return carry.
 */
FP256_EXPORT u64 ll_u256_add(u64 rd[4], const u64 ad[4], const u64 bd[4]);

/**
 * subtract a 64 bit integer from a 256 bit integers, return borrow.\n
 * rd = ad - b.
 * 
 * @param[out] rd         - result.
 * @param[in] ad          - 256 bit integer.
 * @param[in] b           - 64 bit integer.
 * @return borrow.
 */
FP256_EXPORT u64 ll_u256_sub_limb(u64 rd[4], const u64 ad[4], u64 b);

/**
 * subtract two 256 bit integers, return borrow.\n
 * rd = ad - bd.
 * 
 * @param[out] rd         - result.
 * @param[in] ad          - the first 256 bit integer to add.
 * @param[in] bd          - the second 256 bit integer to sub.
 * @return borrow.
 */
FP256_EXPORT u64 ll_u256_sub(u64 rd[4], const u64 ad[4], const u64 bd[4]);

/** 
 * add an integer represented by u64 array and a 64 bit integer, return carry.\n
 * rd = ad + b.
 * 
 * @param[out] rd         - result.
 * @param[in] ad          - u64 array.
 * @param[in] b           - 64 bit integer.
 * @param[in] al          - length of ad array.
 * @return carry
 */
FP256_EXPORT u64 ll_add_limb(u64 *rd, const u64 *ad, u64 b, size_t al);

/** 
 * add two integers represented by u64 array(same size), return carry.\n
 * rd = ad + bd.
 * 
 * @param[out] rd         - result.
 * @param[in] ad          - u64 array.
 * @param[in] bd          - u64 array.
 * @param[in] l           - length of ad and bd array.
 * @return carry.
 */
FP256_EXPORT u64 ll_add_limbs(u64 *rd, const u64 *ad, const u64 *bd, size_t l);

/**
 * add two integers represented by u64 array, return carry.\n
 * rd = ad + bd.
 * 
 * @param[out] rd         - result.
 * @param[in] ad          - u64 array.
 * @param[in] bd          - u64 array.
 * @param[in] al          - length of ad array.
 * @param[in] bl          - length of bd array.
 * @return carry.
 */
FP256_EXPORT u64 ll_add(u64 *rd, const u64 *ad, const u64 *bd, size_t al, size_t bl);

/** 
 * substract a 64 bit integer from an integer represented by u64 array, return borrow.\n
 * rd = ad - b.
 * 
 * @param[out] rd         - result.
 * @param[in] ad          - u64 array.
 * @param[in] b           - 64 bit integer.
 * @param[in] al          - length of ad array.
 * @return carry.
 */
FP256_EXPORT u64 ll_sub_limb(u64 *rd, const u64 *ad, u64 b, size_t al);

/** 
 * substract two integers represented by u64 array(same size), return borrow.\n
 * rd = ad - bd.
 * 
 * @param[out] rd         - result.
 * @param[in] ad          - u64 array.
 * @param[in] bd          - u64 array.
 * @param[in] l           - length of ad and bd array.
 * @return carry.
 */
FP256_EXPORT u64 ll_sub_limbs(u64 *rd, const u64 *ad, const u64 *bd, size_t l);

/**
 * subtract two integers represented by u64 array, assume ad >= bd.\n
 * return borrow, which is always 0.\n
 * rd = ad - bd.
 * 
 * @param[out] rd         - result.
 * @param[in] ad          - u64 array.
 * @param[in] bd          - u64 array.
 * @param[in] al          - length of ad array.
 * @param[in] bl          - length of bd array.
 * @return 0.
 */
FP256_EXPORT u64 ll_sub(u64 *rd, const u64 *ad, const u64 *bd, size_t al, size_t bl);

/**
 * multiply big integer by a 64 bit integer, return the most significant limb of the result.\n
 * rd = ad * b.
 * 
 * @param[out] rd         - result.
 * @param[in] ad          - the first u64 array to multiply.
 * @param[in] b           - the second 64 bit integer to multiply.
 * @param[in] al          - length of ad array.
 * @return the most significant limb of the result.
 */
FP256_EXPORT u64 ll_mul_limb(u64 *rd, const u64 *ad, u64 b, size_t al);

/**
 * multiply big integer by a 64 bit integer and add it to another big integer, return the most significant limb of the result.\n
 * rd = rd + ad * b.
 * 
 * @param[in,out] rd      - result.
 * @param[in] ad          - the first u64 array to multiply.
 * @param[in] b           - the second 64 bit integer to multiply.
 * @param[in] rl          - length of the input rd array.
 * @param[in] al          - length of ad array.
 * @return the most significant limb of the result.
 */
FP256_EXPORT u64 ll_muladd_limb(u64 *rd, const u64 *ad, u64 b, size_t rl, size_t al);

/**
 * multiply big integer by a 64 bit integer and subtract it to a big integer, return the most significant limb of the result.
 * assume result >= 0.\n
 * rd = rd - ad * b.
 * 
 * @param[in,out] rd      - result.
 * @param[in] ad          - the first u64 array to multiply.
 * @param[in] b           - the second 64 bit integer to multiply.
 * @param[in] al          - length of the input rd array.
 * @return the most significant limb of the result.
 */
FP256_EXPORT u64 ll_mulsub_limb(u64 *rd, const u64 *ad, u64 b, size_t rl, size_t al);

/**
 * multiply two big integer represented by u64 array, return the most significant limb of the result.\n
 * rd = ad * bd.
 * 
 * @param[out] rd         - result.
 * @param[in] ad          - the first u64 array to multiply.
 * @param[in] bd          - the second u64 array to multiply.
 * @param[in] al          - length of ad array.
 * @param[in] bl          - length of bd array.
 * @return the most significant limb of the result.
 */
FP256_EXPORT u64 ll_mul(u64 *rd, const u64 *ad, const u64 *bd, size_t al, size_t bl);

/**
 * montgomery multiplication of two integers in montgomery representation.\n
 * rd = ad * bd * R^{-1} mod Nd, R = 2^(64*l).
 * 
 * @param[out] rd         - result.
 * @param[in] Ad          - the first u64 array to multiply.
 * @param[in] Bd          - the second u64 array to multiply.
 * @param[in] Nd          - odd modulus, see mont_ctx.
 * @param[in] k0          - precomputed value, see mont_ctx.
 * @param[in] l           - length of Ad, Bd array.
 */
FP256_EXPORT void ll_mont_mul(u64 *rd, const u64 *Ad, const u64 *Bd, const u64 *Nd, u64 k0, size_t l);

/**
 * montgomery square of an integer in montgomery representation.\n
 * rd = ad^2 * R^{-1} mod Nd, R = 2^(64*l).
 * 
 * @param[out] rd         - result.
 * @param[in] Ad          - the u64 array to square.
 * @param[in] Nd          - odd modulus, see mont_ctx.
 * @param[in] k0          - precomputed value, see mont_ctx.
 * @param[in] l           - length of Ad, Bd array.
 */
FP256_EXPORT void ll_mont_sqr(u64 *rd, const u64 *Ad, const u64 *Nd, u64 k0, size_t l);

/**
 * transform integer ad to montgomery representation.
 * 
 * @param[out] Ad         - result.
 * @param[in] ad          - the u64 array to transform.
 * @param[in] Nd          - odd modulus, see mont_ctx.
 * @param[in] RR          - precomputed value, see mont_ctx.
 * @param[in] k0          - precomputed value, see mont_ctx.
 * @param[in] l           - length of Ad, Bd array.
 */
FP256_EXPORT void ll_to_mont(u64 *Ad, const u64 *ad, const u64 *Nd, const u64 *RR, u64 k0, size_t l);

/**
 * transform integer Ad from montgomery representation.
 * 
 * @param[out] ad         - result.
 * @param[in] Ad          - the u64 array to transform.
 * @param[in] Nd          - odd modulus, see mont_ctx.
 * @param[in] k0          - precomputed value, see mont_ctx.
 * @param[in] l           - length of Ad, Bd array.
 */
FP256_EXPORT void ll_from_mont(u64 *ad, const u64 *Ad, const u64 *Nd, u64 k0, size_t l);

/**
 * multiply a 256 bit integer and a 64 bit integer.\n
 * rd = ad * b.
 * 
 * @param[out] rd         - result.
 * @param[in] ad          - the first u64 array to multiply.
 * @param[in] b           - the second 64 bit integer to multiply.
 */
FP256_EXPORT u64 ll_u256_mul_limb(u64 rd[4], const u64 ad[4], u64 b);

/**
 * multiply two 256 bit integer represented by u64 array.\n
 * rd = ad * bd.
 * 
 * @param[out] rd         - result.
 * @param[in] ad          - the first u64 array to multiply.
 * @param[in] bd          - the second u64 array to multiply.
 */
FP256_EXPORT void ll_u256_mul(u64 rd[8], const u64 ad[4], const u64 bd[4]);

/**
 * multiply two 256 bit integer represented by u64 array, reserve lower 256 bit result.\n
 * rd = ad * bd.
 * 
 * @param[out] rd         - result.
 * @param[in] ad          - the first u64 array to multiply.
 * @param[in] bd          - the second u64 array to multiply.
 */
FP256_EXPORT void ll_u256_mullo(u64 rd[4], const u64 ad[4], const u64 bd[4]);

/**
 * square a 256 bit integer represented by u64 array.\n
 * rd = ad ^ 2.
 * 
 * @param[out] rd         - result.
 * @param[in] ad          - the u64 array to square.
 */
FP256_EXPORT void ll_u256_sqr(u64 rd[8], const u64 ad[4]);

/**
 * square a 256 bit integer represented by u64 array, reserve lower 256 bit result.\n
 * rd = ad ^ 2.
 * 
 * @param[out] rd         - result.
 * @param[in] ad          - the u64 array to square.
 */
FP256_EXPORT void ll_u256_sqrlo(u64 rd[4], const u64 ad[4]);

/**
 * montgomery multiplication of two integers in montgomery representation.\n
 * rd = ad * bd * R^{-1} mod Nd, R = 2^256.
 * 
 * @param[out] rd         - result.
 * @param[in] ad          - the first u64 array to multiply.
 * @param[in] bd          - the second u64 array to multiply.
 * @param[in] Nd          - odd modulus, see mont_ctx.
 * @param[in] k0          - precomputed value, see mont_ctx.
 */
FP256_EXPORT void ll_u256_mont_mul(u64 rd[4], const u64 ad[4], const u64 bd[4], const u64 Nd[4], u64 k0);

/**
 * montgomery square an integer in montgomery representation.\n
 * rd = ad^2 * R^{-1} mod Nd, R = 2^256.
 * 
 * @param[out] rd         - result.
 * @param[in] ad          - the u64 array to square.
 * @param[in] Nd          - odd modulus, see mont_ctx.
 * @param[in] k0          - precomputed value, see mont_ctx.
 */
FP256_EXPORT void ll_u256_mont_sqr(u64 rd[4], const u64 ad[4], const u64 Nd[4], u64 k0);

/**
 * montgomery reduction.\n
 * rd = ad * R^{-1} mod Nd, R = 2^256.
 * 
 * @param[out] rd         - result.
 * @param[in] ad          - the u64 array to reduce.
 * @param[in] Nd          - odd modulus, see mont_ctx.
 * @param[in] k0          - precomputed value, see mont_ctx.
 */
FP256_EXPORT void ll_u256_mont_reduce(u64 rd[4], const u64 ad[4], const u64 Nd[4], u64 k0);

/**
 * montgomery exponential of an integer in montgomery representation.\n
 * rd = ad^ed * R mod Nd, R = 2^256.
 * 
 * @param[out] rd         - result.
 * @param[in] ad          - the base.
 * @param[in] ed          - the exponent.
 * @param[in] el          - length of ed array.
 * @param[in] RR          - R^2 mod N, see mont_ctx.
 * @param[in] Nd          - odd modulus, see mont_ctx.
 * @param[in] k0          - precomputed value, see mont_ctx.
 */
FP256_EXPORT void ll_u256_mont_exp(u64 rd[4], const u64 ad[4], const u64 *ed, size_t el, const u64 RR[4], const u64 Nd[4], u64 k0);

/**
 * divide two big integers represented by u64 array, TODO : add reference.\n
 * rd = nd % dd, qd = nd / dd.\n
 * @pre
 * rd, qd, nd and dd must be pairwise diffrent pointer,\n
 * rd must have at least dl limbs,\n
 * qd must have at least (nl - dl + 1) limbs.
 * 
 * @param[out] rd         - remainder.
 * @param[out] qd         - quotient.
 * @param[out] rl         - length of rd array.
 * @param[out] ql         - length of qd array.
 * @param[in]  nd         - numerator.
 * @param[in]  dd         - divisor.
 * @param[in]  nl         - length of nd array.
 * @param[in]  dl         - length of dd array.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int ll_div(u64 *rd, u64 *qd, size_t *rl, size_t *ql, const u64 *nd, const u64 *dd, size_t nl, size_t dl);

/**
 * divide two big integers represented by u64 array, TODO : add reference.\n
 * rd = nd % dd, qd = nd / dd.\n
 * @pre
 * rd, qd, nd and dd must be pairwise diffrent pointer,\n
 * dd must be less than 2^256,\n
 * rd must have at least dl limbs,\n
 * qd must have at least (nl - dl + 1) limbs.
 * 
 * @param[out] rd         - remainder.
 * @param[out] qd         - quotient.
 * @param[out] rl         - length of rd array.
 * @param[out] ql         - length of qd array.
 * @param[in]  nd         - numerator.
 * @param[in]  dd         - divisor.
 * @param[in]  nl         - length of nd array.
 * @param[in]  dl         - length of dd array.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int ll_naive_div(u64 *rd, u64 *qd, size_t *rl, size_t *ql, const u64 *nd, const u64 *dd, size_t nl, size_t dl);

/** 
 * lehmer gcd algorithm.\n
 * use double digit(2*64 bit) and approximative condition in simulate step,
 * reference : https://www.sciencedirect.com/science/article/pii/S0747717185710097. \n
 * 
 * it calculates gcd, sd, td, sl and tl satisfying:
 * - gcd = greatest comon divisor of ad and bd,
 * - gcd = ad * sd + bd * td.
 * 
 * @param[out] gcd        - greatest common divisor of ad and bd.
 * @param[out] sd         - .
 * @param[out] td         - .
 * @param[out] sl         - length of sd array, sl < 0 means sd < 0.
 * @param[out] tl         - length of td array, tl < 0 means td < 0.
 * @param[in]  ad         - u64 array.
 * @param[in]  bd         - u64 array.
 * @param[in]  al         - length of ad array.
 * @param[in]  bl         - length of bd array.
 * @param[in]  extended   - 1 : extended gcd algorithm, 0 : gcd algorithm.
 * @return length of gcd array if gcd is not NULL, 0 otherwise.
 */
FP256_EXPORT size_t ll_lehmer_exgcd(u64 *gcd, u64 *sd, u64 *td, ssize_t *sl, ssize_t *tl, 
                    const u64 *ad, const u64 *bd, size_t al, size_t bl, int extended);

/* left shift ad by n bits : rd = ad << n.
 * rd can be equal to ad, n can be larger than 63.
 * caller should prepare enough space for the result, 
 * rd must have at least (al + n/64 + 1) limbs.
 * return most significant limb of rd.
 */
FP256_EXPORT u64 ll_lshift(u64 *rd, const u64 *ad, size_t al, size_t n);

/* right shift ad by n bits : rd = ad >> n.
 * rd can be equal to ad, n can be larger than 63.
 * caller should prepare enough space for the result, 
 * return most significant limb of rd.
 */
FP256_EXPORT u64 ll_rshift(u64 *rd, const u64 *ad, size_t al, size_t n);

/**
 * left shift a 256 bit integer.\n
 * rd = ad << n, n < 64.
 * 
 * @param[out] rd         - result.
 * @param[in] ad          - the u64 array to shift.
 * @param[in] n           - number of bits to shift.
 */
FP256_EXPORT void ll_u256_lshift(u64 rd[5], const u64 ad[4], const size_t n);

/**
 * right shift a 256 bit integer.\n
 * rd = ad >> n, n < 64.
 * 
 * @param[out] rd         - result.
 * @param[in] ad          - the u64 array to shift.
 * @param[in] n           - number of bits to shift.
 */
FP256_EXPORT void ll_u256_rshift(u64 rd[4], const u64 ad[4], const size_t n);

/**
 * fill u8 array with random data. \n
 * 
 * @param[out] buf        - result.
 * @param[in] ad          - length of buf.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int ll_rand_buf(u8 *buf, size_t len);

/**
 * fill u64 array with random data so that the integer it represents is in range [0, 2^(64*nlimbs)). \n
 * 
 * @param[out] rd         - result.
 * @param[in] nlimbs      - number of limbs of random data.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int ll_rand_limbs(u64 *rd, size_t nlimbs);

/**
 * fill u64 array with random data so that the integer it represents is in range [0, 2^(8*nbytes)) \n
 * 
 * @param[out] rd         - result.
 * @param[in] nbytes      - number of bytes of random data.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int ll_rand_bytes(u64 *rd, size_t nbytes);

/**
 * fill u64 array with random data so that the integer it represents is in range [0, 2^(nbits)) \n
 * 
 * @param[out] rd         - result.
 * @param[in] nbits       - number of bits of random data.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int ll_rand_bits(u64 *rd, size_t nbits);

/**
 * fill u64 array with random data so that the integer it represents is in range [0, range) \n
 * 
 * @param[out] rd         - result.
 * @param[in] range       - range of result.
 * @param[in] rgl         - length of range array.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int ll_rand_range(u64 *rd, const u64 *range, size_t rgl);

/**
 * convert u64 array to hex array and print it
 * 
 * @param[in] ad          - the u64 array to print.
 * @param[in] al          - length of ad array.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int ll_print_hex(const u64 *ad, size_t rl);

/**
 * convert u8 array to hex array and print it
 * 
 * @param[in] name        - description for s.
 * @param[in] s           - the u8 array to print.
 * @param[in] slen        - length of s array.
 */
FP256_EXPORT void print_hex(const char *name, const u8 *s, size_t slen);

#ifdef __cplusplus
}
#endif

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

#pragma once

#include <fp256/fp256.h>

#ifdef __cplusplus
extern "C" {
#endif

FP256_EXPORT void ll_set_zero(u64 *ad, size_t al);
FP256_EXPORT void ll_set_limb(u64 *ad, size_t al, u64 limb);
FP256_EXPORT void ll_copy_limbs(u64 *rd, const u64 *ad, size_t al);

/* return nlimbs of a */
FP256_EXPORT size_t ll_num_limbs(const u64 *ad, size_t max);

/* set ad[al:max-1] to 0 */
FP256_EXPORT void ll_normalize(u64 *ad, size_t al, size_t max);

/* return number of leading zero bits of a */
FP256_EXPORT size_t ll_leading_zeros(const u64 a);

/* return index of the most significant bit 1 of a,
 * it's euqal to 64 - ll_leading_zeros(a).
 */
FP256_EXPORT size_t ll_num_bits(const u64 a);

/* 1 : idx's bit of ad is 1
 * 0 : idx's bit of ad is 0
 */
FP256_EXPORT int ll_test_bit(u64 *ad, size_t idx);

/* set idx's bit of ad to 1 */
FP256_EXPORT void ll_set_bit(u64 *ad, size_t idx);

/* set ad[0~max) = 0 first, then set idx's bit of ad to 1, 
 * or equivalently, set ad = 2^{idx}
 */
FP256_EXPORT void ll_clear_set_bit(u64 *ad, size_t idx, size_t max);

/* 1 : ad[0:al-1] = 0
 * 0 : ad[0:al-1] != 0
 */
FP256_EXPORT int ll_is_zero(const u64 *ad, size_t al);

/* compare ad and bd
 * +1 : ad > bd
 *  0 : ad = bd
 * -1 : ad < bd
 */
FP256_EXPORT int ll_cmp_limbs(const u64 *ad, const u64 *bd, size_t al, size_t bl);

/* change byte order */
FP256_EXPORT u32 ll_bswap4(const u32 in);
FP256_EXPORT u64 ll_bswap8(const u64 in);

/* */
FP256_EXPORT u32 to_be32(const u32 in);
FP256_EXPORT u32 to_le32(const u32 in);

/* convert u8 array to one u32 integer, inlen <= 4 */
FP256_EXPORT int u8_to_u32(u32 *out, const u8 *in, size_t inlen, int order);

/* convert one u32 integer to u8 array */
FP256_EXPORT int u32_to_u8(u8 out[4], const u32 in, int order);

/* convert u8 array to one u64 integer, inlen <= 8 */
FP256_EXPORT int u8_to_u64(u64 *out, const u8 *in, size_t inlen, int order);

/* convert one u64 integer to u8 array */
FP256_EXPORT int u64_to_u8(u8 out[8], const u64 in, int order);

/* convert hex array to one u64 integer, inlen <= 16 */
FP256_EXPORT int hex_to_u64(u64 *out, const u8 *in, size_t inlen, int order);

/* convert one u32 integer to hex array */
FP256_EXPORT int u32_to_hex(u8 out[8], const u32 in, int order);

/* convert one u64 integer to hex array */
FP256_EXPORT int u64_to_hex(u8 out[16], const u64 in, int order);

/* convert one u8 array to hex array */
FP256_EXPORT int u8_to_hex(u8 *out, const u8 *in, size_t in_len);

/* convert one hex array to u8 array */
FP256_EXPORT int hex_to_u8(u8 *out, const u8 *in, size_t in_len);

/* byte array to u64 array */
FP256_EXPORT int ll_from_bytes(u64 *rd, size_t *rl, const u8 *bytes, size_t blen);

/* hex array to u64 array */
FP256_EXPORT int ll_from_hex(u64 *rd, size_t *rl, const u8 *hex, size_t hexlen);

/* u64 array to byte array */
FP256_EXPORT int ll_to_bytes(u8 *bytes, size_t *blen, const u64 *rd, size_t rl);

/* u64 array to hex array */
FP256_EXPORT int ll_to_hex(u8 *hex, size_t *hlen, const u64 *rd, size_t rl);

/* add two 256 bit integers, return carry */
FP256_EXPORT u64 ll_u256_add(u64 rd[4], const u64 ad[4], const u64 bd[4]);

/* sub two 256 bit integers, return borrow */
FP256_EXPORT u64 ll_u256_sub(u64 rd[4], const u64 ad[4], const u64 bd[4]);

/* rd = ad + bd, 
 * return nlimbs of rd
 */
FP256_EXPORT size_t ll_add(u64 *rd, const u64 *ad, const u64 *bd, size_t al, size_t bl);

/* rd = ad - bd, assume ad >= bd
 * return nlimbs of rd
 */
FP256_EXPORT size_t ll_sub(u64 *rd, const u64 *ad, const u64 *bd, size_t al, size_t bl);

/* rd = ad * b
 * return nlimbs of rd
 */
FP256_EXPORT size_t ll_mul_limb(u64 *rd, const u64 *ad, u64 b, size_t al);

/* rd = rd + ad * b
 * return nlimbs of rd
 */
FP256_EXPORT size_t ll_muladd_limb(u64 *rd, const u64 *ad, u64 b, size_t rl, size_t al);

/* rd = rd - ad * b, assume rd > ad * b
 * return nlimbs of rd
 */
FP256_EXPORT size_t ll_mulsub_limb(u64 *rd, const u64 *ad, u64 b, size_t rl, size_t al);

/* rd = ad * bd 
 * return nlimbs of rd
 */
FP256_EXPORT size_t ll_mul(u64 *rd, const u64 *ad, const u64 *bd, size_t al, size_t bl);

/* rd = ad * bd
 * ad, bd are less than 2^256
 */
FP256_EXPORT void ll_u256_mul_limbs(u64 rd[4*2], const u64 ad[4], const u64 bd[4], size_t al, size_t bl);
FP256_EXPORT void ll_u256_mul(u64 rd[4*2], const u64 ad[4], const u64 bd[4]);

/* rd = ad^2
 * ad is less than 2^256
 */
FP256_EXPORT void ll_u256_sqr(u64 rd[4*2], const u64 ad[4]);

/* rd = montgomery multiplication of ad and bd, 
 * or rd = ad * bd * R^{-1} mod Nd, R = 2^256.
 * ad and bd should be less than Nd.
 */
FP256_EXPORT void ll_u256_mont_mul(u64 rd[4], const u64 ad[4], const u64 bd[4], const u64 Nd[4], u64 k0);

/* rd = montgomery multiplication of ad and ad, 
 * or rd = ad^2 * R^{-1} mod Nd, R = 2^256.
 * ad should be less than Nd.
 */
FP256_EXPORT void ll_u256_mont_sqr(u64 rd[4], const u64 ad[4], const u64 Nd[4], u64 k0);

/* divide nd by dd
 * rd = nd % dd
 * qd = nd / dd
 * 
 * require
 * rd, qd, nd and dd are pairwise diffrent pointer
 * dd is less than 2^256
 * rd has at least dl limbs
 * qd has at least (nl - dl + 1) limbs
 */
FP256_EXPORT int ll_div(u64 *rd, u64 *qd, size_t *rl, size_t *ql, const u64 *nd, const u64 *dd, size_t nl, size_t dl);
FP256_EXPORT void ll_div_1_limb(u64 *rd, u64 *qd, const u64 *nd, const u64 *dd, const size_t nl);
FP256_EXPORT void ll_div_2_limbs(u64 *rd, u64 *qd, const u64 *nd, const u64 *dd, const size_t nl);
FP256_EXPORT void ll_div_3_limbs(u64 *rd, u64 *qd, const u64 *nd, const u64 *dd, const size_t nl);
FP256_EXPORT void ll_div_4_limbs(u64 *rd, u64 *qd, const u64 *nd, const u64 *dd, const size_t nl);

/* lehmer gcd algorithm.
 * it calculates gcd, sd, td, sl and tl, satisfying
 * gcd = gcd(ad, bd), gcd = ad * sd + bd * td.
 * 
 * use double digit(2*64 bit) and approximative condition in simulate step,
 * reference : https://www.sciencedirect.com/science/article/pii/S0747717185710097.
 * 
 * only track v0, v1 where 
 * a0 >= a1,
 * u0 * a + v0 * b = a0, 
 * u1 * a + v1 * b = a1.
 * 
 * if gcd != NULL, returns nlimbs of gcd, otherwise returns 0.
 * sl < 0 means sd < 0, 
 * tl < 0 means td < 0.
 * gcd, sd, td, sl and tl can be NULL.
 */
FP256_EXPORT size_t ll_lehmer_exgcd(u64 *gcd, u64 *sd, u64 *td, ssize_t *sl, ssize_t *tl, 
                    const u64 *ad, const u64 *bd, size_t al, size_t bl, int extended);

/* left shift a by n bits : r = a << n.
 * r can be equal to a, n can be larger than 63.
 * caller should prepare enough space for the result, 
 * r must have at least (al + n/64 + 1) limbs.
 * return nlimbs of r.
 */
FP256_EXPORT size_t ll_lshift(u64 *r, const u64 *a, size_t al, size_t n);

/* right shift a by n bits : r = a >> n.
 * r can be equal to a, n can be larger than 63.
 * caller should prepare enough space for the result, 
 * r must have at least al limbs.
 * return nlimbs of r.
 */
FP256_EXPORT size_t ll_rshift(u64 *r, const u64 *a, size_t al, size_t n);

/* left shift a by b bits,  r = a << b, 0 <= b < 64 */
FP256_EXPORT void ll_u256_lshift(u64 rd[5], const u64 ad[4], const size_t n);

/* right shift a by b bits, r = a >> b, 0 <= b < 64 */
FP256_EXPORT void ll_u256_rshift(u64 rd[4], const u64 ad[4], const size_t n);

/* left shift a by b bits,  r = a << b, 0 <= b < 64 */
FP256_EXPORT void ll_u512_lshift(u64 rd[9], const u64 ad[8], const size_t n);

/* right shift a by b bits, r = a >> b, 0 <= b < 64 */
FP256_EXPORT void ll_u512_rshift(u64 rd[8], const u64 ad[8], const size_t n);

/* ll_u256_fmod(f means fast) assume inputs are already reduced and 
 * outputs will also be reduced(i.e. in range [0, m)). 
 */

/* r = -a mod m */
FP256_EXPORT void ll_u256_fmod_neg(u64 r[4], const u64 a[4], const u64 m[4]);

/* r = 2a mod m */
FP256_EXPORT void ll_u256_fmod_double(u64 r[4], const u64 a[4], const u64 m[4]);

/* r = 3a mod m */
FP256_EXPORT void ll_u256_fmod_triple(u64 r[4], const u64 a[4], const u64 m[4]);

/* r = a/2 mod m */
FP256_EXPORT void ll_u256_fmod_div_by_2(u64 r[4], const u64 a[4], const u64 m[4]);

/* r = a + b mod m */
FP256_EXPORT void ll_u256_fmod_add(u64 r[4], const u64 a[4], const u64 b[4], const u64 m[4]);

/* r = a + w mod m, w is a 64bit integer */
FP256_EXPORT void ll_u256_fmod_add_limb(u64 r[4], const u64 a[4], const u64 w, const u64 m[4]);

/* r = a - b mod m */
FP256_EXPORT void ll_u256_fmod_sub(u64 r[4], const u64 a[4], const u64 b[4], const u64 m[4]);

/* r = a - w mod m, w is a 64bit integer */
FP256_EXPORT void ll_u256_fmod_sub_limb(u64 r[4], const u64 a[4], const u64 w, const u64 m[4]);

/* generate random data */
FP256_EXPORT int ll_rand_buf(unsigned char *buf, size_t len);

/* random integer in [0, 2^(64*nlimbs)) */
FP256_EXPORT int ll_rand_limbs(u64 *ad, size_t nlimbs);

/* random integer in [0, 2^(8*nbytes)) */
FP256_EXPORT int ll_rand_bytes(u64 *ad, size_t nbytes);

/* random integer in [0, 2^(nbits)) */
FP256_EXPORT int ll_rand_bits(u64 *ad, size_t nbits);

/* random integer in [0, range) */
FP256_EXPORT int ll_rand_range(u64 *ad, const u64 *range, size_t rl);

/* convert rd to hex array and print it */
FP256_EXPORT int ll_print_hex(const u64 *rd, size_t rl);

/* convert s to hex array and print it */
FP256_EXPORT void print_hex(const char *name, const unsigned char *s, size_t slen);

#ifdef __cplusplus
}
#endif

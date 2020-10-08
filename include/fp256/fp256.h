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

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include "config.h"

#define u64 unsigned long long
#define s64 signed long long
#define u32 unsigned int
#define s32 signed int
#define u8  unsigned char

#if defined(_MSC_VER)
/* for ssize_t */
# include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

#define FP256_OK      0
#define FP256_ERR    -1

#ifndef BUILD_STATIC
# ifdef _WIN32
#  ifdef BUILD_SHARED
#   define FP256_EXPORT __declspec(dllexport)
#  else
#   define FP256_EXPORT __declspec(dllimport)
#  endif
# else
#  if (defined(__GNUC__) && __GNUC__>= 4)
#   define FP256_EXPORT __attribute__ ((visibility ("default")))
#  endif
# endif
#else
# define FP256_EXPORT
#endif

#define ORDER_BIG_ENDIAN     0
#define ORDER_LITTLE_ENDIAN  1

/* useless emmm */
#define FP256_LIMBS    4
#define FP256_BYTES    8

typedef struct fp256_st {
    /* 256bit number is stored in d */
    u64 d[4];
    /* number of limbs used */
    unsigned int nlimbs;
    /* */
    int neg;
} fp256;

typedef struct mont_ctx_st {
    /* R = 2^(64w), 0 < w < 5 */
    size_t w;
    /* modulo */
    fp256 N;
    /* R^2 mod N */
    fp256 RR;
    /* -N^(-1) mod 2^64 */
    u64 k0;
} mont_ctx;

#ifdef  __cplusplus
extern "C" {
#endif

FP256_EXPORT int fp256_init(void);
FP256_EXPORT void fp256_deinit(void);
FP256_EXPORT int fp256_crit_enter(void);
FP256_EXPORT int fp256_crit_leave(void);

FP256_EXPORT int fp256_num_limbs(const fp256 *a);
FP256_EXPORT int fp256_num_bits(const fp256 *a);

/* set r = a */
FP256_EXPORT int fp256_copy(fp256 *r, const fp256 *a);

/* set r = 0 */
FP256_EXPORT int fp256_set_zero(fp256 *r);

/* set r = 1 */
FP256_EXPORT int fp256_set_one(fp256 *r);

/* set r = w */
FP256_EXPORT int fp256_set_limb(fp256 *r, const u64 limb);

/* set r = limbs */
FP256_EXPORT int fp256_set_limbs(fp256 *r, const u64 *limbs, size_t l, int neg);

/* 1  : a = 0
 * 0  : a != 0
 * -1 : error
 */ 
FP256_EXPORT int fp256_is_zero(const fp256 *a);

/* 1  : a = 1
 * 0  : a != 1
 * -1 : error
 */ 
FP256_EXPORT int fp256_is_one(const fp256 *a);

/* 1  : r is odd
 * 0  : r is even
 * -1 : error
 */ 
FP256_EXPORT int fp256_is_odd(const fp256 *a);

/* 1  : r is even
 * 0  : r is odd
 * -1 : error
 */ 
FP256_EXPORT int fp256_is_even(const fp256 *a);

/* compare two signed 256 bit integers
 * 1  : a > b
 * 0  : a = b
 * -1 : a < b
 */ 
FP256_EXPORT int fp256_cmp(const fp256 *a, const fp256 *b);

/* compare two unsigned 256 bit integers
 * 1  : a > b
 * 0  : a = b
 * -1 : a < b
 */ 
FP256_EXPORT int fp256_cmp_abs(const fp256 *a, const fp256 *b);

/* 1  : idx's bit of a is 1
 * 0  : idx's bit of a is 0
 * -1 : error
 */
FP256_EXPORT int fp256_test_bit(const fp256 *a, size_t idx);

/* set idx's bit of a to 1 */
FP256_EXPORT int fp256_set_bit(fp256 *a, size_t idx);

/* set a = 0 first, then set idx's bit of a to 1, 
 * or equivalently, set a = 2^{idx}
 */
FP256_EXPORT int fp256_clear_set_bit(fp256 *a, size_t idx);

/* clear data in fp256 structure */
FP256_EXPORT void fp256_clear(fp256 *a);

/* convert byte array to big integer */ 
FP256_EXPORT int fp256_from_bytes(fp256 *r, const u8 *bytes, size_t blen);

/* convert big integer to byte array */
FP256_EXPORT int fp256_to_bytes(u8 bytes[FP256_LIMBS*8], const fp256 *a);

/* convert hex string to big integer */
FP256_EXPORT int fp256_from_hex(fp256 *r, const u8 *hex, size_t hlen);

/* convert big integer to hex string */
FP256_EXPORT int fp256_to_hex(u8 hex[FP256_LIMBS*16], const fp256 *a);

/* convert big integer to hex string, then print it */
FP256_EXPORT int fp256_print_hex(const fp256 *a);

/* left shift a by n bits, r stores lower 256 bit result */ 
FP256_EXPORT int fp256_lshift(fp256 *r, const fp256 *a, size_t n);

/* right shift a by n bits */ 
FP256_EXPORT int fp256_rshift(fp256 *r, const fp256 *a, size_t n);

/*  */
FP256_EXPORT int fp256_rand_limbs(fp256 *r, size_t nlimbs, int neg);

/*  */
FP256_EXPORT int fp256_rand_bytes(fp256 *r, size_t nbytes, int neg);

/*  */
FP256_EXPORT int fp256_rand_bits(fp256 *r, size_t nbits, int neg);

/* */
FP256_EXPORT int fp256_rand_range(fp256 *r, const fp256 *range);

/* r = a + b, r stores lower 256 bit result */
FP256_EXPORT int fp256_add(fp256 *r, const fp256 *a, const fp256 *b);
FP256_EXPORT int fp256_add_limb(fp256 *r, const fp256 *a, const u64 b);

/* r = a - b, r stores lower 256 bit result */
FP256_EXPORT int fp256_sub(fp256 *r, const fp256 *a, const fp256 *b);
FP256_EXPORT int fp256_sub_limb(fp256 *r, const fp256 *a, const u64 b);

/* r = a * b, r stores lower 256 bit result */
FP256_EXPORT int fp256_mul(fp256 *r, const fp256 *a, const fp256 *b);

/* r = a * a, r stores lower 256 bit result */
FP256_EXPORT int fp256_sqr(fp256 *r, const fp256 *a);

/* rem = num % div
 * quo = num / div
 */ 
FP256_EXPORT int fp256_naive_div(fp256 *rem, fp256 *quo, const fp256 *num, const fp256 *div);

/* r = a mod m */
FP256_EXPORT int fp256_mod(fp256 *r, const fp256 *a, const fp256 *m);

/* r = -a mod m */
FP256_EXPORT int fp256_mod_neg(fp256 *r, const fp256 *a, const fp256 *m);

/* r = (a + b) mod m */
FP256_EXPORT int fp256_mod_add(fp256 *r, const fp256 *a, const fp256 *b, const fp256 *m);

/* r = (a - b) mod m */
FP256_EXPORT int fp256_mod_sub(fp256 *r, const fp256 *a, const fp256 *b, const fp256 *m);

/* r = (a * b) mod m */
FP256_EXPORT int fp256_mod_mul(fp256 *r, const fp256 *a, const fp256 *b, const fp256 *m);

/* r = a^2 mod m */
FP256_EXPORT int fp256_mod_sqr(fp256 *r, const fp256 *a, const fp256 *m);

/* r = a^{-1} mod m */
FP256_EXPORT int fp256_mod_inv(fp256 *r, const fp256 *a, const fp256 *m);

FP256_EXPORT int fp256_mod_exp(fp256 *r, const fp256 *a, const fp256 *exp, const fp256 *m);

/* r = gcd(a, b) */
FP256_EXPORT int fp256_gcd(fp256 *r, const fp256 *a, const fp256 *b);

/* whether a and b are coprime
 * 1  : coprime
 * 0  : not coprime
 * -1 : error
 */
FP256_EXPORT int fp256_is_coprime(const fp256 *a, const fp256 *b);

FP256_EXPORT int fp256_mont_ctx_init(mont_ctx *mctx, size_t w, const fp256 *N);
FP256_EXPORT int fp256_mont_mul(fp256 *r, const fp256 *a, const fp256 *b, const mont_ctx *mctx);
FP256_EXPORT int fp256_mont_sqr(fp256 *r, const fp256 *a, const mont_ctx *mctx);
FP256_EXPORT int fp256_to_mont(fp256 *r, const fp256 *a, const mont_ctx *mctx);
FP256_EXPORT int fp256_from_mont(fp256 *r, const fp256 *a, const mont_ctx *mctx);

FP256_EXPORT int fp256_has_neon(void);
FP256_EXPORT int fp256_has_sse2(void);
FP256_EXPORT int fp256_has_sse3(void);
FP256_EXPORT int fp256_has_avx(void);
FP256_EXPORT int fp256_has_avx2(void);
FP256_EXPORT int fp256_has_bmi2(void);
FP256_EXPORT int fp256_has_adx(void);
FP256_EXPORT int fp256_has_rdrand(void);
FP256_EXPORT int fp256_has_rdseed(void);

#ifdef  __cplusplus
}
#endif
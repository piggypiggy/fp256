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
 * @file fp256.h
 */

#pragma once

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

/** 64 bit unsigned integer */
#define u64 unsigned long long
/** 64 bit signed integer */
#define s64 signed long long
/** 32 bit unsigned integer */
#define u32 unsigned int
/** 32 bit signed integer */
#define s32 signed int
/** 8 bit unsigned character */
#define u8  unsigned char

#if defined(_MSC_VER)
/* for ssize_t */
# include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

/* TODO : error code? */
#define FP256_OK      0
#define FP256_ERR    -1

#if defined(BUILD_STATIC)
# define FP256_EXPORT
#else
# if defined(_MSC_VER)
#  if defined(BUILD_SHARED)
#   define FP256_EXPORT __declspec(dllexport)
#  else
#   define FP256_EXPORT __declspec(dllimport)
#  endif
# elif (defined(__GNUC__) && __GNUC__>= 4)
#  define FP256_EXPORT __attribute__ ((visibility ("default")))
# else
#  define FP256_EXPORT
# endif
#endif

#define ORDER_BIG_ENDIAN     0
#define ORDER_LITTLE_ENDIAN  1

/* useless emmm */
#define FP256_LIMBS    4
#define FP256_BYTES    8

struct fp256;
struct mont_ctx;

typedef struct fp256 fp256;
typedef struct mont_ctx mont_ctx;

/**
 * structure for 256 bit integer type.
 */
struct fp256 {
    /** d stores 256 bit integer, it has four 64 bit limbs. */
    u64 d[4];
    /** number of limbs used */
    unsigned int nlimbs;
};

/**
 * struct for precomputed value for montgomery multiplication.
 */
struct mont_ctx {
    /** R = 2^(64w), 0 < w < 5 */
    size_t w;
    /** odd modulus */
    fp256 N;
    /** R^2 mod N */
    fp256 RR;
    /** -N^(-1) mod 2^64 */
    u64 k0;
};

#ifdef  __cplusplus
extern "C" {
#endif

/**
 * library initialization. It should be called only once.
 * 
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_init(void);

/** library initialization. */
FP256_EXPORT void fp256_deinit(void);

/**
 * enter critical section.
 * 
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_crit_enter(void);

/**
 * leave critical section.
 * 
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_crit_leave(void);

/**
 * count number of limbs of a.
 * 
 * @param[in] a           - 256 bit integer.
 * @return number of limbs.
 */
FP256_EXPORT size_t fp256_num_limbs(const fp256 *a);

/**
 * count number of bits of a.
 * 
 * @param[in] a           - 256 bit integer.
 * @return number of bits.
 */
FP256_EXPORT size_t fp256_num_bits(const fp256 *a);

/** 
 * set r = a.
 * 
 * @param[out] r          - destination 256 bit integer.
 * @param[in] a           - source 256 bit integer.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_copy(fp256 *r, const fp256 *a);

/** 
 * set r = 0.
 * 
 * @param[in,out] r       - result.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_set_zero(fp256 *r);

/**
 * set r = 1.
 * 
 * @param[in,out] r       - result.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_set_one(fp256 *r);

/**
 * set r = limb.
 * 
 * @param[out] r          - result.
 * @param[in] limb        - 64 bit integer.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_set_limb(fp256 *r, const u64 limb);

/**
 * set r = limbs.
 * 
 * @param[out] r          - result.
 * @param[in] limbs       - 64 bit integer array.
 * @param[in] l           - array length.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_set_limbs(fp256 *r, const u64 *limbs, size_t l);

/**
 * compare a with 0.
 * 
 * @param[in] a           - the 256 bit integer to compare.
 * @return 
 * - 1  if a = 0
 * - 0  if a != 0
 * - -1 otherwise.
 */ 
FP256_EXPORT int fp256_is_zero(const fp256 *a);

/**
 * compare a with 1.
 * 
 * @param[in] a           - the 256 bit integer to compare.
 * @return 
 * - 1  if a = 1
 * - 0  if a != 1
 * - -1 otherwise.
 */ 
FP256_EXPORT int fp256_is_one(const fp256 *a);

/**
 * test if a is odd.
 * 
 * @param[in] a           - the 256 bit integer to test.
 * @return 
 * - 1  if a is odd
 * - 0  if a is even
 * - -1 otherwise.
 */ 
FP256_EXPORT int fp256_is_odd(const fp256 *a);

/**
 * test if a is even.
 * 
 * @param[in] a           - the 256 bit integer to test.
 * @return 
 * - 1  if a is even
 * - 0  if a is odd
 * - -1 otherwise.
 */ 
FP256_EXPORT int fp256_is_even(const fp256 *a);

/**
 * compare two unsigned 256 bit integers
 * 
 * @param[in] a           - 256 bit integer.
 * @param[in] b           - 256 bit integer.
 * @return 
 * - 1  if a > b
 * - 0  if a = b
 * - -1 if a < b.
 */ 
FP256_EXPORT int fp256_cmp(const fp256 *a, const fp256 *b);

/**
 * test if idx's bit of a is 1.
 * 
 * @param[in] a           - the 256 bit integer to test.
 * @param[in] idx         - which bit to test.
 * @return 
 * - 1  if idx's bit of a is 1.
 * - 0  if idx's bit of a is 0.
 * - -1 otherwise.
 */ 
FP256_EXPORT int fp256_test_bit(const fp256 *a, size_t idx);

/**
 * set idx's bit of a to 1.
 * 
 * @param[out] a          - result.
 * @param[in] idx         - which bit to set 1.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */ 
FP256_EXPORT int fp256_set_bit(fp256 *a, size_t idx);

/**
 * set a = 0 first, then set idx's bit of a to 1, 
 * or equivalently, set a = 2^{idx}.
 * 
 * @param[out] a          - result.
 * @param[in] idx         - which bit to set 1.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */ 
FP256_EXPORT int fp256_clear_set_bit(fp256 *a, size_t idx);

/** clear data in fp256 structure */
FP256_EXPORT void fp256_cleanse(fp256 *a);

/** 
 * convert byte array to 256 bit integer.
 * 
 * @param[out] r          - result.
 * @param[in] bytes       - the byte array to convert.
 * @param[in] blen        - length of byte array.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */ 
FP256_EXPORT int fp256_from_bytes(fp256 *r, const u8 *bytes, size_t blen);

/** 
 * convert 256 bit integer to byte array.
 * 
 * @param[out] bytes      - result.
 * @param[in] a           - the 256 bit integer to convert.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */ 
FP256_EXPORT int fp256_to_bytes(u8 bytes[32], const fp256 *a);

/** 
 * convert hex array to 256 bit integer.
 * 
 * @param[out] r          - result.
 * @param[in] hex         - the hex array to convert.
 * @param[in] hlen        - length of hex array.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */ 
FP256_EXPORT int fp256_from_hex(fp256 *r, const u8 *hex, size_t hlen);

/** 
 * convert 256 bit integer to hex array.
 * 
 * @param[out] hex        - result.
 * @param[in] a           - the 256 bit integer to convert.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */ 
FP256_EXPORT int fp256_to_hex(u8 hex[64], const fp256 *a);

/**
 * convert 256 bit integer to hex array, then print it.
 * 
 * @param[in] a           - the 256 bit integer to print.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_print_hex(const fp256 *a);

/**
 * print parameters in montgomery context.
 * 
 * @param[in] mctx        - the montgomery context to print.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_print_mont_ctx(const mont_ctx *mctx);

/** 
 * left shift a by n bits, r stores lower 256 bit result.
 * 
 * @param[out] r          - result.
 * @param[in] a           - the 256 bit integer to shift.
 * @param[in] n           - number of bits to shift.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */ 
FP256_EXPORT int fp256_lshift(fp256 *r, const fp256 *a, size_t n);

/** 
 * right shift a by n bits, r stores lower 256 bit result.
 * 
 * @param[out] r          - result.
 * @param[in] a           - the 256 bit integer to shift.
 * @param[in] n           - number of bits to shift.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */ 
FP256_EXPORT int fp256_rshift(fp256 *r, const fp256 *a, size_t n);

/**
 * generate random integer in range [0, 2^(64*nlimbs)).
 * 
 * @param[out] r          - result.
 * @param[in] nlimbs      - number of limbs of result.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 * */
FP256_EXPORT int fp256_rand_limbs(fp256 *r, size_t nlimbs);

/**
 * generate random integer in range [0, 2^(8*nbytes)).
 * 
 * @param[out] r          - result.
 * @param[in] nlimbs      - number of bytes of result.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 * */
FP256_EXPORT int fp256_rand_bytes(fp256 *r, size_t nbytes);

/**
 * generate random integer in range [0, 2^(nbits)).
 * 
 * @param[out] r          - result.
 * @param[in] nlimbs      - number of bits of result.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 * */
FP256_EXPORT int fp256_rand_bits(fp256 *r, size_t nbits);

/**
 * generate random integer in range [0, range).
 * 
 * @param[out] r          - result.
 * @param[in] range       - 256 bit integer.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 * */
FP256_EXPORT int fp256_rand_range(fp256 *r, const fp256 *range);

/**
 * compute r = (a + b) mod 2^256.
 * 
 * @param[out] r          - result.
 * @param[in] a           - 256 bit integer.
 * @param[in] b           - 256 bit integer.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_add(fp256 *r, const fp256 *a, const fp256 *b);

/**
 * compute r = (a + b) mod 2^256.
 * 
 * @param[out] r          - result.
 * @param[in] a           - 256 bit integer.
 * @param[in] b           - 64 bit integer.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_add_limb(fp256 *r, const fp256 *a, const u64 b);

/**
 * compute r = (a - b) mod 2^256.
 * 
 * @param[out] r          - result.
 * @param[in] a           - 256 bit integer.
 * @param[in] b           - 256 bit integer.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_sub(fp256 *r, const fp256 *a, const fp256 *b);

/**
 * compute r = (a - b) mod 2^256.
 * 
 * @param[out] r          - result.
 * @param[in] a           - 256 bit integer.
 * @param[in] b           - 64 bit integer.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_sub_limb(fp256 *r, const fp256 *a, const u64 b);

/**
 * compute r = a * b, r stores lower 256 bit result.
 * 
 * @param[out] r          - result.
 * @param[in] a           - 256 bit integer.
 * @param[in] b           - 256 bit integer.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_mullo(fp256 *r, const fp256 *a, const fp256 *b);

/**
 * compute r = a * b, r stores upper 256 bit result.
 * 
 * @param[out] r          - result.
 * @param[in] a           - 256 bit integer.
 * @param[in] b           - 256 bit integer.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_mulhi(fp256 *r, const fp256 *a, const fp256 *b);

/**
 * compute a * b, rhi stores upper 256 bit result, rlo stores lower 256 bit result.
 * 
 * @param[out] rhi        - result.
 * @param[out] rlo        - result.
 * @param[in] a           - 256 bit integer.
 * @param[in] b           - 256 bit integer.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_mul(fp256 *rhi, fp256 *rlo, const fp256 *a, const fp256 *b);

/**
 * compute r = a^2, r stores lower 256 bit result.
 * 
 * @param[out] r          - result.
 * @param[in] a           - 256 bit integer.
 * @param[in] b           - 256 bit integer.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_sqrlo(fp256 *r, const fp256 *a);

/**
 * compute r = a^2, r stores upper 256 bit result.
 * 
 * @param[out] r          - result.
 * @param[in] a           - 256 bit integer.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_sqrhi(fp256 *r, const fp256 *a);

/**
 * compute a^2, rhi stores upper 256 bit result, rlo stores lower 256 bit result.
 * 
 * @param[out] rhi        - result.
 * @param[out] rlo        - result.
 * @param[in] a           - 256 bit integer.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_sqr(fp256 *rhi, fp256 *rlo, const fp256 *a);

/**
 * compute rem = num % div, quo = floor(num / div). \n
 * It relpaces division with multiplication by reciprocal,
 * see https://gmplib.org/~tege/division-paper.pdf.
 * 
 * @param[out] rem        - remainder.
 * @param[out] quo        - quotient.
 * @param[in] num         - numerator.
 * @param[in] div         - divisor.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_div(fp256 *rem, fp256 *quo, const fp256 *num, const fp256 *div);

/**
 * compute rem = num % div, quo = floor(num / div).
 * 
 * @param[out] rem        - remainder.
 * @param[out] quo        - quotient.
 * @param[in] num         - numerator.
 * @param[in] div         - divisor.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_naive_div(fp256 *rem, fp256 *quo, const fp256 *num, const fp256 *div);

/** 
 * compute r = a % m.
 * 
 * @param[out] r          - remainder.
 * @param[in] a           - the 256 bit integer to reduce.
 * @param[in] m           - modulo.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_mod(fp256 *r, const fp256 *a, const fp256 *m);

/** 
 * compute r = -a % m.
 * 
 * @param[out] r          - remainder.
 * @param[in] a           - the 256 bit integer to reduce.
 * @param[in] m           - modulus.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_mod_neg(fp256 *r, const fp256 *a, const fp256 *m);

/** 
 * compute r = (a + b) % m.
 * 
 * @param[out] r          - remainder.
 * @param[in] a           - the first 256 bit integer to add.
 * @param[in] b           - the second 256 bit integer to add.
 * @param[in] m           - modulus.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_mod_add(fp256 *r, const fp256 *a, const fp256 *b, const fp256 *m);

/** 
 * compute r = (a - b) % m.
 * 
 * @param[out] r          - remainder.
 * @param[in] a           - the first 256 bit integer to add.
 * @param[in] b           - the second 256 bit integer to sub.
 * @param[in] m           - modulus.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_mod_sub(fp256 *r, const fp256 *a, const fp256 *b, const fp256 *m);

/** 
 * compute r = (a * b) % m.
 * 
 * @param[out] r          - remainder.
 * @param[in] a           - the first 256 bit integer to multiply.
 * @param[in] b           - the second 256 bit integer to multiply.
 * @param[in] m           - modulus.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_mod_mul(fp256 *r, const fp256 *a, const fp256 *b, const fp256 *m);

/** 
 * compute r = a^2 % m.
 * 
 * @param[out] r          - remainder.
 * @param[in] a           - the 256 bit integer to square.
 * @param[in] m           - modulus.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_mod_sqr(fp256 *r, const fp256 *a, const fp256 *m);

/** 
 * compute r = a^{-1} % m.
 * 
 * @param[out] r          - inverse.
 * @param[in] a           - the 256 bit integer to invert.
 * @param[in] m           - modulus.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_mod_inv(fp256 *r, const fp256 *a, const fp256 *m);

/** 
 * compute r = a^e % m.
 * 
 * @param[out] r          - result.
 * @param[in] a           - base.
 * @param[in] e           - exponential.
 * @param[in] m           - modulus.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_mod_exp(fp256 *r, const fp256 *a, const fp256 *e, const fp256 *m);

/** 
 * compute r = greatest common divisor of a and b.
 * 
 * @param[out] r          - gcd.
 * @param[in] a           - 256 bit integer.
 * @param[in] b           - 256 bit integer.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_gcd(fp256 *r, const fp256 *a, const fp256 *b);

/** 
 * check if a and b are coprime.
 * 
 * @param[in] a           - 256 bit integer.
 * @param[in] b           - 256 bit integer.
 * @return 
 * - 1  a and b are coprime
 * - 0  a and b are not coprime
 * - -1 otherwise.
 */
FP256_EXPORT int fp256_is_coprime(const fp256 *a, const fp256 *b);

/** 
 * initialize precomputed values for montgomery multiplication.
 * R = 2^(64w),
 * mctx->RR = 2^(128w) mod N,
 * mctx->k0 = -N^{-1} mod 2^64.
 * 
 * @param[out] mctx       - montgomery context.
 * @param[in] w           - 256 bit integer.
 * @param[in] N           - odd modulus.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_mont_ctx_init(mont_ctx *mctx, size_t w, const fp256 *N);

/**
 * montgomery multiplication, r = A * B * R^{-1} mod N 
 * 
 * @param[out] r          - result.
 * @param[in] A           - the first integer(in montgomery form) to multiply.
 * @param[in] B           - the second integer(in montgomery form) to multiply.
 * @param[in] mctx        - montgomery context.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_mont_mul(fp256 *r, const fp256 *A, const fp256 *B, const mont_ctx *mctx);

/**
 * montgomery multiplication, r = A * A * R^{-1} mod N 
 * 
 * @param[out] r          - result.
 * @param[in] A           - the integer(in montgomery form) to multiply.
 * @param[in] mctx        - montgomery context.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_mont_sqr(fp256 *r, const fp256 *A, const mont_ctx *mctx);

/**
 * montgomery exponential, r = A^e * R mod N 
 * 
 * @param[out] r          - result.
 * @param[in] A           - the base(in montgomery form).
 * @param[in] e           - the exponent.
 * @param[in] mctx        - montgomery context.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_mont_exp(fp256 *r ,const fp256 *A, const fp256 *e, const mont_ctx *mctx);

/**
 * transform integer a to montgomery form
 * 
 * @param[out] A          - result.
 * @param[in] a           - the 256 bit integer to transform.
 * @param[in] mctx        - montgomery context.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_to_mont(fp256 *A, const fp256 *a, const mont_ctx *mctx);

/**
 * transform integer A from montgomery form
 * 
 * @param[out] a          - result.
 * @param[in] A           - the 256 bit integer to transform.
 * @param[in] mctx        - montgomery context.
 * @return #FP256_OK if succeeded, #FP256_ERR otherwise.
 */
FP256_EXPORT int fp256_from_mont(fp256 *a, const fp256 *A, const mont_ctx *mctx);

/**
 * arm processor neon flag.
 * 
 * @return 
 * - 1 if processor supports neon instruction.
 * - 0 otherwise.
 */
FP256_EXPORT int fp256_has_neon(void);

/**
 * x86 processor sse2 flag.
 * 
 * @return 
 * - 1 if processor supports sse2 instruction.
 * - 0 otherwise.
 */
FP256_EXPORT int fp256_has_sse2(void);

/**
 * x86 processor sse3 flag.
 * 
 * @return 
 * - 1 if processor supports sse3 instruction.
 * - 0 otherwise.
 */
FP256_EXPORT int fp256_has_sse3(void);

/**
 * x86 processor avx flag.
 * 
 * @return 
 * - 1 if processor supports avx instruction.
 * - 0 otherwise.
 */
FP256_EXPORT int fp256_has_avx(void);

/**
 * x86 processor avx2 flag.
 * 
 * @return 
 * - 1 if processor supports avx2 instruction.
 * - 0 otherwise.
 */
FP256_EXPORT int fp256_has_avx2(void);

/**
 * x86 processor bmi2 flag.
 * 
 * @return 
 * - 1 if processor supports bmi2 instruction.
 * - 0 otherwise.
 */
FP256_EXPORT int fp256_has_bmi2(void);

/**
 * x86 processor adx flag.
 * 
 * @return 
 * - 1 if processor supports adx instruction.
 * - 0 otherwise.
 */
FP256_EXPORT int fp256_has_adx(void);

/**
 * x86 processor rdrand flag.
 * 
 * @return 
 * - 1 if processor supports rdrand instruction.
 * - 0 otherwise.
 */
FP256_EXPORT int fp256_has_rdrand(void);

/**
 * x86 processor rdseed flag.
 * 
 * @return 
 * - 1 if processor supports rdseed instruction.
 * - 0 otherwise.
 */
FP256_EXPORT int fp256_has_rdseed(void);

#ifdef  __cplusplus
}
#endif

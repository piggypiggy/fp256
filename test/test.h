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

#pragma once

#include <fp256/fp256.h>
#include <fp256/fp256_ll.h>
#include <stdio.h>
#include <time.h>
#include "simple_thread.h"

#ifdef __cplusplus
extern "C" {
#endif

#define RETURN_IF_ERROR(TEST)    \
    do {                         \
        if (TEST != FP256_OK) {  \
            fp256_deinit();      \
            return -1;           \
        }                        \
    } while(0);

typedef struct do_which_st {
    /* do which test */
    int do_u256add;
    int do_lladd;
    int do_add;
    int do_u256mul;
    int do_llmul;
    int do_mullo;
    int do_sqrlo;
    int do_u256sqr;
    int do_div;
    int do_naive_div;
    int do_gcd;
    int do_rand;
    int do_shift;
    int do_convert;
    int do_mod_add;
    int do_mod_mul;
    int do_mod_sqr;
    int do_mod_inv;
    int do_mod_exp;
    int do_fmod_add;
    int do_mont_mul;
    int do_mont_sqr;
} DO_WHICH;

typedef struct test_args_st {
    /* number of tests */
    int64_t N;
    /* number of threads */
    int64_t T;
    /* */
    DO_WHICH do_which;
} TEST_ARGS;

typedef struct {
    char *name;
    void* (*setup) (void);
    void (*run) (void *data, int64_t N);
    void (*clear) (void *data);
    int64_t N;
    int64_t T;
    int ok;
} TEST_THREAD_DATA;

/* start n threads */
int test_start_n_thread(void* (*func)(void*), TEST_THREAD_DATA *thread_data);

void test_rand_init(void);
void test_random_string(u8 *s, int len);
void test_random_hex_string(u8 *s, int len);
int test_random_number(void);

int get_test_args(int argc, char **argv, TEST_ARGS *args);
void set_test_args(TEST_ARGS *args, int64_t N, int64_t t);

void test_print_hex(const char *desp, const u64 *ad, size_t al);
void test_fp256_print_hex(const char *desp, const fp256 *a);
void test_fp256_print_mont_ctx(const char *desp, const mont_ctx *mctx);

int run_test(char *name, int(*test_case)(void), void*(*test_func)(void*), int64_t N, int64_t T);

#ifdef __cplusplus
}
#endif

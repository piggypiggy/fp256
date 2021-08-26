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

#include "test.h"

static const char *bench_help_string = "\
usage : bench [arithmetic] -n [num_test] -t [num_thread] \n\
\n\
arithmetic : add u256add modadd \n\
             mullo sqrlo u256mul u256sqr modmul modsqr \n\
             shift u256shift \n\
             div naivediv \n\
             gcd modinv \n\
             montmul montsqr \n\
             modexp \n\
-n         : number of tests, default is 200000. \n\
-t         : number of threads, default is 0, single thread. \n\
";

/* 0->'0', 1->'1', ... , 15->'F' */
static const u8 ascii_table[16] = {
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
};

/* initialize seed using prng.
 * we need random number rather than secure random number in tests,
 * so rand() is used, it's much faster than ll_rand_buf.
 */
void test_rand_init(void)
{
    unsigned int seed;
    ll_rand_buf((u8*)&seed, sizeof(unsigned int));
    srand(seed);
}

void test_random_string(u8 *s, int len)
{
    while (len--)
        s[len] = rand() % 256;
}

void test_random_hex_string(u8 *s, int len)
{
    while (len--)
        s[len] = ascii_table[rand() % 16];
}

int test_random_number(void)
{
    return rand();
}

/* compare two strings(not case-sensitive),
 * return length of string if they are equal,
 * return 0 if not equal.
 */
static size_t arg_cmp(const char *arg1, const char *arg2)
{
    size_t i, len1, len2;
    char c1, c2;

    len1 = strlen(arg1);
    len2 = strlen(arg2);
    if (len1 != len2)
        return 0;

    for (i = 0; i < len1; i++) {
        /* to upper case */
        c1 = arg1[i];
        if (0x61 <= c1 && c1 <= 0x7a)
            c1 -= 0x20;

        c2 = arg2[i];
        if (0x61 <= c2 && c2 <= 0x7a)
            c2 -= 0x20;

        if (c1 != c2)
            return 0;
    }

    return len1;
}

int get_test_args(int argc, char **argv, TEST_ARGS *args)
{
    int do_all = 1;
    // size_t len;
    DO_WHICH *do_which;
    memset(args, 0, sizeof(TEST_ARGS));
    do_which = &args->do_which;

    if (argc == 1) {
        /* do all speed test */
        memset(do_which, 1, sizeof(DO_WHICH));
    }
    else {
        for (int i = 1; i < argc; i++) {
            if (memcmp(argv[i], "-n", 2) == 0 || memcmp(argv[i], "-N", 2) == 0) {
                i++;
                if (i < argc) {
                    args->N = atoi(argv[i]);
                    if (args->N <= 0)
                        goto err;
                }
                else
                    goto err;
                continue;
            }

            if (memcmp(argv[i], "-t", 2) == 0 || memcmp(argv[i], "-T", 2) == 0) {
                i++;
                if (i < argc) {
                    args->T = atoi(argv[i]);
                    if (args->T < 0)
                        goto err;
                }
                else
                    goto err;
                continue;
            }

            /* for benchmark */
            if (arg_cmp(argv[i], "add") > 0) {
                do_which->do_add = 1;
                do_all = 0;
                continue;
            }

            if (arg_cmp(argv[i], "lladd") > 0 || arg_cmp(argv[i], "ll_add") > 0) {
                do_which->do_lladd = 1;
                do_all = 0;
                continue;
            }

            if (arg_cmp(argv[i], "u256add") > 0 || arg_cmp(argv[i], "u256_add") > 0) {
                do_which->do_u256add = 1;
                do_all = 0;
                continue;
            }

            if (arg_cmp(argv[i], "convert") > 0) {
                do_which->do_convert = 1;
                do_all = 0;
                continue;
            }

            if (arg_cmp(argv[i], "mullo") > 0) {
                do_which->do_mullo = 1;
                do_all = 0;
                continue;
            }

            if (arg_cmp(argv[i], "sqrlo") > 0) {
                do_which->do_sqrlo = 1;
                do_all = 0;
                continue;
            }

            if (arg_cmp(argv[i], "llmul") > 0 || arg_cmp(argv[i], "ll_mul") > 0) {
                do_which->do_llmul = 1;
                do_all = 0;
                continue;
            }

            if (arg_cmp(argv[i], "u256mul") > 0 || arg_cmp(argv[i], "u256_mul") > 0) {
                do_which->do_u256mul = 1;
                do_all = 0;
                continue;
            }


            if (arg_cmp(argv[i], "div") > 0) {
                do_which->do_div = 1;
                do_all = 0;
                continue;
            }

            if (arg_cmp(argv[i], "naivediv") > 0 || arg_cmp(argv[i], "naive_div") > 0) {
                do_which->do_naive_div = 1;
                do_all = 0;
                continue;
            }

            if (arg_cmp(argv[i], "gcd") > 0) {
                do_which->do_gcd = 1;
                do_all = 0;
                continue;
            }

            if (arg_cmp(argv[i], "rand") > 0) {
                do_which->do_rand = 1;
                do_all = 0;
                continue;
            }

            if (arg_cmp(argv[i], "shift") > 0) {
                do_which->do_shift = 1;
                do_all = 0;
                continue;
            }

            if (arg_cmp(argv[i], "modadd") > 0 || arg_cmp(argv[i], "mod_add") > 0) {
                do_which->do_mod_add = 1;
                do_all = 0;
                continue;
            }

            if (arg_cmp(argv[i], "modmul") > 0 || arg_cmp(argv[i], "mod_mul") > 0) {
                do_which->do_mod_mul = 1;
                do_all = 0;
                continue;
            }

            if (arg_cmp(argv[i], "modsqr") > 0 || arg_cmp(argv[i], "mod_sqr") > 0) {
                do_which->do_mod_sqr = 1;
                do_all = 0;
                continue;
            }

            if (arg_cmp(argv[i], "modinv") > 0 || arg_cmp(argv[i], "mod_inv") > 0) {
                do_which->do_mod_inv = 1;
                do_all = 0;
                continue;
            }

            if (arg_cmp(argv[i], "modexp") > 0 || arg_cmp(argv[i], "mod_exp") > 0) {
                do_which->do_mod_exp = 1;
                do_all = 0;
                continue;
            }

            if (arg_cmp(argv[i], "fmodadd") > 0 || arg_cmp(argv[i], "fmod_add") > 0) {
                do_which->do_fmod_add = 1;
                do_all = 0;
                continue;
            }

            if (arg_cmp(argv[i], "montmul") > 0 || arg_cmp(argv[i], "mont_mul") > 0) {
                do_which->do_mont_mul = 1;
                do_all = 0;
                continue;
            }

            if (arg_cmp(argv[i], "montsqr") > 0 || arg_cmp(argv[i], "mont_sqr") > 0) {
                do_which->do_mont_sqr = 1;
                do_all = 0;
                continue;
            }

            if (arg_cmp(argv[i], "-h") > 0 || arg_cmp(argv[i], "-help") > 0) {
                printf("%s \n", bench_help_string);
                do_all = 0;
                continue;
            }

            goto err;
        }
    }

    /* default thread = 0 */
    args->T = args->T >= 0 ? args->T : 4;
    /* default N = 200000 */
    args->N = args->N > 0 ? args->N : 200000;

    if (do_all) {
        /* do all speed test */
        memset(do_which, 1, sizeof(DO_WHICH));
    }
    return 0;

err:
    printf("invalid argument\n");
    printf("%s \n", bench_help_string);
    return -1;
}

void set_test_args(TEST_ARGS *args, int64_t N, int64_t T)
{
    if (N > 0)
        args->N = N;

    if (T >= 0)
        args->T = T;
}

void test_print_hex(const char *desp, const u64 *ad, size_t al)
{
    printf("%s", desp);
    ll_print_hex(ad, al);
}

void test_fp256_print_hex(const char *desp, const fp256 *a)
{
    printf("%s", desp);
    fp256_print_hex(a);
}

void test_fp256_print_mont_ctx(const char *desp, const mont_ctx *mctx)
{
    printf("%s", desp);
    fp256_print_mont_ctx(mctx);
}

int run_func(char *name, void*(*func)(void*), int64_t N, int64_t T)
{
    TEST_THREAD_DATA thread_data;

    thread_data.name = name;
    thread_data.N = N;
    thread_data.T = T;
    thread_data.ok = FP256_ERR;

    /* single thread */
    printf("\nsinglethread N = %" PRId64 "\n", N);
    func((void*)&thread_data);
    if (thread_data.ok != FP256_OK)
        return FP256_ERR;

    /* multi thread */
    if (T > 0) {
        printf("\nmultithread T = %" PRId64 "\n", T);
        thread_data.ok = FP256_OK;
        test_start_n_thread(func, (void*)&thread_data);
        if (thread_data.ok != FP256_OK)
            return FP256_ERR;
    }

    return FP256_OK;
}

int run_test(char *name, int(*test_case)(void), void*(*test_func)(void*), int64_t N, int64_t T)
{
    printf("------------ %s ------------ \n", name);

    if (test_case != NULL) {
        if (test_case() != FP256_OK) {
            return FP256_ERR;
        }
    }

    if (test_func != NULL)
        return run_func(name, test_func, N, T);

    return FP256_OK;
}

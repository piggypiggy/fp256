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

#include "test.h"

/* 0->'0', 1->'1', ... , 15->'F' */
static const unsigned char ascii_table[16] = {
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
    ll_rand_buf((unsigned char*)&seed, sizeof(unsigned int));
    srand(seed);
}

void test_random_string(unsigned char *s, int len)
{
    while (len--)
        s[len] = rand() % 256;
}

void test_random_hex_string(unsigned char *s, int len)
{
    while (len--)
        s[len] = ascii_table[rand() % 16];
}

int test_random_number(void)
{
    return rand();
}

void get_test_args(int argc, char **argv, TEST_ARGS *args)
{
    int do_all = 1;
    DO_WHICH *do_which;
    memset(args, 0, sizeof(TEST_ARGS));
    do_which = &args->do_which;

    if (argc == 1) {
        /* do all speed test */
        memset(do_which, 1, sizeof(DO_WHICH));
    }

    for (int i = 1; i < argc; i++) {
        if (memcmp(argv[i], "t=", 2) == 0 || memcmp(argv[i], "T=", 2) == 0) {
            args->T = atoi(argv[i] + 2);
            continue;
        }

        if (memcmp(argv[i], "n=", 2) == 0 || memcmp(argv[i], "N=", 2) == 0) {
            args->N = atoi(argv[i] + 2);
            continue;
        }

        if (memcmp(argv[i], "add", 3) == 0) {
            do_which->do_add = 1;
            do_all = 0;
            continue;
        }

        if (memcmp(argv[i], "convert", 7) == 0) {
            do_which->do_convert = 1;
            do_all = 0;
            continue;
        }

        if (memcmp(argv[i], "mul", 3) == 0) {
            do_which->do_mul = 1;
            do_all = 0;
            continue;
        }

        if (memcmp(argv[i], "sqr", 3) == 0) {
            do_which->do_sqr = 1;
            do_all = 0;
            continue;
        }

        if (memcmp(argv[i], "div", 3) == 0) {
            printf("please do naivediv test\n");
            do_which->do_div = 1;
            do_all = 0;
            continue;
        }

        if (memcmp(argv[i], "naivediv", 8) == 0 || memcmp(argv[i], "naive_div", 9) == 0) {
            do_which->do_naive_div = 1;
            do_all = 0;
            continue;
        }

        if (memcmp(argv[i], "gcd", 3) == 0) {
            do_which->do_gcd = 1;
            do_all = 0;
            continue;
        }

        if (memcmp(argv[i], "rand", 4) == 0) {
            do_which->do_rand = 1;
            do_all = 0;
            continue;
        }

        if (memcmp(argv[i], "shift", 5) == 0) {
            do_which->do_shift = 1;
            do_all = 0;
            continue;
        }

        if (memcmp(argv[i], "modadd", 6) == 0 || memcmp(argv[i], "mod_add", 7) == 0) {
            do_which->do_mod_add = 1;
            do_all = 0;
            continue;
        }

        if (memcmp(argv[i], "modmul", 6) == 0 || memcmp(argv[i], "mod_mul", 7) == 0) {
            do_which->do_mod_mul = 1;
            do_all = 0;
            continue;
        }

        if (memcmp(argv[i], "modsqr", 6) == 0 || memcmp(argv[i], "mod_sqr", 7) == 0) {
            do_which->do_mod_sqr = 1;
            do_all = 0;
            continue;
        }

        if (memcmp(argv[i], "modinv", 6) == 0 || memcmp(argv[i], "mod_inv", 7) == 0) {
            do_which->do_mod_inv = 1;
            do_all = 0;
            continue;
        }

        if (memcmp(argv[i], "fmodadd", 7) == 0 || memcmp(argv[i], "fmod_add", 8) == 0) {
            do_which->do_fmod_add = 1;
            do_all = 0;
            continue;
        }

        if (memcmp(argv[i], "montmul", 7) == 0 || memcmp(argv[i], "mont_mul", 8) == 0) {
            do_which->do_mont_mul = 1;
            do_all = 0;
            continue;
        }

        if (memcmp(argv[i], "montsqr", 7) == 0 || memcmp(argv[i], "mont_sqr", 8) == 0) {
            do_which->do_mont_sqr = 1;
            do_all = 0;
            continue;
        }

        /* TODO : add help */
    }

    /* default thread = 0 */
    args->T = args->T >= 0 ? args->T : 4;
    /* default N = 200000 */
    args->N = args->N > 0 ? args->N : 200000;

    if(do_all) {
        /* do all speed test */
        memset(do_which, 1, sizeof(DO_WHICH));
    }
}

void set_test_args(TEST_ARGS *args, int64_t N, int64_t T)
{
    if (N > 0)
        args->N = N;

    if (T >= 0)
        args->T = T;
}

void test_fp256_print_hex(const char *desp, const fp256 *a)
{
    printf("%s", desp);
    fp256_print_hex(a);
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
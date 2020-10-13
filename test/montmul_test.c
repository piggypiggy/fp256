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
#include "test_local.h"

#include "test.h"
#include "test_local.h"

void* fp256_mont_mul_test(void *p)
{
    int64_t i, N;
    fp256 r, tr1, tr2, a, b, ta, tb, A, B, odd, mod, k0;
    size_t al, bl, ol, w;
    mont_ctx mctx;
    TEST_THREAD_DATA *td;

    td = (TEST_THREAD_DATA*)p;
    N = td->N;

    /* mod = 2^64 */
    fp256_clear_set_bit(&mod, 64);
    for (i = 0; i < N; i++) {
        /* random odd modulo and corresponding R */
        do {
            al = rand() % 4 + 1;
            fp256_rand_limbs(&a, al, 0);
            bl = rand() % 4 + 1;
            fp256_rand_limbs(&b, bl, 0);
            ol = 4;
            fp256_rand_limbs(&odd, ol, 0);
            w = ol;
        } while (fp256_is_odd(&odd) != 1);
        fp256_mont_ctx_init(&mctx, w, &odd);
        /* a, b should be less than modulo */
        fp256_mod(&a, &a, &odd);
        fp256_mod(&b, &b, &odd);

        /* check k0 * N = -1 mod 2^64 */
        fp256_set_limb(&k0, mctx.k0);
        /* k0 * N mod 2^64 */
        fp256_mod_mul(&r, &k0, &mctx.N, &mod);
        fp256_mod_neg(&r, &r, &mod);
        if (fp256_is_one(&r) != 1) {
            printf("fp256_mont_mul check k0 %" PRId64 " failed\n", i + 1);
            test_fp256_print_mont_ctx("mont ctx:\n", &mctx);
            test_fp256_print_hex("r  = ", &r);
            goto end;
        }

        /* A = aR mod N */
        fp256_to_mont(&A, &a, &mctx);
        /* ta = a */
        fp256_from_mont(&ta, &A, &mctx);
        /* B = bR mod N */
        fp256_to_mont(&B, &b, &mctx);
        /* ta = a */
        fp256_from_mont(&tb, &B, &mctx);
        /* r = abR mod N */
        fp256_mont_mul(&r, &A, &B, &mctx);
        /* tr1 = Ab mod N = abR mod N */
        fp256_mod_mul(&tr1, &A, &b, &mctx.N);
        /* tr2 = aB mod N = abR mod N */
        fp256_mod_mul(&tr2, &a, &B, &mctx.N);

        /* check a = ta, b = tb */
        if (fp256_cmp(&a, &ta) != 0 || fp256_cmp(&b, &tb) != 0) {
            printf("fp256_from_mont %" PRId64 " failed\n", i + 1);
            test_fp256_print_mont_ctx("mont ctx:\n", &mctx);
            test_fp256_print_hex("a   = ", &a);
            test_fp256_print_hex("A   = ", &A);
            test_fp256_print_hex("ta  = ", &ta);
            test_fp256_print_hex("b   = ", &b);
            test_fp256_print_hex("B   = ", &B);
            test_fp256_print_hex("tb  = ", &tb);
            goto end;
        }

        /* check r = tr1 = tr2 */
        if (fp256_cmp(&r, &tr1) != 0 || fp256_cmp(&r, &tr2) != 0) {
            printf("fp256_mont_mul %" PRId64 " failed\n", i + 1);
            test_fp256_print_mont_ctx("mont ctx:\n", &mctx);
            test_fp256_print_hex("a   = ", &a);
            test_fp256_print_hex("A   = ", &A);
            test_fp256_print_hex("b   = ", &b);
            test_fp256_print_hex("B   = ", &B);
            test_fp256_print_hex("r   = ", &r);
            test_fp256_print_hex("tr1 = ", &tr1);
            test_fp256_print_hex("tr2 = ", &tr2);
            goto end;
        }
    }

    printf("fp256_mont_mul test passed \n");
    td->ok = FP256_OK;
end:
    return NULL;
}

int main(int argc, char **argv)
{
    TEST_ARGS args;

    if (fp256_init() != FP256_OK)
        return -1;

    get_test_args(argc, argv, &args);
    test_rand_init();

    RETURN_IF_ERROR(run_test("fp256_mont_mul", NULL, fp256_mont_mul_test, args.N, args.T));

    fp256_deinit();
    return 0;
}

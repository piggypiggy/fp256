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

void* fp256_mont_mul_test(void *p)
{
    int64_t i, N;
    fp256 r, odd, mod, k0;
    size_t ol, w;
    mont_ctx mctx;
    TEST_THREAD_DATA *td;

    td = (TEST_THREAD_DATA*)p;
    N = td->N;

    /* mod = 2^64 */
    fp256_clear_set_bit(&mod, 64);
    for (i = 0; i < N; i++) {
        /* random odd modulo and corresponding R */
        do {
            ol = rand();
            ol %= 5;
            fp256_rand_limbs(&odd, ol, 0);
            w = ol;
        } while (fp256_is_odd(&odd) != 1);
        fp256_mont_ctx_init(&mctx, w, &odd);

        /* check k0 * N = -1 mod 2^64 */
        fp256_set_limb(&k0, mctx.k0);
        /* k0 * N mod 2^64 */
        fp256_mod_mul(&r, &k0, &mctx.N, &mod);
        fp256_mod_neg(&r, &r, &mod);
        if (fp256_is_one(&r) != 1) {
            printf("fp256_mont_mul check k0 %" PRId64 " failed\n", i + 1);
            test_fp256_print_hex("k0 = ", &k0);
            test_fp256_print_hex("N  = ", &mctx.N);
            test_fp256_print_hex("r  = ", &r);
            goto end;
        }

        /* TODO : mont mul/sqr test */

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
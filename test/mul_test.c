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

typedef struct {
    char *r;
    char *a;
    char *b;
} FP256_MUL_TEST_VECTOR;



void fp256_mul_test_vector(void)
{

}

void* fp256_mul_test(void *p)
{
    int64_t i, N;
    fp256 r1, r2, a, b;
    TEST_THREAD_DATA *td;

    td = (TEST_THREAD_DATA*)p;
    N = td->N;

    for (i = 0; i < N; i++) {
        fp256_rand_limbs(&a, 4, 0);
        fp256_rand_limbs(&b, 4, 0);
        fp256_mul(&r1, &a, &b);
        fp256_mul(&r2, &b, &a);
        if (fp256_cmp(&r1, &r2) != 0) {
            printf("fp256_mul a*b != b*a %" PRId64 "\n", i+1);
            goto end;
        }
    }

    printf("fp256_mul test passed \n");
    td->ok = FP256_OK;
end:
    return NULL;
}

void* fp256_sqr_test(void *p)
{
    int64_t i, N;
    fp256 r1, r2, a;
    TEST_THREAD_DATA *td;

    td = (TEST_THREAD_DATA*)p;
    N = td->N;

    for (i = 0; i < N; i++) {
        fp256_rand_limbs(&a, 0, 0);
        fp256_mul(&r1, &a, &a);
        fp256_sqr(&r2, &a);
        if (fp256_cmp(&r1, &r2) != 0) {
            test_fp256_print_hex("a   = ", &a);
            test_fp256_print_hex("a^2 = ", &r2);
            test_fp256_print_hex("a*a = ", &r1);
            printf("fp256_sqr a^2 != a*a %" PRId64 "\n", i+1);
            goto end;
        }
    }

    printf("fp256_sqr test passed \n");
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

    RETURN_IF_ERROR(run_test("fp256_mul", NULL, fp256_mul_test, args.N, args.T));
    RETURN_IF_ERROR(run_test("fp256_sqr", NULL, fp256_sqr_test, args.N, args.T));

    fp256_deinit();
    return 0;
}
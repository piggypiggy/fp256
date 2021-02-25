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

typedef struct {
    char *r;
    char *a;
    char *b;
} FP256_MUL_TEST_VECTOR;



void fp256_mullo_test_vector(void)
{

}

void* fp256_mullo_test(void *p)
{
    int64_t i, N;
    fp256 r1, r2, a, b;
    TEST_THREAD_DATA *td;

    td = (TEST_THREAD_DATA*)p;
    N = td->N;

    for (i = 0; i < N; i++) {
        fp256_rand_limbs(&a, 4);
        fp256_rand_limbs(&b, 4);
        fp256_mullo(&r1, &a, &b);
        fp256_mullo(&r2, &b, &a);
        if (fp256_cmp(&r1, &r2) != 0) {
            printf("fp256_mullo a*b != b*a %" PRId64 "\n", i+1);
            goto end;
        }
    }

    printf("fp256_mullo test passed \n");
    td->ok = FP256_OK;
end:
    return NULL;
}

void* fp256_sqrlo_test(void *p)
{
    int64_t i, N;
    fp256 r1, r2, a;
    TEST_THREAD_DATA *td;

    td = (TEST_THREAD_DATA*)p;
    N = td->N;

    for (i = 0; i < N; i++) {
        fp256_rand_limbs(&a, 4);
        fp256_mullo(&r1, &a, &a);
        fp256_sqrlo(&r2, &a);
        if (fp256_cmp(&r1, &r2) != 0) {
            test_fp256_print_hex("a   = ", &a);
            test_fp256_print_hex("a^2 = ", &r2);
            test_fp256_print_hex("a*a = ", &r1);
            printf("fp256_sqrlo a^2 != a*a %" PRId64 "\n", i+1);
            goto end;
        }
    }

    printf("fp256_sqrlo test passed \n");
    td->ok = FP256_OK;
end:
    return NULL;
}

int main(int argc, char **argv)
{
    TEST_ARGS args;

    if (fp256_init() != FP256_OK)
        return -1;

    RETURN_IF_ERROR(get_test_args(argc, argv, &args));
    test_rand_init();

    RETURN_IF_ERROR(run_test("fp256_mullo", NULL, fp256_mullo_test, args.N, args.T));
    RETURN_IF_ERROR(run_test("fp256_sqrlo", NULL, fp256_sqrlo_test, args.N, args.T));

    fp256_deinit();
    return 0;
}

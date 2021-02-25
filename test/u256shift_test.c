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

/* r = a << n
 * when n is negative, r = a >> (-n)
 */
typedef struct {
    char *r;
    char *a;
    int n;
} U256_SHIFT_TEST_VECTOR;

static U256_SHIFT_TEST_VECTOR u256_shift_test_vector[] = {
    /* 1 */
    {
        "1fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe",
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
        1,
    },
    /* 2 */
    {
        "7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
        -1,
    },
    /* 3 */
    {
        "86a4be7013cb9ce8f0d6e64089ee6ace01973ca63492e8400000000000000",
        "43525f3809e5ce74786b732044f7356700cb9e531a49742",
        57,
    },
    /* 4 */
    {
        "49c70d1b6a3d36520000000000",
        "49c70d1b6a3d3652",
        40,
    },
    /* 5 */
    {
        "9ffd78ed2f724649d42f9c400e85a28905d018ef982c6d74a2dd846dd4248bc0000",
        "4ffebc7697b92324ea17ce200742d14482e80c77cc1636ba516ec236ea1245e",
        17,
    },
    /* 6 */
    {
        "79576dd5e2855d596ed3cf978259404539d43d9b06610cbb",
        "3cabb6eaf142aeacb769e7cbc12ca0229cea1ecd8330865da6",
        -7,
    },
    /* 7 */
    {
        "784f2e8166c4e",
        "784f2e8166c4ed6e31e55",
        -32,
    },
    /* 8 */
    {
        "1b47e46a240c3af73f6fbcd5917e85f4190b42ead97af16ed61212d0882a00000000000000",
        "1b47e46a240c3af73f6fbcd5917e85f4190b42ead97af16ed61212d0882a",
        56,
    },
    /* 9 */
    {
        "3d1",
        "7a2966c056d9",
        -37,
    },
    /* 10 */
    {
        "2f821b996e564f2aeb738f2800d5e796b3e7ae4ffad3819bea32a",
        "17c10dccb72b279575b9c794006af3cb59f3d727fd69c0cdf5195",
        1,
    },
    /* 11 */
    {
        "0",
        "0",
        -34,
    },
    /* 12 */
    {
        "c51505287352e3adbbe7f43086dbe35e8dcae648923985",
        "3145414a1cd4b8eb6ef9fd0c21b6f8d7a372b992248e615c906425635d",
        -46,
    },
    /* 13 */
    {
        "0",
        "adbc6d6",
        -30,
    },
    /* 14 */
    {
        "164000000",
        "59",
        26,
    },
    /* 15 */
    {
        "e5dbb7d92",
        "72eddbec90c3b212",
        -27,
    },
    /* 16 */
    {
        "1bba94879f6e74d53",
        "3775290f3edce9aa7075b92b0b6083d",
        -57,
    },
    /* 17 */
    {
        "301f07747f94d79d4a1f8d37e250519e3c16",
        "301f07747f94d79d4a1f8d37e250519e3c16f",
        -4,
    },
    /* 18 */
    {
        "1efa6b680eefc6ba81efa7",
        "7be9ada03bbf1aea07be9cddb70c7303c",
        -46,
    },
    /* 19 */
    {
        "a71c13369",
        "538e099b4e6cc2801e342d5b",
        -59,
    },
    /* 20 */
    {
        "3a53c844623e7498ca44fdefce89807b8a6e07921add42baa1100ddb8fc052600",
        "3a53c844623e7498ca44fdefce89807b8a6e07921add42baa1100ddb8fc0526",
        8,
    },
};

int ll_u256_shift_test_vector(void)
{
    unsigned int i;
    int n;
    u64 tr[5], r[5], a[4];
    size_t rl, al, trl;

    for (i = 0; i < sizeof(u256_shift_test_vector) / sizeof(U256_SHIFT_TEST_VECTOR); i++) {
        /* clear tr, r, a first */
        ll_set_zero(r, 5);
        ll_set_zero(a, 4);

        ll_from_hex(r, &rl, (u8*)u256_shift_test_vector[i].r, strlen(u256_shift_test_vector[i].r));
        ll_from_hex(a, &al, (u8*)u256_shift_test_vector[i].a, strlen(u256_shift_test_vector[i].a));

        /* r = a << n */
        n = u256_shift_test_vector[i].n;
        if (n >= 0) {
            ll_u256_lshift(tr, a, (size_t)n);
            trl = 5;
        }
        else {
            ll_u256_rshift(tr, a, (size_t)-n);
            trl = 4;
        }

        if (ll_cmp_limbs(tr, r, trl, trl) != 0) {
            printf("ll_u256_shift_test_vector %d failed\n", i + 1);
            test_print_hex("r = ", tr, trl);
            test_print_hex("a = ", a, 4);
            printf("n = %d\n", n);
            printf("a << n should be :\n");
            test_print_hex("r = ", r, trl);
            return FP256_ERR;
        }
    }

    printf("ll_u256_shift_test_vector passed\n");
    return FP256_OK;
}

int main(int argc, char **argv)
{
    TEST_ARGS args;

    if (fp256_init() != FP256_OK)
        return -1;

    RETURN_IF_ERROR(get_test_args(argc, argv, &args));
    test_rand_init();

    RETURN_IF_ERROR(run_test("ll_u256_shift", ll_u256_shift_test_vector, NULL, args.N, args.T));

    fp256_deinit();
    return 0;
}

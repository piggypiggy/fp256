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
} FP256_SHIFT_TEST_VECTOR;

static FP256_SHIFT_TEST_VECTOR shift_test_vector[] = {
    /* 1 */
    {
        "9963a3455f909990eede9abd2a199ee5d624518b2e16c00ca9f6ef2",
        "2658e8d157e426643bb7a6af4a8667b975891462cb85b0032a7dbbca",
        -2,
    },
    /* 2 */
    {
        "7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
        -1,
    },
    /* 3 */
    {
        "2a3eb8f451ec286830000000000000000000000000000000000000000",
        "547d71e8a3d850d06",
        159,
    },
    /* 4 */
    {
        "0",
        "0",
        0,
    },
    /* 5 */
    {
        "a11",
        "2844de3d9f0d46442720",
        -66,
    },
    /* 6 */
    {
        "d3290c34b41597696b4fbd01e1e",
        "6994861a5a0acbb4b5a7de80f0f49",
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
        "3f98ce7f8117b2673f8ff1a627f91fd000000000000000000000000000",
        "3f98ce7f8117b2673f8ff1a627f91fd",
        108,
    },
    /* 9 */
    {
        "51dc0b2e8f000000000000000000000000000000000",
        "51dc0b2e8f",
        132,
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
        "0x706044a310ccf075a57925c797f2d1aad",
        -293,
    },
    /* 12 */
    {
        "227d02596385e395b443d54d4d508d5a5",
        "44fa04b2c70bc72b6887aa9a9aa11ab4a494ec97ac37d5a8e6f7b589",
        -93,
    },
    /* 13 */
    {
        "0",
        "adbc6d6",
        -30,
    },
    /* 14 */
    {
        "956a84fc0000000000000000000000000000000000000",
        "4ab5427e",
        149,
    },
    /* 15 */
    {
        "1b249978893b590fabbc0000000000000000000000000000000000000000",
        "6c9265e224ed643eaef",
        162,
    },
    /* 16 */
    {
        "8ab76d44a2984482561f348ce494db73cab88af1055a0bfe345de000",
        "455bb6a2514c22412b0f9a46724a6db9e55c457882ad05ff1a2ef",
        13,
    },
    /* 17 */
    {
        "0",
        "0",
        72,
    },
    /* 18 */
    {
        "3e631b2da3478a510c1585",
        "7cc6365b468f14a2182b0a51cb7b99866656134e4d35cff58",
        -109,
    },
    /* 19 */
    {
        "a71c13369",
        "538e099b4e6cc2801e342d5b",
        -59,
    },
    /* 20 */
    {
        "886176",
        "22185da4f34ace10684f41e13ce0e89f855a",
        -118,
    },
    /* 21 */
    {
        "31c70000000000000000",
        "31c7",
        64,
    },
    /* 22 */
    {
        "21080f4676bcefafcebf2dee3208c7d16fbe15ebc00000000000000",
        "84203d19daf3bebf3afcb7b8c8231f45bef857af",
        58,
    },
    /* 23 */
    {
        "15237fb540364af09404b4c003f49406326c46932c2bea",
        "548dfed500d92bc25012d3000fd25018c9b11a4cb0afaa37f0d530413ad96",
        -62,
    },
    /* 24 */
    {
        "123357f1cd163dfa9fa19b8400000000000000000000000",
        "48cd5fc73458f7ea7e866e1",
        94,
    },
    /* 25 */
    {
        "88888888888888888888888888888888888888888888888888888888888",
        "88888888888888888888888888888888888888888888888888888888888",
        0,
    },
};

int fp256_shift_test_vector(void)
{
    unsigned int i;
    int n;
    fp256 tr, tr1, r, a;

    for (i = 0; i < sizeof(shift_test_vector) / sizeof(FP256_SHIFT_TEST_VECTOR); i++) {
        fp256_from_hex(&r, (u8*)shift_test_vector[i].r, strlen(shift_test_vector[i].r));
        fp256_from_hex(&a, (u8*)shift_test_vector[i].a, strlen(shift_test_vector[i].a));

        /* tr = a << n */
        n = shift_test_vector[i].n;
        if (n > 0)
            fp256_lshift(&tr, &a, (size_t)n);
        else if (n < 0)
            fp256_rshift(&tr, &a, (size_t)-n);
        else { /* n = 0 */
            fp256_lshift(&tr, &a, 0);
            fp256_rshift(&tr1, &a, 0);

            /* lshift(a, 0) ?= rshift(a, 0) */
            if (fp256_cmp(&tr, &tr1) != 0) {
                printf("fp256_shift_test_vector %d failed\n", i + 1);
                printf("lshift(a, 0) != rshift(a, 0)\n");
                test_fp256_print_hex("a << 0 = ", &tr);
                test_fp256_print_hex("a >> 0 = ", &tr1);
                test_fp256_print_hex("a      = ", &a);
                printf("n = %d\n", n);
                printf("a << 0, a >> 0 should be :\n");
                test_fp256_print_hex("r      = ", &r);
                return FP256_ERR;
            }
        }

        if (fp256_cmp(&tr, &r) != 0) {
            printf("fp256_shift_test_vector %d failed\n", i + 1);
            test_fp256_print_hex("r = ", &tr);
            test_fp256_print_hex("a = ", &a);
            printf("n = %d\n", n);
            printf("a << n should be :\n");
            test_fp256_print_hex("r = ", &r);
            return FP256_ERR;
        }
    }

    printf("fp256_shift_test_vector passed\n");
    return FP256_OK;
}

int main(int argc, char **argv)
{
    TEST_ARGS args;

    if (fp256_init() != FP256_OK)
        return -1;

    RETURN_IF_ERROR(get_test_args(argc, argv, &args));
    test_rand_init();

    RETURN_IF_ERROR(run_test("fp256_shift", fp256_shift_test_vector, NULL, args.N, args.T));

    fp256_deinit();
    return 0;
}

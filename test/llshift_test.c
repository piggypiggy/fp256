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
} SHIFT_TEST_VECTOR;

static SHIFT_TEST_VECTOR shift_test_vector[] = {
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
        "2c9e32ed432f8b3f96a24ae0fb0d9a80635299c6cc72541e22bd0b12bcc4d2c4080000000000000000000",
        "164f1976a197c59fcb5125707d86cd4031a94ce366392a0f115e85895e62696204",
        77,
    },
    /* 4 */
    {
        "0",
        "0",
        0,
    },
    /* 5 */
    {
        "eeed8ca74cdb87d72ce15b559c3250f20e5060850dd6062245b4ed19eb94beb25a5edb4b1dd7a5394b8",
        "eeed8ca74cdb87d72ce15b559c3250f20e5060850dd6062245b4ed19eb94beb25a5edb4b1dd7a5394b8e307ced466787aad4",
        -68,
    },
    /* 6 */
    {
        "2f485d6f161402c52a085829cd769ad2f5016a237c98d2a7631e781d817b2e15d22b1ab",
        "bd2175bc58500b14a82160a735da6b4bd405a88df2634a9d8c79e07605ecb85748ac6ae7f44b01644aa7a354b67f954f44764a59d9ff0cf5e49",
        -178,
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
        "34fa168fed5229fa5869b977f517521597a23b80682f43b1b38cd2a680dbb444469237d80a6e18a07c660789e",
        "69f42d1fdaa453f4b0d372efea2ea42b2f447700d05e87636719a54d01b768888d246fb014dc3140f8cc0f13d43",
        -9,
    },
    /* 13 */
    {
        "0",
        "adbc6d6",
        -30,
    },
    /* 14 */
    {
        "ef50739a25c4811096213c34a00000000000000000000000000000000000000000000000000000000000000000000000000",
        "1dea0e7344b8902212c4278694",
        295,
    },
    /* 15 */
    {
        "117563319c38d92d45c5b6f1f3adfb9deccff76502f4f1f7d0b4443b98ae56dc186760f54bbdf3c72a33103b3bdc600000000000000000000000",
        "8bab198ce1c6c96a2e2db78f9d6fdcef667fbb2817a78fbe85a221dcc572b6e0c33b07aa5def9e39519881d9dee3",
        93,
    },
    /* 16 */
    {
        "59b45c9e13a53d24ab582a38c3478363dda2b67114d9674e342c5aba107f6c44fb",
        "b368b93c274a7a4956b05471868f06c7bb456ce229b2ce9c6858b57420fed889f6a164618c414f59840cd4a8df",
        -97,
    },
    /* 17 */
    {
        "0",
        "0",
        72,
    },
    /* 18 */
    {
        "5796ce52d326b31a1bc8c3c96716b036c3d20fabb1b7a455fb2f91e3fca6ff8e6df3a848e31dd31400000000000000000000000000000000000000000000000000",
        "15e5b394b4c9acc686f230f259c5ac0db0f483eaec6de9157ecbe478ff29bfe39b7cea1238c774c5",
        202,
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
        "ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc",
        "ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc",
        0,
    },
    /* 22 */
    {
        "10132d77ff7954bde15a9d0ea1ba4de53cd2cdf83546713db40000000000000000",
        "404cb5dffde552f7856a743a86e93794f34b37e0d519c4f6d",
        66,
    },
    /* 23 */
    {
        "15237fb540364af09404b4c003f49406326c46932c2bea",
        "548dfed500d92bc25012d3000fd25018c9b11a4cb0afaa37f0d530413ad96",
        -62,
    },
    /* 24 */
    {
        "8ce14522e7b078f22afba9878e01f2bb23ccef0754cbf0d6f39ce689",
        "23385148b9ec1e3c8abeea61e3807caec8f33bc1d532fc35bce739a252033045b32ee18bdbf505ff8e5aac7b10",
        -134,
    },
    /* 25 */
    {
        "",
        "0",
        -64,
    },
    /* 26 */
    {
        "8888888888888888",
        "8888888888888888",
        0,
    },
};

int ll_shift_test_vector(void)
{
    unsigned int i;
    int n;
    /* result has at most 12 limbs */
    u64 tr[12], tr1[12], r[12], a[12];
    size_t trl, trl1, rl, al, max;

    for (i = 0; i < sizeof(shift_test_vector) / sizeof(SHIFT_TEST_VECTOR); i++) {
        /* clear */
        ll_set_zero(tr, 12);
        ll_set_zero(tr1, 12);

        ll_from_hex(r, &rl, (u8*)shift_test_vector[i].r, strlen(shift_test_vector[i].r));
        ll_from_hex(a, &al, (u8*)shift_test_vector[i].a, strlen(shift_test_vector[i].a));

        /* tr = a << n */
        n = shift_test_vector[i].n;
        if (n > 0) {
            ll_lshift(tr, a, al, (size_t)n);
            trl = (al == 0 ? 0 : ll_num_limbs(tr, 12));
        }
        else if (n < 0) {
            ll_rshift(tr, a, al, (size_t)-n);
            trl = (al == 0 ? 0 : ll_num_limbs(tr, 12));
        }
        else { /* n = 0 */
            ll_lshift(tr, a, al, 0);
            trl = (al == 0 ? 0 : ll_num_limbs(tr, 12));

            ll_rshift(tr1, a, al, 0);
            trl1 = (al == 0 ? 0 : ll_num_limbs(tr1, 12));

            /* lshift(a, 0) ?= rshift(a, 0) */
            if (trl != trl1 || (ll_cmp_limbs(tr, tr1, trl, trl1) != 0)) {
                printf("ll_shift_test_vector %d failed\n", i + 1);
                printf("lshift(a, 0) != rshift(a, 0)\n");
                printf("al = %zu, rl = %zu, trl = %zu, trl1 = %zu\n", al, rl, trl, trl1);
                max = (trl > rl ? trl : rl);
                max = (max > trl1 ? max : trl1);
                max = (max > al ? max : al);
                test_print_hex("a << 0 = ", tr, max);
                test_print_hex("a >> 0 = ", tr1, max);
                test_print_hex("a      = ", a, al);
                printf("a << 0, a >> 0 should be :\n");
                test_print_hex("r      = ", r, max);
                return FP256_ERR;
            }
        }

        if (ll_cmp_limbs(tr, r, trl, rl) != 0) {
            printf("ll_shift_test_vector %d failed\n", i + 1);
            printf("al = %zu, rl = %zu, trl = %zu\n", al, rl, trl);
            max = (trl > rl ? trl : rl);
            max = (max > al ? max : al);
            test_print_hex("r = ", tr, max);
            test_print_hex("a = ", a, al);
            printf("n = %d\n", n);
            printf("a << n should be :\n");
            test_print_hex("r = ", r, max);
            return FP256_ERR;
        }
    }

    printf("ll_shift_test_vector passed\n");
    return FP256_OK;
}

int main(int argc, char **argv)
{
    TEST_ARGS args;

    if (fp256_init() != FP256_OK)
        return -1;

    RETURN_IF_ERROR(get_test_args(argc, argv, &args));
    test_rand_init();

    RETURN_IF_ERROR(run_test("ll_shift", ll_shift_test_vector, NULL, args.N, args.T));

    fp256_deinit();
    return 0;
}

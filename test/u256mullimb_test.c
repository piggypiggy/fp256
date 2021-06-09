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

/* r = a * b */
typedef struct {
    char *r;
    char *a;
    u64 b;
} U256_MUL_LIMB_TEST_VECTOR;

static U256_MUL_LIMB_TEST_VECTOR u256_mul_limb_test_vector[] = {
    /* 1 */
    {
        "8",
        "4",
        2,
    },
    /* 2 */
    {
        "fffffffffffffffeffffffffffffffffffffffffffffffffffffffffffffffff0000000000000001",
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
        0xffffffffffffffff,
    },
    /* 3 */
    {
        "ffffffffffff88880000000000007776ff0000000100000000fffffffef0000000000000001",
        "ffffffffffff8888ffffffffffffffffff00000000fffffffffffffffff",
        0xffffffffffffffff,
    },
    /* 4 */
    {
        "1311c71c4bbe42ecbe36b6a7c3d16db1a2f03b561e11aec05bc4da0bde4153646771a",
        "e8f3625607e05695b174bae2ab624de9dded0a90aeed321667cb6e71f06c9eb",
        0x14f4ce,
    },
    /* 5 */
    {
        "1afce2",
        "3e3e",
        0x6f,
    },
    /* 6 */
    {
        "55ca27965b8aa5316c0c89c0db3",
        "23a64842c84f",
        0x2680ea268046e75d,
    },
    /* 7 */
    {
        "12345677fffffffffffffffffffffffffffffffffffffffffffffffffeb49923e20b27c0",
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffedcba988",
        0x12345678,
    },
    /* 8 */
    {
        "2a07d18e9475fcd3721724c47a19a6ec43a373921c0ead13788361d444164f5800ebb3a",
        "5cf483087255d5c8ebfcb246bd1bae3699b62f16f8a41560d1407e03f72363",
        0x73c0b95fe,
    },
    /* 9 */
    {
        "571a2ec813f23da9ad184d1ca6908f",
        "1c1b0e833711430b",
        0x3195e756c9c1b0d,
    },
    /* 10 */
    {
        "123cfc917dd45835d56722ee84356d798d41812248db274a9898bd",
        "dbce64c2d3420cc1401212fa59f16f25c48aeb9",
        0x153dcacb6661825,
    },
    /* 11 */
    {
        "0",
        "dbce64c2d3420cc1401212fa59f16f25c48aeb9",
        0,
    },
    /* 12 */
    {
        "",
        "0",
        0,
    },
};

int ll_u256_mul_limb_test_vector(void)
{
    unsigned int i;
    u64 tr[5], r[5], a[4], b;
    size_t trl, rl, al;

    for (i = 0; i < sizeof(u256_mul_limb_test_vector) / sizeof(U256_MUL_LIMB_TEST_VECTOR); i++) {
        /* clear a first */
        ll_set_zero(a, 4);

        ll_from_hex(r, &rl, (u8*)u256_mul_limb_test_vector[i].r, strlen(u256_mul_limb_test_vector[i].r));
        ll_from_hex(a, &al, (u8*)u256_mul_limb_test_vector[i].a, strlen(u256_mul_limb_test_vector[i].a));
        b = u256_mul_limb_test_vector[i].b;

        /* r = a + b */
        tr[4] = ll_u256_mul_limb(tr, a, b);
        trl = ll_num_limbs(tr, 5);
        if (ll_cmp_limbs(tr, r, trl, rl) != 0) {
            printf("ll_u256_mul_limb_test_vector %d failed\n", i + 1);
            test_print_hex("r = ", tr, 5);
            test_print_hex("a = ", a, 4);
            test_print_hex("b = ", &b, 1);
            printf("a * b should be :\n");
            test_print_hex("r = ", r, 5);
            return FP256_ERR;
        }
    }

    printf("ll_u256_mul_limb_test_vector passed\n");
    return FP256_OK;
}

int main(int argc, char **argv)
{
    TEST_ARGS args;

    if (fp256_init() != FP256_OK)
        return -1;

    RETURN_IF_ERROR(get_test_args(argc, argv, &args));
    test_rand_init();

    RETURN_IF_ERROR(run_test("ll_u256_mul_limb", ll_u256_mul_limb_test_vector, NULL, args.N, args.T));

    fp256_deinit();
    return 0;
}

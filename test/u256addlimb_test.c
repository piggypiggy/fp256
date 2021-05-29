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

/* r = a + b */
typedef struct {
    char *r;
    char *a;
    u64 b;
} U256_ADD_LIMB_TEST_VECTOR;

static U256_ADD_LIMB_TEST_VECTOR u256_add_limb_test_vector[] = {
    /* 1 */
    {
        "3",
        "1",
        2,
    },
    /* 2 */
    {
        "1000000000000000000000000000000000000000000000000fffffffffffffffe",
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
        0xffffffffffffffff,
    },
    /* 3 */
    {
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
        "ffffffffffffffffffffffffffffffffffffffffffffffff0000000000000000",
        0xffffffffffffffff,
    },
    /* 4 */
    {
        "3c1d80648fa3f49af684884a316d",
        "3c1d80648fa30b04ad78203be8c9",
        0xe996490c680e48a4,
    },
    /* 5 */
    {
        "7790020cd54d4e29",
        "caf",
        0x7790020cd54d417a,
    },
    /* 6 */
    {
        "140f63",
        "132ba4",
        0xe3bf,
    },
    /* 7 */
    {
        "10000000000000000000000000000000000000000000000000000000000000000",
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffedcba988",
        0x12345678,
    },
    /* 8 */
    {
        "df871474",
        "df87146d",
        7,
    },
    /* 9 */
    {
        "70000000000000007",
        "66666666666666666",
        0x99999999999999a1,
    },
    /* 10 */
    {
        "1348ae1895f44d14dc638dbcc",
        "1348ae1895f44d067a7048844",
        0xe61f345388,
    },
};

int ll_u256_add_limb_test_vector(void)
{
    unsigned int i;
    u64 tr[5], r[5], a[4], b;
    size_t trl, rl, al;

    for (i = 0; i < sizeof(u256_add_limb_test_vector) / sizeof(U256_ADD_LIMB_TEST_VECTOR); i++) {
        /* clear a first */
        ll_set_zero(a, 4);

        ll_from_hex(r, &rl, (u8*)u256_add_limb_test_vector[i].r, strlen(u256_add_limb_test_vector[i].r));
        ll_from_hex(a, &al, (u8*)u256_add_limb_test_vector[i].a, strlen(u256_add_limb_test_vector[i].a));
        b = u256_add_limb_test_vector[i].b;

        /* r = a + b */
        tr[4] = ll_u256_add_limb(tr, a, b);
        trl = ll_num_limbs(tr, 5);
        if (ll_cmp_limbs(tr, r, trl, rl) != 0) {
            printf("ll_u256_add_limb_test_vector %d failed\n", i + 1);
            test_print_hex("r = ", tr, 5);
            test_print_hex("a = ", a, 4);
            test_print_hex("b = ", &b, 1);
            printf("a + b should be :\n");
            test_print_hex("r = ", r, 5);
            return FP256_ERR;
        }
    }

    printf("ll_u256_add_limb_test_vector passed\n");
    return FP256_OK;
}

int fp256_add_limb_test_vector(void)
{
    unsigned int i;
    fp256 tr, r, a;
    u64 b;

    for (i = 0; i < sizeof(u256_add_limb_test_vector) / sizeof(U256_ADD_LIMB_TEST_VECTOR); i++) {
        /* ignore r >= 2^256, a >= 2^256 cases */
        if (fp256_from_hex(&r, (u8*)u256_add_limb_test_vector[i].r, strlen(u256_add_limb_test_vector[i].r)) != FP256_OK)
            continue;
        if (fp256_from_hex(&a, (u8*)u256_add_limb_test_vector[i].a, strlen(u256_add_limb_test_vector[i].a)) != FP256_OK)
            continue;
        b = u256_add_limb_test_vector[i].b;

        /* r = a + b */
        fp256_add_limb(&tr, &a, b);
        if (fp256_cmp(&tr, &r) != 0) {
            printf("fp256_add_limb_test_vector %d failed\n", i + 1);
            test_fp256_print_hex("r = ", &tr);
            test_fp256_print_hex("a = ", &a);
            test_print_hex("b = ", &b, 1);
            printf("a + b should be :\n");
            test_fp256_print_hex("r = ", &r);
            return FP256_ERR;
        }
    }

    printf("fp256_add_limb_test_vector passed\n");
    return FP256_OK;
}

int main(int argc, char **argv)
{
    TEST_ARGS args;

    if (fp256_init() != FP256_OK)
        return -1;

    RETURN_IF_ERROR(get_test_args(argc, argv, &args));
    test_rand_init();

    RETURN_IF_ERROR(run_test("ll_u256_add_limb", ll_u256_add_limb_test_vector, NULL, args.N, args.T));
    RETURN_IF_ERROR(run_test("fp256_add_limb", fp256_add_limb_test_vector, NULL, args.N, args.T));

    fp256_deinit();
    return 0;
}

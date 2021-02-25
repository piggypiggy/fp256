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

/* r = a - b */
typedef struct {
    char *r;
    char *a;
    char *b;
} U256_SUB_TEST_VECTOR;

static U256_SUB_TEST_VECTOR u256_sub_test_vector[] = {
    /* 1 */
    {
        "1",
        "3",
        "2",
    },
    /* 2 */
    {
        "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
        "1ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe",
        "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
    },
    /* 3 */
    {
        "5555555555555555555555555555555555555555555555555555555555555554",
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
        "5555555555555555555555555555555555555555555555555555555555555556",
    },
    /* 4 */
    {
        "11245e1f5dc3c612289f0f7f970bf09300a716f9a86ff",
        "1efe6ecdcf47dd34d3dc1ce3d8684196c6d128e3205a1",
        "dda10ae71841722ab3d0d64415c5103c62a11e977ea2",
    },
    /* 5 */
    {
        "13bc48121e80e6b4e8844c209903b823c5bdfc2f898df3f4de6b63",
        "140fc7a87c80db0955ea8283565527ea874aa9d6d2ed0b9e1a62dc",
        "537f965dfff4546d663662bd516fc6c18cada7495f17a93bf779",
    },
    /* 6 */
    {
        "1792d533cd1c26a3bf355fdb88850ed4ed1d5f6001b38844a711e30",
        "1792d533cd1c26a3bf355fdb88850edab8eb6e215376d7fe989f86e",
        "5cbce0ec151c34fb9f18da3e",
    },
    /* 7 */
    {
        "c74ccc89b394db508f7c15f9c0125f8ccefb0533333d3346c68",
        "c74ccc89b394db508f7c15f9c0125f8ccefb0533333d3346c6d",
        "5",
    },
    /* 8 */
    {
        "19615e9a40d02588d57e80f693cf",
        "19615e9a40d02588d57e80f693cf",
        "0",
    },
    /* 9 */
    {
        "2b40b7a18fcee2fca775c41d17b5b4b8967f7cc141d90743e190290216a0ff8",
        "33f685dc1797e293b7f76fb33ea8d524685b9b24e6b3d60ce6dfea3c67c6094",
        "8b5ce3a87c8ff971081ab9626f3206bd1dc1e63a4dacec9054fc13a512509c",
    },
    /* 10 */
    {
        "aaab76240a742ec9e53129661786f2699105a198b787e04dbc5ceab69939d",
        "aaabb66b72641a40eb8d04b80c13dc58c4ce39d0f27c8107dfc1d3a4c0b66",
        "404767efeb77065bdb51f48ce9ef33c898383af4a0ba2364e8ee277c9",
    },
};

int ll_u256_sub_test_vector(void)
{
    unsigned int i;
    u64 tr[5], r[5], a[4], b[4];
    size_t trl, rl, al, bl;

    for (i = 0; i < sizeof(u256_sub_test_vector) / sizeof(U256_SUB_TEST_VECTOR); i++) {
        /* clear a, b first */
        ll_set_zero(a, 4);
        ll_set_zero(b, 4);

        ll_from_hex(r, &rl, (u8*)u256_sub_test_vector[i].r, strlen(u256_sub_test_vector[i].r));
        ll_from_hex(a, &al, (u8*)u256_sub_test_vector[i].a, strlen(u256_sub_test_vector[i].a));
        ll_from_hex(b, &bl, (u8*)u256_sub_test_vector[i].b, strlen(u256_sub_test_vector[i].b));

        /* r = a - b */
        ll_u256_sub(tr, a, b);
        trl = ll_num_limbs(tr, 4);
        if (ll_cmp_limbs(tr, r, trl, rl) != 0) {
            printf("ll_u256_sub_test_vector %d failed\n", i + 1);
            test_print_hex("r = ", tr, 5);
            test_print_hex("a = ", a, 4);
            test_print_hex("b = ", b, 4);
            printf("a - b should be :\n");
            test_print_hex("r = ", r, 5);
            return FP256_ERR;
        }
    }

    printf("ll_u256_sub_test_vector passed\n");
    return FP256_OK;
}

int main(int argc, char **argv)
{
    TEST_ARGS args;

    if (fp256_init() != FP256_OK)
        return -1;

    RETURN_IF_ERROR(get_test_args(argc, argv, &args));
    test_rand_init();

    RETURN_IF_ERROR(run_test("ll_u256_sub", ll_u256_sub_test_vector, NULL, args.N, args.T));

    fp256_deinit();
    return 0;
}

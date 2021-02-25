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
    char *b;
} U256_ADD_TEST_VECTOR;

static U256_ADD_TEST_VECTOR u256_add_test_vector[] = {
    /* 1 */
    {
        "3",
        "1",
        "2",
    },
    /* 2 */
    {
        "1fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffd",
        "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe",
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
    },
    /* 3 */
    {
        "10000000000000000000000000000000000000000000000000000000000000000",
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
        "5555555555555555555555555555555555555555555555555555555555555556",
    },
    /* 4 */
    {
        "801ee1eeb71db55bade1ada146a3f9d2e2bd0537bc40c0dbd54b99bfb8698f3",
        "801ee15bee0a4dbd691ffd3e5ffbcb6308b9edb5e5586440cd2c6f1051428fc",
        "92c913679e44c1b062e6a82e6fda031781d6e85c9b081f2aaf6726ff7",
    },
    /* 5 */
    {
        "1665a9f1f1774ed456491300095e2da833d50e68d5f04f1c95a970b8033510aa",
        "1ed71f3821941598ca84c46cda8710146da51a4a",
        "1665a9f1f1774ed4564912ffea870e701240f8d00b6b8aafbb2260a3958ff660",
    },
    /* 6 */
    {
        "7876fd96f3f8714964a87d4ec99e4162219c3fd134cd",
        "3869786c2b645fa3e4c67373a1d47b1f28faca8098ae",
        "400d852ac89411a57fe209db27c9c642f8a175509c1f",
    },
    /* 7 */
    {
        "cb83da4d72ca3a8a7e58b9ef13f1ef0230d2a145aa84af0bc0544b2cf",
        "8fbc2fd042a34e65091548216b5152f9728bb3b191577aff29bf64e",
        "caf41e1da287973c194fa4a6f2869daf37601591f8f35790c12a8bc81",
    },
    /* 8 */
    {
        "df871474",
        "df87146d",
        "7",
    },
    /* 9 */
    {
        "3aa6b727d8e72e2d9a63b2c82a2406bca29d2ded3c069fd4338e58dfb1fa4",
        "3aa6b685bb53a581d7d7031c8df2749ecf6351e1f2b7fff2bd6d6ab8bbfa0",
        "a21d9388abc28cafab9c31921dd339dc0b494e9fe17620ee26f6004",
    },
};

int ll_u256_add_test_vector(void)
{
    unsigned int i;
    u64 tr[5], r[5], a[4], b[4];
    size_t trl, rl, al, bl;

    for (i = 0; i < sizeof(u256_add_test_vector) / sizeof(U256_ADD_TEST_VECTOR); i++) {
        /* clear a, b first */
        ll_set_zero(a, 4);
        ll_set_zero(b, 4);

        ll_from_hex(r, &rl, (u8*)u256_add_test_vector[i].r, strlen(u256_add_test_vector[i].r));
        ll_from_hex(a, &al, (u8*)u256_add_test_vector[i].a, strlen(u256_add_test_vector[i].a));
        ll_from_hex(b, &bl, (u8*)u256_add_test_vector[i].b, strlen(u256_add_test_vector[i].b));

        /* r = a + b */
        tr[4] = ll_u256_add(tr, a, b);
        trl = ll_num_limbs(tr, 5);
        if (ll_cmp_limbs(tr, r, trl, rl) != 0) {
            printf("ll_u256_add_test_vector %d failed\n", i + 1);
            test_print_hex("r = ", tr, 5);
            test_print_hex("a = ", a, 4);
            test_print_hex("b = ", b, 4);
            printf("a + b should be :\n");
            test_print_hex("r = ", r, 5);
            return FP256_ERR;
        }
    }

    printf("ll_u256_add_test_vector passed\n");
    return FP256_OK;
}

int main(int argc, char **argv)
{
    TEST_ARGS args;

    if (fp256_init() != FP256_OK)
        return -1;

    RETURN_IF_ERROR(get_test_args(argc, argv, &args));
    test_rand_init();

    RETURN_IF_ERROR(run_test("ll_u256_add", ll_u256_add_test_vector, NULL, args.N, args.T));

    fp256_deinit();
    return 0;
}

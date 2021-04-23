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
} ADD_TEST_VECTOR;

static ADD_TEST_VECTOR add_test_vector[] = {
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
        "40a2439d286010e1573218873d00930c474ec82c61eb0e0118aede90d328b6203684109be99f3d916c01c8be1",
        "40a2439d286010e1573218873d00930c474ec82c61eb0e0118aede90d23f636ae1ccd56bf97c214c53fe6ec01",
        "e952b554b73b2ff0231c45180359fe0",
    },
    /* 5 */
    {
        "27c2852eb0c7788a783ec78b9a0a8fa591d31b9e80e3e33e390473cce643571b087e183d5a4144a6087f119c36d424a80686d8b7a3d2d18173f6de13d8941",
        "72b660462f714ec2634e6397ffe8ea255dad7ce9b217fe53dca52107b5e7754f5de21abfa43439e99843eaad008c86e95aa5c83f6139d27cbabea",
        "27c2852e3e11184448cd78c936bc2c0d91ea31792336665486ec7579099e36135296a2edfc5f29e6644ad7b29e9039fb05fa51ce492d094212bd0b971dd57",
    },
    /* 6 */
    {
        "26b606d62d680ff0bdccf53a1f176ce194b43d731b2bb8472d6f8fdb79717224bd282114fe0513f9844044711daf91",
        "26b606d62d680ff0bdccf539a4175ecbb35ecc14087f53e4bc8e7c0cf0a369398cfa565bfe1848f72ef3e1f5a7e723",
        "7b000e15e155715f12ac646270e113ce88ce08eb302dcab8ffeccb02554c627b75c86e",
    },
    /* 7 */
    {
        "cf07366265ac744b8bf3681ac5d327f816a61",
        "e85b",
        "cf07366265ac744b8bf3681ac5d327f808206",
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
    /* 10 */
    {
        "472c20dc787e45601bf447bb69df8c95aaa1108ec64b263a1c3c1285f96008e5fbaf2dfb800b0e71abd4a94cc1ccdd6e7d02bbc7cc92d03d0fb2697f20c4",
        "472c20dc787e45601bf3d68925fea143d7a5c3cec904b925cebb475c58b99782c13159dbc2f8ccfff917c863ff3165873ea2818b6313ab4b427a9f5ec51d",
        "713243e0eb51d2fb4cbffd466d144d80cb29a0a671633a7dd41fbd124171b2bce0e8c29b77e73e603a3c697f24f1cd37ca205ba7",
    },
    /* 11 */
    {
        "7f60f3bca303e6fe41f32f8b5255f466d0f20e8b167928d81b046aab4dbc4beaec05ee19a5aefb8b848f3d4b9a4027c3d98623d582d18df22de6fa1777ce3d",
        "7f60f3bca303e6fe41f32f8b5255f466d0f20e8b167928d81b046aab4dbc4beaec05ee19a5aefb8b848f3d4b9a4027c3d98623d582d18df22de6fa143db189",
        "33a1cb4",
    },
    /* 12 */
    {
        "225c495148cd17ad5723a8ba8daacaa948e6aaaa55fb1a9e38cce47ec2870250b6fd774f8e164c9a1af64d03a7d896bc70c823ccd875b5cb3f0b2aa",
        "22dc01f829c5ee69a1fd3fa1560ad2344946250de5805306ad0e2207bd886ae8a7b4cd51cbde92a4c6",
        "225c495148cd17ad5723a8ba8daacaa948e6a87c95db9801d9e64a5eee8cecf009da32bb2bc56e4215c5e232c5b81ae3ea1999518ba0990d55e0de4",
    },
    /* 13 */
    {
        "11111111111111111111111111111",
        "11111111111111111111111111111",
        "",
    },
    /* 14 */
    {
        "22222222222222222222222222222222222222222222222222",
        "",
        "22222222222222222222222222222222222222222222222222",
    }
};

int ll_add_test_vector(void)
{
    unsigned int i;
    u64 tr[9], r[9], a[8], b[8], ta[8];
    size_t trl, rl, al, bl, tal, max;

    for (i = 0; i < sizeof(add_test_vector) / sizeof(ADD_TEST_VECTOR); i++) {
        ll_from_hex(r, &rl, (u8*)add_test_vector[i].r, strlen(add_test_vector[i].r));
        ll_from_hex(a, &al, (u8*)add_test_vector[i].a, strlen(add_test_vector[i].a));
        ll_from_hex(b, &bl, (u8*)add_test_vector[i].b, strlen(add_test_vector[i].b));

        /* tr = a + b */
        ll_add(tr, a, b, al, bl);
        trl = ll_num_limbs(tr, (al > bl ? al : bl) + 1);
        if (ll_cmp_limbs(tr, r, trl, rl) != 0) {
            max = (trl > rl ? trl : rl);
            printf("ll_add_test_vector %d failed\n", i + 1);
            test_print_hex("r = ", tr, max);
            test_print_hex("a = ", a, max);
            test_print_hex("b = ", b, max);
            printf("a + b should be :\n");
            test_print_hex("r = ", r, max);
            return FP256_ERR;
        }

        /* ta = r - b */
        ll_sub(ta, r, b, rl, bl);
        tal = ll_num_limbs(ta, rl);
        if (ll_cmp_limbs(ta, a, tal, al) != 0) {
            max = rl;
            printf("ll_sub_test_vector %d failed\n", i + 1);
            test_print_hex("a = ", ta, max);
            test_print_hex("r = ", r, max);
            test_print_hex("b = ", b, max);
            printf("r - b should be :\n");
            test_print_hex("a = ", a, max);
            return FP256_ERR;
        }
    }

    printf("ll_add_test_vector passed\n");
    return FP256_OK;
}

int main(int argc, char **argv)
{
    TEST_ARGS args;

    if (fp256_init() != FP256_OK)
        return -1;

    RETURN_IF_ERROR(get_test_args(argc, argv, &args));
    test_rand_init();

    RETURN_IF_ERROR(run_test("ll_add", ll_add_test_vector, NULL, args.N, args.T));

    fp256_deinit();
    return 0;
}

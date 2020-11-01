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

/* r = a * b */
typedef struct {
    char *r;
    char *a;
    char *b;
} U256_MUL_TEST_VECTOR;

static U256_MUL_TEST_VECTOR u256_mul_test_vector[] = {
    /* 1 */
    {
        "1",
        "1",
        "1",
    },
    /* 2 */
    {
        "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffd0000000000000000000000000000000000000000000000000000000000000002",
        "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe",
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
    },
    /* 3 */
    {
        "38e38e38e38e38e38e38e38e38e38e38e38e38e38e38e38e38e38e38e38e38e3c71c71c71c71c71c71c71c71c71c71c71c71c71c71c71c71c71c71c71c71c71c",
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
        "5555555555555555555555555555555555555555555555555555555555555556",
    },
    /* 4 */
    {
        "1a5563c12e79ad4d53ab341465ad675775de75e4aca78d78ce26b073",
        "ac8cbd1240a3e871507b091eb",
        "2711ba59e4a6474779dd25d3734b199",
    },
    /* 5 */
    {
        "0",
        "0",
        "0",
    },
    /* 6 */
    {
        "15cfc1a4eb92c23395658002350",
        "5fc28f5d7cb0c",
        "3a4f55e049de9c",
    },
    /* 7 */
    {
        "d82b7b9a7715864b94e6532d1821cfdedbc8ab031128086236c37530cf07d3f9d57054fcfa36c9ba2a89e9f019df79",
        "104162d4f0c6b62ecf7c91dbc824031f8038b303de08ec7545d5cf1f55f1f",
        "d4c5f51a7d5a82d012968b94d4877e2667",
    },
    /* 8 */
    {
        "5ab476deeb83095e35a8814cbdbb1c604fdac2360b6a08053b0ff6f99492a44b876e53205432f2dabc",
        "86ee625b1dc6a27db14d9bd8d8b8ca5964",
        "ac17561474f9aa5cae9cd5f36d04144fbaeb66fdf304a447",
    },
    /* 9 */
    {
        "689b40415f3fddc41bbc52c77b8277c76fed6b5dea2afec",
        "5a",
        "1298c177b5e9387de2cc2578cc007050f77429d7ce9b8e",
    },
    /* 10 */
    {
        "1787fc886cb905ca2bdc204f6e5ab61781b249ab7292ccf926e4dc3c053e3cab11625297191e12f5cedd9b16436720281eae3ca2f9",
        "3de7fc9a8759f06899c42fafd967c251fc9106f5b9cd89857eecde65",
        "614eec467c4581c48e38fc6e144009f34dc2d3862c424cef05",
    },
    /* 11 */
    {
        "0",
        "0",
        "0xe54ff64e6fb4513471fb",
    },
    /* 12 */
    {
        "59c09284a11712df410325afdfb53b58662e615427472eb10c6f135a651149e85c937c237b4da15450538",
        "2c8ed215de1c71e2dcdab52b27c07d151a74b0253f0d566407343b8964341248",
        "203a817e57486e96d50b6f",
    },
    /* 13 */
    {
        "b1d31b3b264fc3631534d4cbe5851e14ba264fc6fb8047963e71711e4440124462ee0ef9790758097051dfe5538912b09b47b2e",
        "1e48be971bf861b59607b5d0d913b1850f3814c5bda4b1cae",
        "5df32dc34b1aa68d4b291de47f736317358151253f149a30434c2c1",
    },
    /* 14 */
    {
        "72ebd29fb80c63450e4e2ca8300ca70c66811bf5c785dd09f5032f61f75a932e0b6f2886a917d4d06f70cdde72101f623b549259f39f00ff1270",
        "38d8919d29ad7f2201cdd0e260a8bca2ba285a05e71e6cf88c547",
        "2058926f8d0c335db9b7b6ac20dac80c07a9988cdbb53241e47ab3ba45ae5210",
    },
    /* 15 */
    {
        "347ac5b14fd360f4160c08510656ef461622cd99e6ca1674fe2d0d93d722aee0",
        "1f17ff3883d06de393ed99485322df17d9905dcb473355017dd13cbe028",
        "1b012c",
    },
    /* 16 */
    {
        "432938c605a2d6d9a785099732964bf2e83b12ea92c2d31933c5796d0f00d1aea97dd6c3e24904810b6985cfa29aa7a900603719a0620adba9f4f93dfa0",
        "11f34eff91d52ed0fad6484abf60e37d71151e1ad314e34744c124b39b8fe3",
        "3bdd1282c7b867d0ca304b83fd24ed1e179c7f72b05c6423e01348755173e0",
    },

};

int ll_u256_mul_test_vector(void)
{
    unsigned int i;
    u64 tr[8], r[8], a[4], b[4];
    size_t rl, al, bl;

    for (i = 0; i < sizeof(u256_mul_test_vector) / sizeof(U256_MUL_TEST_VECTOR); i++) {
        /* clear trl, rl, a, b first */
        ll_set_zero(tr, 8);
        ll_set_zero(r, 8);
        ll_set_zero(a, 4);
        ll_set_zero(b, 4);

        ll_from_hex(r, &rl, (u8*)u256_mul_test_vector[i].r, strlen(u256_mul_test_vector[i].r));
        ll_from_hex(a, &al, (u8*)u256_mul_test_vector[i].a, strlen(u256_mul_test_vector[i].a));
        ll_from_hex(b, &bl, (u8*)u256_mul_test_vector[i].b, strlen(u256_mul_test_vector[i].b));

        /* r = a * b */
        ll_u256_mul(tr, a, b);
        if (ll_cmp_limbs(tr, r, 8, 8) != 0) {
            printf("ll_u256_mul_test_vector %d failed\n", i + 1);
            test_print_hex("r = ", tr, 8);
            test_print_hex("a = ", a, 4);
            test_print_hex("b = ", b, 4);
            printf("a * b should be :\n");
            test_print_hex("r = ", r, 8);
            return FP256_ERR;
        }
    }

    printf("ll_u256_mul_test_vector passed\n");
    return FP256_OK;
}

int main(int argc, char **argv)
{
    TEST_ARGS args;

    if (fp256_init() != FP256_OK)
        return -1;

    get_test_args(argc, argv, &args);
    test_rand_init();

    RETURN_IF_ERROR(run_test("ll_u256_mul", ll_u256_mul_test_vector, NULL, args.N, args.T));

    fp256_deinit();
    return 0;
}

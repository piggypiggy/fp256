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

/* r = (a * b) mod m */
typedef struct {
    char *r;
    char *a;
    char *b;
    char *m;
} FP256_MODMUL_TEST_VECTOR;

static FP256_MODMUL_TEST_VECTOR modmul_test_vector[] = {
    /* 1 */
    {
        "48e5a",
        "1063",
        "2074fce60a63d69d3dafda1625b79276ce",
        "8ddbde",
    },
    /* 2 */
    {
        "17a0f5f5a14f04d4b3afb50784b884b1186e95536f090d08146",
        "f9a7a781da1b022977fa35",
        "183ab2b361271ab6d34379a0ad4ee",
        "192989a41d1c019ed981d12d84b59e3aecd4a64ab27ce0d6a3bc6",
    },
    /* 3 */
    {
        "1ea5829",
        "20e66ad79366543f8c20ee3ad7bf5d4481b40869c1073",
        "529dfe8a7cb9ca4acf43b4cc4916",
        "36729dd",
    },
    /* 4 */
    {
        "2ce64fa5d0c40d9ccb128adbca1b1d663d8051f7d5a1e45e49e437da711c8b8",
        "6df83ee26b335e2baec0b72afd74da3444",
        "e90d42d4534a44a69946100be8cc3dbd2b4186638059401933e4d54c2d0db",
        "1a69766adb8a2bace219f278b6a51a5c2038b0b52d1a633dc6c3e01c1a366bd6",
    },
    /* 5 */
    {
        "99c7ebb898437f1e7cc2c9f6532287eb01fae884e8838d91a619554b",
        "6eb899c0944ea2d455fb7396a4e6cc4763b8a18b",
        "8fb64d4b031dbdcfc",
        "a824b2a0ffba0c01383ee6c9e820d83491b76ee8677abd90ff13a0b5",
    },
    /* 6 */
    {
        "2ff7d3a82c784b6b2e",
        "1fcbb684af05fb470435c5cc6ccd3019ef",
        "18dd3785c9eff1cdfb1504e3",
        "53441317161246db57",
    },
    /* 7 */
    {
        "0",
        "1",
        "1",
        "1",
    },
    /* 8 */
    {
        "6ed28ece04e2d9de6d8357472",
        "8d88c8a0ab",
        "c87335929ac7156",
        "785a6559e0a6cfbc67970500424cd125612bcb39d9341821e37f0064953d",
    },
    /* 9 */
    {
        "677b7ab8d2570eeec243554cf1",
        "1a83dd4f8834a077c87b3ae6bdd4c5f3a7b1de4c31f15ac5c3f27cf",
        "12179b0537274726627144b474c441a3cb2bb37587e262cae667",
        "f2f099f9de92fd07542be7a45de",
    },
    /* 10 */
    {
        "1",
        "2",
        "3",
        "5",
    },
    /* 11 */
    {
        "0",
        "255555555553333333333335555555555555555555555555555555555555",
        "0",
        "5555555555553333333333335555555555555555555555555",
    },
};

int fp256_mod_mul_test_vector(void)
{
    unsigned int i;
    fp256 tr, r, a, b, m;

    for (i = 0; i < sizeof(modmul_test_vector) / sizeof(FP256_MODMUL_TEST_VECTOR); i++) {
        fp256_from_hex(&r, (u8*)modmul_test_vector[i].r, strlen(modmul_test_vector[i].r));
        fp256_from_hex(&a, (u8*)modmul_test_vector[i].a, strlen(modmul_test_vector[i].a));
        fp256_from_hex(&b, (u8*)modmul_test_vector[i].b, strlen(modmul_test_vector[i].b));
        fp256_from_hex(&m, (u8*)modmul_test_vector[i].m, strlen(modmul_test_vector[i].m));

        /* r = (a * b) mod m */
        fp256_mod_mul(&tr, &a, &b, &m);
        if (fp256_cmp(&tr, &r) != 0) {
            printf("fp256_mod_mul_test_vector %d failed\n", i + 1);
            test_fp256_print_hex("r = ", &tr);
            test_fp256_print_hex("a = ", &a);
            test_fp256_print_hex("b = ", &b);
            test_fp256_print_hex("m = ", &m);
            printf("a*b mod m should be :\n");
            test_fp256_print_hex("r = ", &r);
            return FP256_ERR;
        }
    }

    printf("fp256_mod_mul_test_vector passed\n");
    return FP256_OK;
}

int main(int argc, char **argv)
{
    TEST_ARGS args;

    if (fp256_init() != FP256_OK)
        return -1;

    RETURN_IF_ERROR(get_test_args(argc, argv, &args));
    test_rand_init();

    RETURN_IF_ERROR(run_test("fp256_mod_mul", fp256_mod_mul_test_vector, NULL, args.N, args.T));

    fp256_deinit();
    return 0;
}

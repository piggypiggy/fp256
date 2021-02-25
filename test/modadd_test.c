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

/* r = (a + b) mod m */
typedef struct {
    char *r;
    char *a;
    char *b;
    char *m;
} FP256_MODADD_TEST_VECTOR;

static FP256_MODADD_TEST_VECTOR modadd_test_vector[] = {
    /* 1 */
    {
        "0",
        "2",
        "3",
        "1",
    },
    /* 2 */
    {
        "1",
        "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe",
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
        "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe",
    },
    /* 3 */
    {
        "2",
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
        "7",
    },
    /* 4 */
    {
        "444443777784443aaaccccd665ffeeeef5555555566656662222322666666611",
        "ddddddddddddddddddffffffffffeeeeeeeeeeeeefffffffbbbbbbbfffffffaa",
        "ffffff33333ffff66666666fff9999999ffffffffffff00000000fffffffffff",
        "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc",
    },
    /* 5 */
    {
        "6666666666666666666666666666666666666666666666666666666666666667",
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
        "5555555555555555555555555555555555555555555555555555555555555556",
        "9999999999999999999999999999999999999999999999999999999999999999",
    },
    /* 6 */
    {
        "1a0279402bff2850fa5ed43edaada3c68cc9b6ae06",
        "2055117883de72da3e1add7c7fd027c589c2e68d8b96820633ff4506c8b7",
        "312",
        "544dcd2e71fd6e0411f1ecee9d50d12a943cbb3251",
    },
    /* 7 */
    {
        "1ee45ff09cbd262c7fcc3d4a9bd0048fcc01cc78",
        "a635e91e47295dfb6f512ac3cf2901b2dee4a23",
        "1481015eb84a904cc8d72a9e5edd74749e138255",
        "d2bd2a726b8cc6e939b0a315d6bac30a899cbbf52b5a8243ebce77912b16",
    },
    /* 8 */
    {
        "6",
        "362cb06e57943ee93f3f",
        "330659c664e6f9d3dca3a87d4126d39e2d7dfe31ce502b73e8d37aed75",
        "7",
    },
    /* 9 */
    {
        "339718fcdd46e48b808912ea05fff3876cd2456b85f3e3c4cd9b4aac",
        "569c5",
        "966a7a6d5e977e7e6fb2d46089dc908b549db977b94dbbe2f34bfb3b057",
        "4c097b2848b147a189d101bc8ff2f3931bd1b3f3cf00e0ca95435e68",
    },
    /* 10 */
    {
        "cf855358219ff28dd9448feb9619667ddcc4faa6dcbbf6aa943c02",
        "3e9d72e645daf8c6b74c088ffd3bff00837abd1417bef07c6ef1f8aa8c1e7",
        "21970e623f46f8e378cdd2246525ec2ce01788bea7800cd7f1ec46582698f",
        "19be7756aa7564a3233baffa87b7c4642b0a5ee68133c1fab332a04",
    },
    /* 11 */
    {
        "1fbca8f408fe8d715dc697e59848659783a2489b42cbbd82",
        "c27278fb81444a613b5e212430c43f1b95f5868c8719a085da9ec",
        "6db5da9a06216d386181c57e1658b0a4e368bafcb720aa69",
        "7b05619695e9a83b85c688d4710f1bf0eb61a94cd929ff97",
    },
};

int fp256_mod_add_test_vector(void)
{
    unsigned int i;
    fp256 tr, r, a, b, m;

    for (i = 0; i < sizeof(modadd_test_vector) / sizeof(FP256_MODADD_TEST_VECTOR); i++) {
        fp256_from_hex(&r, (u8*)modadd_test_vector[i].r, strlen(modadd_test_vector[i].r));
        fp256_from_hex(&a, (u8*)modadd_test_vector[i].a, strlen(modadd_test_vector[i].a));
        fp256_from_hex(&b, (u8*)modadd_test_vector[i].b, strlen(modadd_test_vector[i].b));
        fp256_from_hex(&m, (u8*)modadd_test_vector[i].m, strlen(modadd_test_vector[i].m));

        /* r = (a + b) mod m */
        fp256_mod_add(&tr, &a, &b, &m);
        if (fp256_cmp(&tr, &r) != 0) {
            printf("fp256_mod_add_test_vector %d failed\n", i + 1);
            test_fp256_print_hex("r = ", &tr);
            test_fp256_print_hex("a = ", &a);
            test_fp256_print_hex("b = ", &b);
            test_fp256_print_hex("m = ", &m);
            printf("(a + b) mod m should be :\n");
            test_fp256_print_hex("r = ", &r);
            return FP256_ERR;
        }
    }

    printf("fp256_mod_add_test_vector passed\n");
    return FP256_OK;
}

int main(int argc, char **argv)
{
    TEST_ARGS args;

    if (fp256_init() != FP256_OK)
        return -1;

    RETURN_IF_ERROR(get_test_args(argc, argv, &args));
    test_rand_init();

    RETURN_IF_ERROR(run_test("fp256_mod_add", fp256_mod_add_test_vector, NULL, args.N, args.T));

    fp256_deinit();
    return 0;
}

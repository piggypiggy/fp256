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

/* r = a^2 mod m */
typedef struct {
    char *r;
    char *a;
    char *m;
} FP256_MODSQR_TEST_VECTOR;

static FP256_MODSQR_TEST_VECTOR modsqr_test_vector[] = {
    /* 1 */
    {
        "0",
        "0",
        "232335478798786756453454678",
    },
    /* 2 */
    {
        "133c95da94e21afc6018cd9",
        "deac8c5e4c4c3e38b8d9e4c3e4cb5a8f",
        "27efc0fc839ab094d3348e1",
    },
    /* 3 */
    {
        "8b7c90c337e5c510671",
        "183b2d8b6336bd5b5e246853108d3705f377bbfc38af7e8de75fa9e3e937b",
        "28d6314b5c566ddafe34",
    },
    /* 4 */
    {
        "e79c1ba5aaa579c94cb775309",
        "3cdffb38fd883",
        "34d660954aaaed4a10ee2595d7",
    },
    /* 5 */
    {
        "44b575e48285485e1566fef73f021a477c52066767e12658ea0",
        "5e0be8cbbe4d0be214324c53e0e72302aecb0ac064d8b59fd38428a",
        "4cca87e0a58a680fd59e5086251f33c2efb0764ff7a00ce6164",
    },
    /* 6 */
    {
        "6eb50175f8fce2106ed14530927ea3566ca4145c85f2ceaa3f",
        "7123e049b9e673362df3d5a5318c6e623ae4b0928f",
        "c4f3e2c8bcc0f9c30e2703cd391e2d91820fe5f5d560f23b32",
    },
    /* 7 */
    {
        "10d604d936308856ed75389a9b744588d8bcca077519b1386531d0",
        "3b57295af968d3a8c2f07561d86792d843f51ddd6638b3f1bb689980b6adf0",
        "25872f6a171cbaab810261d8a44c81d2d6a5e0a934b88f6a25397e",
    },
    /* 8 */
    {
        "4785b3a771847e396ac8f8405e05d8a6fb049341aa5efb86a3929",
        "21d40ddefc36ddc32ba9a7b554d",
        "bc7e684e9131f1bfe53756be2736d40f123697484742cb757665fb1",
    },
    /* 9 */
    {
        "9bf41a4",
        "3ec2d026b53a2c446fd2a070e33428d5af2c4c6c8e71c2a80fd88348d15e1a2",
        "13d47cf0",
    },
    /* 10 */
    {
        "1e76243bc302651d3d7cbfa610c5c0eb448c00",
        "e57313b83800cfa42bf4d42ce407b1431664",
        "67d6206ac2c8cf2524d47d4289baef8e706408",
    },
    /* 11 */
    {
        "8ca51",
        "2f7",
        "2c625d09cdb9dbbe4dcc04777f3a2b4d4478",
    },
    /* 12 */
    {
        "e56e4792ce30a061b2d007816ce299f5ff1f34a4a57cc581a1ad89",
        "f259fff6eb25b4a73f8f7281a43",
        "4b16a378c70a6c7a7813846b45c401dafd51f2359f3a34e5eb92f5a51138f6d9",
    },
    /* 13 */
    {
        "11b0baff702272a43181e0077",
        "2388a27d4422971038b0b3e89cad618447da0335cf6224b26b10d1dec6a685af",
        "2cd0f5bdeef760c4431721c5e",
    },
    /* 14 */
    {
        "0",
        "112e67cddcc658dc0ca21d6f3336cfe",
        "9",
    },
    /* 15 */
    {
        "41",
        "f6e92",
        "4f",
    },
    /* 16 */
    {
        "0",
        "df9f967d91ec7a9fed5b7be0c2ef0273c7bdfbae980e71",
        "1",
    },
    /* 17 */
    {
        "2c0ae4ab733b8c08",
        "f4ab561074557a7cdef48",
        "a759099786b5ee89",
    },
    /* 18 */
    {
        "2df1ccb0ae6b4585047d72272359a4097634bcb9ca2d3f070543a845",
        "cade255a4fdfa8a067159b08035b8eb0a9a53c2e5c87824d9fb24373ad",
        "3b3bef706243f244a3be08ac97a65d88f64032d06a278b5b52cabb2a",
    },
    /* 19 */
    {
        "78a5b4c985849d7bcef901adbb46d6",
        "8741735673c67414a075",
        "bf8843a15bd76dc66e1b2dd5147b19",
    },
    /* 20 */
    {
        "2344fa5cb43ce3bb326c7402458b9b61ed3d8327d48894318e98413a15c7e3",
        "1f1f9988e6e0c90d2393ab1ecd76d3a66c1dfb54797dcb511e5a7571",
        "2fa80f90837dfd650a9a39647dc7bc365164813a21b33fef04e784d06a986e",
    },
};

int fp256_mod_sqr_test_vector(void)
{
    unsigned int i;
    fp256 tr, r, a, m;

    for (i = 0; i < sizeof(modsqr_test_vector) / sizeof(FP256_MODSQR_TEST_VECTOR); i++) {
        fp256_from_hex(&r, (u8*)modsqr_test_vector[i].r, strlen(modsqr_test_vector[i].r));
        fp256_from_hex(&a, (u8*)modsqr_test_vector[i].a, strlen(modsqr_test_vector[i].a));
        fp256_from_hex(&m, (u8*)modsqr_test_vector[i].m, strlen(modsqr_test_vector[i].m));

        /* r = a^2 mod m */
        fp256_mod_sqr(&tr, &a, &m);
        if (fp256_cmp(&tr, &r) != 0) {
            printf("fp256_mod_sqr_test_vector %d failed\n", i + 1);
            test_fp256_print_hex("r = ", &tr);
            test_fp256_print_hex("a = ", &a);
            test_fp256_print_hex("m = ", &m);
            printf("a^2 mod m should be :\n");
            test_fp256_print_hex("r = ", &r);
            return FP256_ERR;
        }
    }

    printf("fp256_mod_sqr_test_vector passed\n");
    return FP256_OK;
}

int main(int argc, char **argv)
{
    TEST_ARGS args;

    if (fp256_init() != FP256_OK)
        return -1;

    RETURN_IF_ERROR(get_test_args(argc, argv, &args));
    test_rand_init();

    RETURN_IF_ERROR(run_test("fp256_mod_sqr", fp256_mod_sqr_test_vector, NULL, args.N, args.T));

    fp256_deinit();
    return 0;
}

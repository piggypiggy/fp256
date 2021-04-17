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

#pragma once

/* rem = num mod div
 * quo = num / div
 */
typedef struct {
    char *rem;
    char *quo;
    char *num;
    char *div;
} FP256_DIV_TEST_VECTOR;

static FP256_DIV_TEST_VECTOR div_test_vector[] = {
    /* 1 */
    {
        "0",
        "100000001000000010000000100000001000000010000000100000001",
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
        "ffffffff",
    },
    /* 2 */
    {
        "ffffffffffffffff",
        "10000000000000000000000010000000000000000",
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
        "ffffffffffffffffffffffff",
    },
    /* 3 */
    {
        "4444444444444444444444444444444444444",
        "0",
        "4444444444444444444444444444444444444",
        "1234567888888888888888888888888888888888888888888888888",
    },
    /* 4 */
    {
        "3bd61fe83c24fd846ac1eb11db8f20b98dfa3fb9bda35896781344084b",
        "17be34",
        "32a6d5912b753d917a52d9c017406730e4760ee34f950f12f8adebcd68963333",
        "22222222222222222222222222222222222222222222222222222222222",
    },
    /* 5 */
    {
        "1111111111111111111111111111111111111111111111111111110a",
        "7",
        "88888888888888888888888888888888888888888888888888888888",
        "11111111111111111111111111111111111111111111111111111112",
    },
    /* 6 */
    {
        "ca4ad16ebef144c3",
        "911ad3c2138296eca7298c4463a34ffaf64c161c3954cb52",
        "d8df7155669efba398ca8e662ede30e7834d6ee5eaff364a0552b9fb05edfe97",
        "17e9dbe5bc8be53da",
    },
    /* 7 */
    {
        "a43b83170004fb551fcc5",
        "5bedfc7a250b5cb388cc5b5e12925ef4d9530e35f4b",
        "7615e8266efaf0d09a92de77715fec103f02a8e568c909c929b29c7b8fe1f231",
        "148d6637a1a0fd799100c4",
    },
    /* 8 */
    {
        "6623609157b66efdf8c8dbd025bfc42f3f971ff3e5",
        "16f60dedc7069ad6c5a0cce",
        "b1384aa66eedb293991a0fca13cf4b2e47948f5c152ded903f574b8d76e3c193",
        "7b7df36213f792b893cc14aa693058cb72d9e77611",
    },
    /* 9 */
    {
        "87",
        "31a98f636440ae2fe1c7413c6b208bcdc93973fa16b87e1e2704d40cf2faf2",
        "533925498f0063e63d5ae8523f858a47da35475a18132b5887631759b32e8811",
        "1ad",
    },
    /* 10 */
    {
        "e62315bcb922422bff269db1cb85",
        "280464e46b9b1f7fe5d0cfc967e434ae0486",
        "4dd9fe2f2045ad442b4221e05f55b371521e70c4a007e01880c7d9544e000fc9",
        "1f2093e74364dd577ef2fb79a3836",
    },
    /* 11 */
    {
        "88a1fb7",
        "84e704a27e63b633140b7fb9901bbf544a3204b09a8b0f1a457296a29",
        "fefd4efec44f0dc34a4b8b87225e0b426ff7990cd62a85710930dcbb77e7e0df",
        "1eb2ace8",
    },
    /* 12 */
    {
        "746da861abd0099bfbd016f872fc9adfc4ca3a38292",
        "b37e",
        "59d3bf8d74f5bc93fe948c308d1d976d0956d2865ee325a",
        "801cf186539d3131b0a55b842c77125aab63ae0391c",
    },
    /* 13 */
    {
        "8bf07",
        "6a8bb6c4236fb7b873",
        "a1cb1df2f8ee77bab80c198",
        "184beb",
    },
    /* 14 */
    {
        "86c7344a5f2eed87be5666894c9ed51b859f8f9",
        "127b1153c1a66202b96a",
        "a3ffd28b8ed27aa7668f84b7147234707589f3ed5672f144fd9d41a561",
        "8dfc301436ce0501fb998a4dbbcad3b1c4851e4",
    },
    /* 15 */
    {
        "544b52ce1",
        "ba2b54",
        "5bdc65f8dd15229",
        "7e513a90a",
    },
    /* 16 */
    {
        "3",
        "2e33d93bc31be97f281c06f7ba4250b05e3f51d3be9c230615ce3672662d3",
        "19fd2a319dbfb357868fc3eb58c54d6335039e071b37d3b36c43fea059796e",
        "9",
    },
    /* 17 */
    {
        "6468d62f",
        "1212ed7f76ab505b7523966068b512e0be2db919869d978ce",
        "b09fa77345812c7e1d7100f288120281099a6098c15848165a710693",
        "9c5b46fe",
    },
    /* 18 */
    {
        "3ef5b9f7b717db",
        "8ab73040209138cf35524394862bd235987cd55462b7",
        "25a865e88aa84ccfc0b472be2abe0eda9957d6e94298f95ef79e75a267",
        "457f61db728dd4",
    },
    /* 19 */
    {
        "1f23a9c285d1f272b71e2a92969b03d4af4579ac0a",
        "340492beaf1bd095c16b46",
        "e097c3effe40cc211268c130b72cb54610a41690d684c26ce2befcec81dcda2",
        "4514ede1ebd95de0b67962842224380f093c0d00c4",
    },
    /* 20 */
    {
        "79a4a2a6e223e38e73a99e69a915941c24e81414ed9293c3d18d44d92",
        "4",
        "b3f7ec5d6a9c266e8f3da6a418f10b6eb4e4817509aaef415055525ad2",
        "2b17688cbf1e7a0d6a00c32f5f97ec8b3ca5800ceeb4718144cf1f8350",
    },
    /* 21 */
    {
        "0",
        "0",
        "0",
        "999999999999999999999999999",
    },
    /* 22 */
    {
        "cccccccccccccccc",
        "0",
        "cccccccccccccccc",
        "99999dddddddddddddddddddddddddd9999999999999999999999",
    },
    /* 23 */
    {
        "665190512125a5c241590774607112837bbef9aa812cca73",
        "db6471304b36eb4f",
        "bae935f418dd3fa3b207062fca4b963c829eb3d2b96ed28288de7301ce70286d",
        "da194b901904189639e04631ee9df3fe069818bd20137be6",
    },
    /* 24 */
    {
        "273e986047f1291e37afd6574ca67d81",
        "10daf38145190c152fd9b0caa942c8727",
        "47b3a8bae551d257f604a757809f61368b96f4d0d6033ddff15ecd4fe2ba2498",
        "44103f7d2e8a070b1df02a95396c5691",
    },
    /* 25 */
    {
        "71e424ad7b5fd4dc77837decfaab39a38476adefe2ac158f",
        "d2cd4a252eafd4b7",
        "aaf82b476ab6ebb1878d680186836ba8559e4e462609cc8a4190df63a718ae82",
        "cfa0832e1960246f1d062cbc7a4a0045d1d9b66478e279a5",
    },
    /* 26 */
    {
        "63e43981366007cd2ff5aec5a8d4b513",
        "c1bbe60439b3322ff210d703a1b5c6a7",
        "62a52a444f8e340f013b5d7b874c1c8de5e9cfd03dbe22233d6a883102389d57",
        "825985a49e73ca76db8d5d7e15e0221c",
    },
};

int fp256_div_test_vector(void)
{
    unsigned int i;
    fp256 trem, tquo, rem, quo, num, div;

    for (i = 0; i < sizeof(div_test_vector) / sizeof(FP256_DIV_TEST_VECTOR); i++) {
        fp256_from_hex(&rem, (u8*)div_test_vector[i].rem, strlen(div_test_vector[i].rem));
        fp256_from_hex(&quo, (u8*)div_test_vector[i].quo, strlen(div_test_vector[i].quo));
        fp256_from_hex(&num, (u8*)div_test_vector[i].num, strlen(div_test_vector[i].num));
        fp256_from_hex(&div, (u8*)div_test_vector[i].div, strlen(div_test_vector[i].div));

        fp256_div(&trem, &tquo, &num, &div);
        if (fp256_cmp(&trem, &rem) != 0 ||
            fp256_cmp(&tquo, &quo) != 0) {
            printf("fp256_div_test_vector %d failed\n", i + 1);
            test_fp256_print_hex("rem = ", &trem);
            test_fp256_print_hex("quo = ", &tquo);
            test_fp256_print_hex("num = ", &num);
            test_fp256_print_hex("div = ", &div);
            printf("rem and quo should be :\n");
            test_fp256_print_hex("rem = ", &rem);
            test_fp256_print_hex("quo = ", &quo);
            return FP256_ERR;
        }
    }

    printf("fp256_div_test_vector passed\n");
    return FP256_OK;
}

void* fp256_div_test(void *p)
{
    int64_t i, N;
    fp256 rem, quo, num, div, tmp;
    size_t nl, dl;
    TEST_THREAD_DATA *td;

    td = (TEST_THREAD_DATA*)p;
    N = td->N;

    for (i = 0; i < N; i++) {
        /* div should not be 0, it has been tested in test_case */
        do {
            nl = rand() % 5;
            dl = rand() % 5;
            fp256_rand_limbs(&num, nl);
            fp256_rand_limbs(&div, dl);
        } while (fp256_is_zero(&div) == 1);

        /* rem = num mod div, quo = num / div */
        if (fp256_div(&rem, &quo, &num, &div) != FP256_OK) {
            printf("fp256_div failed %" PRId64 "\n", i+1);
            goto end;
        }

        /* rem = num mod div
         * quo = num / div
         */
        if (fp256_mullo(&tmp, &quo, &div) != FP256_OK)
            goto end;
        if (fp256_add(&tmp, &tmp, &rem) != FP256_OK)
            goto end;

        /* compare tmp and num */
        if (fp256_cmp(&tmp, &num) != 0) {
            printf("fp256_div : quo * div + rem != num, %" PRId64 "\n", i+1);
            test_fp256_print_hex("rem = ", &rem);
            test_fp256_print_hex("quo = ", &quo);
            test_fp256_print_hex("num = ", &num);
            test_fp256_print_hex("div = ", &div);
            test_fp256_print_hex("tmp = ", &tmp);
            goto end;
        }
    }

    printf("fp256_div test passed \n");
    td->ok = FP256_OK;
end:
    return NULL;
}

int fp256_naive_div_test_vector(void)
{
    unsigned int i;
    fp256 trem, tquo, rem, quo, num, div;

    for (i = 0; i < sizeof(div_test_vector) / sizeof(FP256_DIV_TEST_VECTOR); i++) {
        fp256_from_hex(&rem, (u8*)div_test_vector[i].rem, strlen(div_test_vector[i].rem));
        fp256_from_hex(&quo, (u8*)div_test_vector[i].quo, strlen(div_test_vector[i].quo));
        fp256_from_hex(&num, (u8*)div_test_vector[i].num, strlen(div_test_vector[i].num));
        fp256_from_hex(&div, (u8*)div_test_vector[i].div, strlen(div_test_vector[i].div));

        fp256_naive_div(&trem, &tquo, &num, &div);
        if (fp256_cmp(&trem, &rem) != 0 ||
            fp256_cmp(&tquo, &quo) != 0) {
            printf("fp256_naive_div_test_vector %d failed\n", i + 1);
            test_fp256_print_hex("rem = ", &trem);
            test_fp256_print_hex("quo = ", &tquo);
            test_fp256_print_hex("num = ", &num);
            test_fp256_print_hex("div = ", &div);
            printf("rem and quo should be :\n");
            test_fp256_print_hex("rem = ", &rem);
            test_fp256_print_hex("quo = ", &quo);
            return FP256_ERR;
        }
    }

    printf("fp256_naive_div_test_vector passed\n");
    return FP256_OK;
}

void* fp256_naive_div_test(void *p)
{
    int64_t i, N;
    fp256 rem, quo, num, div, tmp;
    size_t nl, dl;
    TEST_THREAD_DATA *td;

    td = (TEST_THREAD_DATA*)p;
    N = td->N;

    for (i = 0; i < N; i++) {
        /* div should not be 0, it has been tested in test_case */
        do {
            nl = rand() % 5;
            dl = rand() % 5;
            fp256_rand_limbs(&num, nl);
            fp256_rand_limbs(&div, dl);
        } while (fp256_is_zero(&div) == 1);

        /* rem = num mod div, quo = num / div */
        if (fp256_naive_div(&rem, &quo, &num, &div) != FP256_OK) {
            printf("fp256_naive_div failed %" PRId64 "\n", i+1);
            goto end;
        }

        /* rem = num mod div
         * quo = num / div
         */
        if (fp256_mullo(&tmp, &quo, &div) != FP256_OK)
            goto end;
        if (fp256_add(&tmp, &tmp, &rem) != FP256_OK)
            goto end;

        /* compare tmp and num */
        if (fp256_cmp(&tmp, &num) != 0) {
            printf("fp256_naive_div : quo * div + rem != num, %" PRId64 "\n", i+1);
            test_fp256_print_hex("rem = ", &rem);
            test_fp256_print_hex("quo = ", &quo);
            test_fp256_print_hex("num = ", &num);
            test_fp256_print_hex("div = ", &div);
            test_fp256_print_hex("tmp = ", &tmp);
            goto end;
        }
    }

    printf("fp256_naive_div test passed \n");
    td->ok = FP256_OK;
end:
    return NULL;
}

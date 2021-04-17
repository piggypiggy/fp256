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

/* inv = a^{-1} mod m 
 * or 1 = (a * inv) mod m 
 */
typedef struct {
    char *inv;
    char *a;
    char *m;
} FP256_MODINV_TEST_VECTOR;

static FP256_MODINV_TEST_VECTOR modinv_test_vector[] = {
    /* 1 */
    {
        "0",
        "dddddddddddddddddddddddd",
        "1",
    },
    /* 2 */
    {
        "11111111111111111111111111111111111110f",
        "88888888888888888888888888888888888888",
        "111111111111111111111111111111111111111",
    },
    /* 3 */
    {
        "1",
        "1",
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffff",
    },
    /* 4 */
    {
        "800000000000000000000000000000000000000000000000000000",
        "2",
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffff",
    },
    /* 5 */
    {
        "4443444344434443444344434443444344434443444344434443444344434",
        "4444",
        "4444444444444444444444444444444444444444444444444444444444444",
    },
    /* 6 */
    {
        "1",
        "9bad1117d141ecc",
        "483",
    },
    /* 7 */
    {
        "fc391ebb",
        "3a5ef635bf77c40a105c701a30a4f0686ce438972e0d344e53",
        "2a1779ddf0",
    },
    /* 8 */
    {
        "4266e61aa7e0f608531b635417c24d73515f9a07881e870f7a830f8b",
        "570d894f1194ff4702e5dd",
        "77ebb18f3e62d3716cb861848d09f1c7c6d067de63a0087eef6d2dde",
    },
    /* 9 */
    {
        "17aaa781bd9f",
        "2c815a587",
        "2a455e68df0c",
    },
    /* 10 */
    {
        "11ddd1c945eee60efa0191f315b803f3d",
        "25963394aea0a789e40dc335df693458714139fdc1f891929c0d",
        "15d8b8ed7bc21dd4e169fce6e0b5b6598",
    },
    /* 11 */
    {
        "1bbb0f0c3a1f0beb849a3b05",
        "5096c18b46c2ea39ecc6d34ec14bf91decf2d9be787dcfe50222b",
        "55390007bf9970379615d5fd",
    },
    /* 12 */
    {
        "19a3ca9fd20051c740df261d3f4195c5b5230a28aacde1d",
        "511946b1ed244c03a47d00699122f63b290dbd27416c401",
        "2945d062944f3f57079372871d997ead54aaf4fe77300b7",
    },
    /* 13 */
    {
        "5f1e53296074a05f768a1d5a11230e6e406b2a6ea5",
        "162963b342071d737167aba3",
        "5f60c103fd89f93d6e90a9a4035810534a4d61e6d2",
    },
    /* 14 */
    {
        "174c7bd785a259f5",
        "888b90843ce19ce77f648d1421e7ef8143dff2551c0010597e81f99d16c55",
        "1b630ad03ba0b8f2",
    },
    /* 15 */
    {
        "91",
        "5118c9d447ced8bed4715e089e95747551e2202861ad49a19c75dc1173a1cdf",
        "9d",
    },
    /* 16 */
    {
        "32b6a1f480ac775fc1dd0b237e048ae5888a9b12ad3ef25f96c",
        "3730a061b4879288440a963f3ec86615ab0bddf9b2dd777220",
        "39599064893dbde46f869cb4dfd1b202fafe50669484dfd78253",
    },
    /* 17 */
    {
        "6",
        "17e30b",
        "23",
    },
    /* 18 */
    {
        "1",
        "18cbf94f5e466e52776e5693019944f5392c4fe4975a6de063bfc54fa06c",
        "37",
    },
    /* 19 */
    {
        "12ed04efb157b56cd94995e36bc4dc9bb840bc5e63353a595f9d314dadc4f5",
        "26e24e376e61c9ee6694210866092565c70c78c32c1a4b12a29e05d4cac1742",
        "315454b7eb4bd35fae1fdc90b4e83452185b218752194f2556c32a054ef81b",
    },
    /* 20 */
    {
        "144219fcca1142b4cb150ef990d95f444d38d5380064d",
        "8",
        "36059aa21ad8b1e21d8d7d442cee5360cdece340010cd",
    },
    /* 21 */
    {
        "6cc7b5e3cb5f5a7693f5822fe2ab999a9861b196344ff3cd23",
        "a7c709ab174158ece0d7d73aee3c0bce6b3d5f29f97a71",
        "a781f04091d9c048ec50b5fe0995314db59888eb56d8de9391",
    },
    /* 22 */
    {
        "e9f42ee2d1fe3e7cc3f3162e336cd21b25f6e6f11f179d1f29356b70eba1c1b",
        "73d26620dd0c9e54a2ef21ad7a493a5e0fcdc5fd91e9e15",
        "1282fe993da4203d1648db44029670eab8939fae203cca75fbbd4be7aaaa7f0e",
    },
    /* 23 */
    {
        "864299ffe969176fda57964f2022171e0fd341d91e206",
        "3145e10fc820dd79c0da7d1a90b7d21e7e9917f54dbf57ecce3e4fb",
        "276df1cefa92b473c63c8b6381d3dd6e5fa18f2b91a8b7",
    },
    /* 24 */
    {
        "5896c5cd5ddcbd436e571fab8084d371a414a97007b2524c64ece874742f0c5",
        "6617add7c10132390562640a6b8e20797fd9196f0b9e",
        "1372462e79e422e2e0fd05c7a0c4a35e2b3fd7d65a0981655c8c3ce5d791658f",
    },
    /* 25 */
    {
        "204129d870723a502f66a1fdce77d8586d6bb82f499070398f",
        "1476f94d71ac908ca6db43f5c5a619a537c09abffb945c5c878805cf3e0b5f2",
        "4ff2c4e842e458637108672d030756f13f9dc789417c0ebce9",
    },
    /* 26 */
    {
        "1c5bd3d6fe8dce0adc8684a7f7",
        "16a735202b17bf7135799a4b7d242c8681c0ba4829209014a6158e4eb3",
        "6acbb80f4ed3cdeab14ae6c08e",
    },
    /* 27 */
    {
        "3bb751b30c3a5dc3b53153809846f9edd279fb88283ff8bcfe87b",
        "2",
        "776ea3661874bb876a62a701308df3dba4f3f710507ff179fd0f5",
    },
    /* 28 */
    {
        "2642c02256282287a3b1",
        "20b71eebba6a7ebfd04f7fa335201a8853dbd8a9180543c12b6291e7bc66351",
        "3e9d5d98123bb5a22280",
    },
    /* 29 */
    {
        "3a665459f92b4d3742b77a3be1129d43de85ae9512ec",
        "7ff68c06a6d91969a754f7f758c20ef8b76faf0ee206",
        "9ee25df6fff25b8aab2297c10e7442e65d7277eea7a5",
    },
    /* 30 */
    {
        "3",
        "316e9fcf1ea653fbdf083a55e87b9a36c661b6b67aab",
        "40",
    },
    /* 31 */
    {
        "29384892db8fc837731d3a65eb4360aedfbb7207393",
        "136b12075fd39567092516b9bc4b12b59e9719e08b7b423880176010193837",
        "58d1b162a8bee9b5bee9acfeaacb22733b6863a713a",
    },
    /* 32 */
    {
        "75e638f9f",
        "691cc78efa1563d8977e5bbc4140b1adfa39361be68e9d5a88a318b8a9609",
        "97854c6c2",
    },
    /* 33 */
    {
        "8c99b965178e75a1efd52fe0b361619c46f8a7f16f018c890315ad8f012c93",
        "2ba73d26ac70f6dba77327337cdbcde77447d2692e13e5c9772abfdd177",
        "9f231656710989db44ddf8a74aedf2e55967042e153b53fd1a2f34ba338ef9",
    },
    /* 34 */
    {
        "1",
        "a41a43c6d77d18bbfa8ad3aabcb4e158fc56737a8aa8b6e140458c983a3c9",
        "4",
    },
    /* 35 */
    {
        "5edf6dc3e7f5476dde7de838d05b417319084d5fa06ac7c9793d4c1b0211",
        "583bcf98f6a13a44fdbd8294fcca1fa6fbc33d006ecd3f91d8198f06c807d2c9",
        "784809b42977df37fa3bb828c3bf7be2f9126948fffa447525c7751cfcde",
    },
    /* 36 */
    {
        "382ae",
        "5ec8d0a2",
        "14e1a3",
    },
    /* 37 */
    {
        "279adfa898fa1a3c9e0baddbfa0cda3597ae0f6ac55c001ccc48463ab0",
        "a2a9b48fc3308cb8f3821655146370d6b3b72cc09fe32f8427d6df0daa3f",
        "2a9945789b917f3b094464f32ea941f58ad185f80522630d78b8c6f46d",
    },
    /* 38 */
    {
        "51319a3249b074b2d760f152687f33eee33d079537d80b8f360d2ee",
        "4f5f2319c5ce3183164647619292d225bf14aad33fbeebe2ce86c5dd4be606",
        "aff1d02df0350baead9fa16c3a34d532a028cafedff9d856f557d5f",
    },
    /* 39 */
    {
        "1f59d70868207121f696a5354abbe31bf0b3c4842ee80a2d2acf8f25b",
        "7e51c341a12ba57f1dd439e4cd5173ad0722a2ff364c313814555ce03",
        "40a642c24b5fc89357bd2bfe77ea0b57ed6cdb7578819e2f76dccf7235",
    },
    /* 40 */
    {
        "7081dfdbb81569266",
        "5cf5ad029e3019e80999337b1fd505bccf5522c432bbb5312146c6e81f32eff",
        "df5ca5dfc8bbd4633",
    },
    /* 41 */
    {
        "10a1d67a57dd41d5813cba7be0a0ceb173ef4c23eedb5ba",
        "c4a4dd2916b9b6599bac7c97385d6fcea5179ef7c3c7740ef356ca20a489074d",
        "40289787516396c346929ff580471b19acba4fcb4fbea4d3",
    },
    /* 42 */
    {
        "190d59fcf41d2ae0b9be663c22ae0181",
        "7fc69b6903b35b8bd38bd179c839458c346631930ccb6409",
        "82c8f5f5cf2401f9f3f50d835c4f0478",
    },
};

int fp256_mod_inv_test_vector(void)
{
    unsigned int i;
    fp256 tinv, inv, a, m;

    for (i = 0; i < sizeof(modinv_test_vector) / sizeof(FP256_MODINV_TEST_VECTOR); i++) {
        fp256_from_hex(&inv, (u8*)modinv_test_vector[i].inv, strlen(modinv_test_vector[i].inv));
        fp256_from_hex(&a,   (u8*)modinv_test_vector[i].a,   strlen(modinv_test_vector[i].a));
        fp256_from_hex(&m,   (u8*)modinv_test_vector[i].m,   strlen(modinv_test_vector[i].m));

        fp256_mod_inv(&tinv, &a, &m);
        if (fp256_cmp(&tinv, &inv) != 0) {
            printf("fp256_mod_inv_test_vector %d failed\n", i + 1);
            test_fp256_print_hex("inv = ", &tinv);
            test_fp256_print_hex("a   = ", &a);
            test_fp256_print_hex("m   = ", &m);
            printf("inv should be :\n");
            test_fp256_print_hex("inv = ", &inv);
            return FP256_ERR;
        }
    }

    printf("fp256_mod_inv_test_vector passed\n");
    return FP256_OK;
}

void* fp256_mod_inv_test(void *p)
{
    int64_t i, N;
    fp256 inv, a, m, z, gcd;
    size_t al, ml;
    TEST_THREAD_DATA *td;

    td = (TEST_THREAD_DATA*)p;
    N = td->N;

    for (i = 0; i < N; i++) {
        /* random a, m */
        do { 
            al = rand();
            ml = rand();
            al %= 5;
            ml %= 5;
            fp256_rand_limbs(&a, al);
            fp256_rand_limbs(&m, ml);
        } while (fp256_is_zero(&a) || fp256_is_zero(&m));

        /* inv = a^{-1} mod m */
        if (fp256_mod_inv(&inv, &a, &m) != FP256_OK) {
            printf("fp256_mod_inv failed %" PRId64 "\n", i+1);
            goto end;
        }

        /* z = (a * inv) mod m */
        if (fp256_mod_mul(&z, &a, &inv, &m) != FP256_OK)
            goto end;

        fp256_gcd(&gcd, &a, &m);

        /* a * inv mod m should be equal to gcd(a, m) */
        if (fp256_cmp(&z, &gcd) != 0) {
            printf("fp256_mod_inv failed %" PRId64 "\n", i+1);
            test_fp256_print_hex("inv         = ", &inv);
            test_fp256_print_hex("a           = ", &a);
            test_fp256_print_hex("m           = ", &m);
            test_fp256_print_hex("a*inv mod m = ", &z);
            test_fp256_print_hex("gcd(a, m)   = ", &gcd);
            printf("\n");
            goto end;
        }
    }

    printf("fp256_mod_inv test passed \n");
    td->ok = FP256_OK;
end:
    return NULL;
}

int main(int argc, char **argv)
{
    TEST_ARGS args;

    if (fp256_init() != FP256_OK)
        return -1;

    RETURN_IF_ERROR(get_test_args(argc, argv, &args));
    test_rand_init();

    RETURN_IF_ERROR(run_test("fp256_mod_inv", fp256_mod_inv_test_vector, fp256_mod_inv_test, args.N, args.T));

    fp256_deinit();
    return 0;
}

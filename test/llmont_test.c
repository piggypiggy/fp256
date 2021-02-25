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

/* r = A * B * R^{-1} mod N */
typedef struct {
    char *r;
    char *A;
    char *B;
    char *N;
} MONT_MUL_TEST_VECTOR;

static MONT_MUL_TEST_VECTOR mont_mul_test_vector[] = {
    /* 1 */
    {
        "1",
        "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe",
        "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe",
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
    },
    /* 2 */
    {
        "0",
        "0",
        "fffffffffffffffffffffffffffffffffff",
        "fffffffffffffffffffffffffffffffffffffffffffffffffffffffff1",
    },
    /* 3 */
    {
        "b",
        "b",
        "1",
        "f",
    },
    /* 4 */
    {
        "126611cc127763a458e4da9ab386da7b82289b724cf659cfcbb2be4972a5042455149c884350a3a2d524bf9ac2ba625a5546d47ae4967be0e23a15089",
        "11a4282fe372ea7b2564967ef345c654194",
        "e598f85d35055bce123f507f56113fde03",
        "12d03e2007226caf61e8666c0f225daaf931f97c7c5d9919949fbee07c0d5b2e0894affa7527bb973c76661c1934c9bf97a90a32b391432f77a028353",
    },
    /* 5 */
    {
        "320f784489526071312",
        "3c1a0222373a46b8f94",
        "3fc5dea501056ff0669",
        "5d1ca0cb8500169f9b3",
    },
    /* 6 */
    {
        "349746e3009f49d75ef27e24c56de34bf3eaa4",
        "e02e024379c510564945173444ac3c5afddb2d",
        "fff5a8346b8363808f47b005d9fcc08225d3b1",
        "1287f0f1ac7dcdd805c3064022cb1c22f371b9f",
    },
    /* 7 */
    {
        "7ecc60237f558948979f16df8c3c2003a22c3e1cd7aa6d9973b35e72f35b",
        "520c901bc22fc73ac2becbaa154610bfd8d4c22e55f1f2af987a289e1bf9",
        "41d587371a78a76eb7a8bb4845cb61439cc93ca7596e65e1585c49225037",
        "11566a8623dd635ae90123dda15943374a2c6db8fd9daa9d7fc267f29c73b",
    },
    /* 8 */
    {
        "14f68b6e1bd5c4ac79d049028f86082bb28ad43124efbe2260159a47",
        "f999dc2d77cf5959055f6ef7788ed865fb1d448b00910542959879d",
        "df2fdca378e5a6e12e9d50df30d62ac2b2e0c7a2218c9d042c11910",
        "3248f03e0d2be7cd8c05380e10f97b587cfa75c546cb0e57bbe31ad1",
    },
    /* 9 */
    {
        "559c65bbf0babe0137dba2c13ed00b37413abc6dcb6df7147cd7197a1f404e0d7d6edb3957a1e282a4feb8db30ee2b9f1bf1682fd7447",
        "2cf0eb1ea5470a5a70cf249078264a4ddb845c4a1e4a61f548ecd0ec",
        "109bb8ef0d4257abd05554b84d891ef046c46e9a75c81bf1cb1f4514852cd96be6fcf2b1c79de2c",
        "1fbb7db17bfa804654ed6350c866ebfda43092d8f1f41469522beeaa9490de39ee56596c85a23b09324ee0f65b0ae69cd52c3cadb1ab97",
    },
    /* 10 */
    {
        "1e919f4ea09d838ab4c2d2ae4daa7b6eec73cc73efc4fb0209b3144bd0e47241b5efb9d6b239e1eba299e45f448067c05f8c765c027f26a1ad8b58c83c35",
        "8d98b2983ae820f1f0893bfb04c2283ae0506336e75dcaf43372a75c0b71e407",
        "22f4933ce586cedcfd8a85e5bf7c23eafb779a84a0e2529e7596ff3fc",
        "257e1c0f820e5a472f711190b10720edbe6269d892df126c38b38681ef586f40b563567aab6bf63c1cceddef12697ca093e7329a7ca124778a01b178916f",
    },
    /* 11 */
    {
        "243e5c547f5b3f44f0d478cbb347d292bb6c9ec45dc2019f8175c903a67629fd2b7108bf3a2aae1ab18740",
        "342f51b4c38045e70c315a8d5452e74172bbd9e1c7e36f06e1e434b88f7409abf",
        "3fd87f28de9cfdadb2d4cc98500b5dab835588919c936671bf0356fb31ff456b017ef227403a85eb142a7f",
        "3fed89402b9a6b1ec78e6882cb0001bbacf4a3802ff14ba67fa5a2e2abf9699c57d3e045d0a0f4560c4307",
    },
    /* 12 */
    {
        "e82eb135c3761045c7e5b28f701f6233fc9cb7008",
        "1ac855882a53fe3cdbe9125e8e87d28f15848159b4",
        "4ac1",
        "1b7a793a9d6ff20cc5c0751f000380d3657fbbbcc9",
    },
    /* 13 */
    {
        "44f352e2272136d9f709e1763836ae83b0a1ea1f7387b7f4580c65a6316c3799c74d07c0bffee571f65a68656747cb31da4eb1e99ea",
        "a1e07d21c3a63233fad149f12707ef4bbf9786a7c0332dc6e3a442f20662472386f6e56cbb4c139bae334627958508599a93a705f4",
        "a6ea26813e3bbf78025b4ac58468c0f3a5a768d9220268d1bce6859635010d9820eb5668de3e703b1b0267b1ecc005fcfaf7e1b0363",
        "3895b69f6b0e86cc41cdc78a419dcd8659e104b7c240f741390746bfcd98a6913f29bd1e861ce626bd02af5cb8146f41746ff83cf921",
    },
    /* 14 */
    {
        "3016",
        "1d45",
        "6e",
        "506b",
    },
    /* 15 */
    {
        "260da8c6f36ff4c5131f6e7fc955f2cde93f7b1b2f3bd894c3783c6b2a92d99e135ad6a6ec010060f7fee7",
        "1",
        "1edeb0fc944d712a0aed0bef2913ce33649808cde4830026f253d4320753af802443e750b83fcd2190f5e9",
        "342af0629ab4f2c0bde7e8a9ef2b71e7a346c2f4ee89424ad83b7bd89461a5f5ede41c239200879944d6a7",
    },
    /* 16 */
    {
        "2",
        "8",
        "4",
        "f",
    },
    /* 17 */
    {
        "4d1db9c42d82253b8e803eec5c244d21f1a26b01f7ca95e69422ee01adffc11d010d86de0c356d63cf89a8e6961ea350d800662b90153dcef5d9582013114a9",
        "ac8e5270cfa37c08a1719377781f451c228835755",
        "f6e9a3c606b2ce34ef900a3af1659dc32876a507c196686ad127b33f63e806eb274f5a4f866825605076365c071607f01c4926723386",
        "7f6eeb55c369179972bb31f548a6d91d8a937a25d1aa97355e5e9618c286595561031af37dd24a8dc96eddea5a4d06438ef9e2d093e721004929281f3f83281",
    },
    /* 18 */
    {
        "1b64d795895c0afd1012864586b887fb9a830a5ce3bafe506a644fdcb3687a6d3919391f4aebcb21491b68069d345",
        "de2fa662c35c6bfb84337510ff6ec9c360b2937678ae90012326960a2ba7b26634986821b805b586d05fd0cb7962",
        "c99e7d670e4764f47965",
        "20508eb5ccde9e474a6abe46de0c0ecac682b5d5ecda6fefcc65bc789471518ccf228488ce985dae0e699e55797c9",
    },
    /* 19 */
    {
        "12c40741e6c231ca728b0c3068ebee5bd57df0a19572caf84ee4ef84d1f",
        "8e9c853a1c0016e68c942728c5277f2c4b1411f228fc17a0fae0bfcbc6",
        "28494adb414a00213b5e52a97f10d627b039193027aa8824d404615654d",
        "3eafa29f143c39c9f1105661fdc6cda06c57b0b66391005bb6b0b60cc4b",
    },
    /* 20 */
    {
        "75e6c46f4ac2ba3bfda8540b5a5654c4733764eb1460def6532052a293916162e44b08ba563f621d8c4f4f69c83d8f557fde5a09218bf4",
        "41a909e68f4700d9718ced9d664a1967e57d5783469ec8301a2f355a3a872b2c940751750b6bd0753375bd79ede1607f72496b439c49e8",
        "637975751ceb6827e91a7ed2de48f07c93cdfaf7a4f180ee37e813fd9632074a115413272abc57193ca0cd114b64d61852be179bb7bd24",
        "d1df4549d60195ec52dfedfd48688564ac3f5988dbc950a716f0094a5c636b73daa43daf1b0a62e149b3aebadae7ab65d18e6532e5fb1b",
    },
    /* 21 */
    {
        "fc4c4ec9c43b706ce22073a7920ebd4285f59d3be0805fef3854c",
        "119dbdf77702133ff9f738a04514db6e7fa0ad2f0482f01873d497",
        "10acd783859a3b930b961b3d5f5c1df709d74934a02d234f6a799b",
        "1864f188c5dbca59eac3c97c8667bcb68aba551308f248a8f2eec7",
    },
    /* 22 */
    {
        "16dcef1af61f0983b9731fe3253495be75b11ee8761c6f1a432507d3bd58ff886591ac04f5998f78410cf63cf7c99825025ddcc9a6bba4cfa32c9740b",
        "36aa1172b798e67",
        "1a9ec2897412ff28611efc8ce4d7a7e87bb442e8b7b8b98b34a3d5344664fe248ba60",
        "19c0c61631716fcc02e942d55fee4c53ac7770bc03ac6fad933067ea6c4bcfa1b0806af0a91f3fefe7eb7c6247ff118d9e00cb7cdfc7addf56bcb8e6f",
    },
    /* 23 */
    {
        "269111a7dde2216910b844f0",
        "8ba706731c586e7b18855f58",
        "1f2dc657f01afb034",
        "90531ff8d7d8d806a4c9b11d",
    },
    /* 24 */
    {
        "c0e677800fe622249acccd795",
        "4f99a14a0e1b82b1574936253",
        "6690ef",
        "d233286d4b43d2a2810433a2b",
    },
    /* 25 */
    {
        "113e9a9ce96a3cb49d0218b9a90be00584f9",
        "d2e7",
        "240018aca664e513c5c5bd5d135ec91c68c3",
        "2462a7b491e37f894d290221791274dad5d7",
    },
    /* 26 */
    {
        "bd45635efdbd862c0340dde252a8613cd0195d997393057f78c7c64817178",
        "3cfad9fbe158e2581715000e3f84a5efbba8369d034661a4",
        "712edd475ccd26e5a95144d07a95de61",
        "36ead0657c16df65317ff3666a2a2d44261b41b42906d0bdc59531c74bc751",
    },
    /* 27 */
    {
        "147799989ce7e456b099015c140891570f3ac013e2af2954cce85167bf9a274fc927",
        "1724b2ce849576684f6db26591c09e4",
        "587c7a26c0ecce309f5369b7ec911f85667d5b1a16aef0075cfdee63ab3d33c5c1",
        "695fa701b1ecda8c6d0cf01523a74217d12eb74778d24a6391b86467adccc6fb7a35",
    },
    /* 28 */
    {
        "6893096e4dda83bfa0270840dd61818f38cd8ab85d204b4480c3ef06eee32f20ccda367bb",
        "61b8d5f64086a26e2f59a39778ba42258ce5fe8",
        "2",
        "db2edda6ed044626c633cac61815cc92ec4bcc5c6cb7f1bf3a04eaf156cda17faac67db3b",
    },
    /* 29 */
    {
        "1ba8183ba0f07e1c18233d3feb74d3ceaa35c7e818fe4b41d5bc95a2a0c69e5d28541455c690ab9434a22c90677747111990da835112afb973e530a6027aad83",
        "323a2e57572bd15cf2c6c7675507cb530b6c9a9586809df5786be353a07efe37f926fa5a16d39d5c140611f3604474676124c23a",
        "d644f505d4a627d480b828a94291056ec92c69386bf8796e5b763a4317be801698f063785306e4a860c185c09d7645e0121ee909572eb0e864301a7b",
        "2c169512fa4b4b850863433d445daf6e3509b3ad1daa211b08af0de93b9c0ebfc05d9e4a98d3d8cf80d8d8dc0bbae49241588b337deacf8f5b84bc216580428f",
    },
    /* 30 */
    {
        "7201f860310000d69c34acd0de3f892333e181a03ea10b7a7eb66d01e",
        "918a6326e6d8859e619f8185",
        "fe88f0be4ad7beb64f",
        "a51a32f5b02587384723f8c3cdaa3a51f9b1a46a34a42e4d2e88f7317",
    },
};

int ll_mont_mul_test_vector(void)
{
    unsigned int i;
    u64 tr[8], r[8], A[8], B[8], N[8], k0;
    size_t rl, al, bl, nl;

    for (i = 0; i < sizeof(mont_mul_test_vector) / sizeof(MONT_MUL_TEST_VECTOR); i++) {
        /* clear */
        ll_set_zero(tr, 8);
        ll_set_zero(r, 8);
        ll_set_zero(A, 8);
        ll_set_zero(B, 8);

        ll_from_hex(r, &rl, (u8*)mont_mul_test_vector[i].r, strlen(mont_mul_test_vector[i].r));
        ll_from_hex(A, &al, (u8*)mont_mul_test_vector[i].A, strlen(mont_mul_test_vector[i].A));
        ll_from_hex(B, &bl, (u8*)mont_mul_test_vector[i].B, strlen(mont_mul_test_vector[i].B));
        ll_from_hex(N, &nl, (u8*)mont_mul_test_vector[i].N, strlen(mont_mul_test_vector[i].N));

        /* tr = A * B */
        k0 = ll_invert_limb(N[0]);
        ll_mont_mul(tr, A, B, N, k0, nl);
        if (ll_cmp_limbs(tr, r, nl, nl) != 0) {
            printf("ll_mont_mul_test_vector %d failed\n", i + 1);
            test_print_hex("r = ", tr, nl);
            test_print_hex("A = ", A, nl);
            test_print_hex("B = ", B, nl);
            test_print_hex("N = ", N, nl);
            printf("A * B should be :\n");
            test_print_hex("r = ", r, nl);
            return FP256_ERR;
        }
    }

    printf("ll_mont_mul_test_vector passed\n");
    return FP256_OK;
}

// void* ll_mont_mul_test(void *p)
// {
//     int64_t i, N;
//     u64 r[8], tr1[8], tr2[8], a[8], b[8], ta[8], tb[8], A[8], B[8], odd[8], k0;
//     size_t al, bl, ol, w;
//     TEST_THREAD_DATA *td;

//     td = (TEST_THREAD_DATA*)p;
//     N = td->N;

//     for (i = 0; i < N; i++) {
//         /* random odd modulus */
//         do {
//             al = rand() % 8 + 1;
//             ll_rand_limbs(a, al);
//             bl = rand() % 8 + 1;
//             ll_rand_limbs(b, bl);
//             ol = rand() % 8 + 1;
//             ll_rand_limbs(odd, ol);
//         } while ((odd[0] & 1) == 0);

//         /* a, b should be less than modulus */
//         ll_div(a, NULL, NULL, NULL, a, odd, al, ol);
//         ll_div(b, NULL, NULL, NULL, b, odd, bl, ol);

//         /* A = aR mod N */
//         ll_to_mont(A, a, odd, )
//         fp256_to_mont(&A, &a, &mctx);
//         /* ta = a */
//         fp256_from_mont(&ta, &A, &mctx);
//         /* B = bR mod N */
//         fp256_to_mont(&B, &b, &mctx);
//         /* ta = a */
//         fp256_from_mont(&tb, &B, &mctx);
//         /* r = abR mod N */
//         fp256_mont_mul(&r, &A, &B, &mctx);
//         /* tr1 = Ab mod N = abR mod N */
//         fp256_mod_mul(&tr1, &A, &b, &mctx.N);
//         /* tr2 = aB mod N = abR mod N */
//         fp256_mod_mul(&tr2, &a, &B, &mctx.N);

//         /* check a = ta, b = tb */
//         if (fp256_cmp(&a, &ta) != 0 || fp256_cmp(&b, &tb) != 0) {
//             printf("fp256_from_mont %" PRId64 " failed\n", i + 1);
//             test_fp256_print_mont_ctx("mont ctx:\n", &mctx);
//             test_fp256_print_hex("a   = ", &a);
//             test_fp256_print_hex("A   = ", &A);
//             test_fp256_print_hex("ta  = ", &ta);
//             test_fp256_print_hex("b   = ", &b);
//             test_fp256_print_hex("B   = ", &B);
//             test_fp256_print_hex("tb  = ", &tb);
//             goto end;
//         }

//         /* check r = tr1 = tr2 */
//         if (fp256_cmp(&r, &tr1) != 0 || fp256_cmp(&r, &tr2) != 0) {
//             printf("fp256_mont_mul %" PRId64 " failed\n", i + 1);
//             test_fp256_print_mont_ctx("mont ctx:\n", &mctx);
//             test_fp256_print_hex("a   = ", &a);
//             test_fp256_print_hex("A   = ", &A);
//             test_fp256_print_hex("b   = ", &b);
//             test_fp256_print_hex("B   = ", &B);
//             test_fp256_print_hex("r   = ", &r);
//             test_fp256_print_hex("tr1 = ", &tr1);
//             test_fp256_print_hex("tr2 = ", &tr2);
//             goto end;
//         }
//     }

//     printf("fp256_mont_mul test passed \n");
//     td->ok = FP256_OK;
// end:
//     return NULL;
// }

int main(int argc, char **argv)
{
    TEST_ARGS args;

    if (fp256_init() != FP256_OK)
        return -1;

    RETURN_IF_ERROR(get_test_args(argc, argv, &args));
    test_rand_init();

    RETURN_IF_ERROR(run_test("ll_mont_mul", ll_mont_mul_test_vector, NULL, args.N, args.T));

    fp256_deinit();
    return 0;
}

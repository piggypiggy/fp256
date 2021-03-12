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

/* r = a * b */
typedef struct {
    char *r;
    char *a;
    char *b;
} MUL_TEST_VECTOR;

static MUL_TEST_VECTOR mul_test_vector[] = {
    /* 1 */
    {
        "6",
        "2",
        "3",
    },
    /* 2 */
    {
        "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe0000000000000000000000000000000000000000000000000000000000000001",
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
    },
    /* 3 */
    {
        "2f00013639064686fd62215527c4f7ff4d07120cca5f25fc91dfe970982681a8b9fbf8c3f4ab0b077",
        "f2b1",
        "3193cf5ad2c11eae4f321d48e4e322dc3e2630496b7ca5e023dd36788627ef2026e16abfa0fa7",
    },
    /* 4 */
    {
        "37f503ce0ef6f3e7ecd16e0d8e5740b02665d50d4e92df8628a49dbb7821adceb9c49dc341fb7fc8587a071f1a41b5c0a79b23d68d12b774631439db5c003e496a",
        "1540df91d567564604e2b62adb723e48dbf2f2f90803a4813160f1128561a2",
        "2a20298becdbdb5ba2ce3a433fce448dca5453bc2f86059a3e102a5941fcc2eebcea5",
    },
    /* 5 */
    {
        "3a52637bc5f96eeb4ed0125f0e69e9aca9e4f625561267a2dc57655f1a715891245ca122d31e476ee3a06db54a1370",
        "8",
        "74a4c6f78bf2ddd69da024be1cd3d35953c9ec4aac24cf45b8aecabe34e2b12248b94245a63c8eddc740db6a9426e",
    },
    /* 6 */
    {
        "7acfefbb30e2b89a961be371795592cb4eefecdad26219083d5b56214b8fb860494a4e93383d8d5303db8f03c7efaaa0438fa92fb56bae338fd6423011ae27673c40174f6a8c4df348134360c993fcdea4b715e9dd668a",
        "b07f24c5f789e32bf8d2c08cbf3d97844fddaebcee4d94de8f4c863ef67181f5ebb875fa39829afe82",
        "b22221c2c004add199de81d133eaf0b3fb56a99b8031437252196db25086b5b485999306c5846f6545fe9b447705",
    },
    /* 7 */
    {
        "161f3dd7ab9341c97068122b26b78812806ae3eec04f728a7a505634f5f231e08e7c81bbca97138977776680ede228c14dca85387c18f58d8b9955733ed57989349ad828e3c66691e38837f3a1320",
        "162d0231f85bb09434e0d60980fcf20af5a6bc6d2c93907c9d402f4e3811aefb36d343",
        "ff61127dd133d5f8481fa34fdd4f027539a89c6f39d2ddf7792f303b9f766b8d3ec276f13dbf057f9631e60",
    },
    /* 8 */
    {
        "2495154443eaeb78e93dd0c90b",
        "3ae65f6c4d26832cf049c415",
        "9f",
    },
    /* 9 */
    {
        "15991951cc3df78fd45ce158d10c3a09e52d8d1889ed387f86dc",
        "30892b0732eaa6",
        "71eafce432fb5664b2fb8169a4a99d4e72a31a",
    },
    /* 10 */
    {
        "962a759571b054a6f3ae117c64dc9be0aac1a47f6db23ace911f9aef5fabb3ad522e428558483701acf9f6",
        "4b153acab8d82a5379d708be326e4df05560d23fb6d91d67488fcd77afd5d9d6a9172142ac241b80d67cfb",
        "2",
    },
    /* 11 */
    {
        "48d50a678ca628a55b07f75c1c33698b858f5fe3",
        "7",
        "a6793c5a660e13c31936c7adf750f13ee823245",
    },
    /* 12 */
    {
        "3a8ddcbac79c4148fb0d99ab2fdd998c2a59493a87626b85573e6efae8569fee02681c08",
        "303a6345dbbd2520fbfde86047a8",
        "136cfdb3f835213b0828e0d951c21fa190eb0f79728dd",
    },
    /* 13 */
    {
        "69709e2e59491a016e3e",
        "5173e2e5b6",
        "14b63e2460d",
    },
    /* 14 */
    {
        "e256d75f9f67186f08204c26d8043ef2b9ffacacd563f58ef05dcc3105e9dd756f4ca417fef0",
        "3469cd30",
        "4517fc469b050560b90d140eeba6e7425d28b1849e52d73e2a7c89dcacd966cd2aef5",
    },
    /* 15 */
    {
        "1751cc9b211856c35c6ba3708ad3af339618e9ea9244e37d835be52127b9449b9169ee278696f71fb400761102b665134767fb92430a7a78c304682a0c37dc22e1218eba6252e4ecad08a27fe2d52e93c0737fb7b3417b628",
        "3fd42211e52ec6b30dde7908d98e364a35f115e150ba1ee328710ac4264401c9827ee4a3795aea259230458",
        "5d874da7dfacd4f7e11cba52b23c12e7de5c41939631de19bc53551645c2bb1df4d0ee7ffedb27938987141fef",
    },
    /* 16 */
    {
        "187e2e3b2db0c62027ee4917aa2eae447d5f0bfdedfb24844ec6f21a94ac",
        "f87af9a6eacbc57d",
        "193bee83e2ee436e71f3c1ef26c2d836e985c2d9d71c",
    },
    /* 17 */
    {
        "6ad54d9741c90547883779f7dfef0f7d9f0d4b44e13d41e",
        "7f66c8c66756fbbf6d9d11d0ffd889a518f93039",
        "d6ab90e",
    },
    /* 18 */
    {
        "e1badb3ec630fa0805f28bedf2a187e9f5bbecce2933c12256d1060a5f22fa054b569b84dc83c9e574aad0708eb1bcd272dc299faf1f5c84e3d09cdea5747f80449d8db7f131ad6ce2f961dc08a04d1c93581757ff339cdd3898ac165c580",
        "ea95a35b26ced820dcd1538751aa8aa75b420b85d418c71391e0c571647c3fbab0b26c58ca96a6043870510a7132e2",
        "f6564806c9b3367991e55c34caf1b15895062641384f966693db49eaf34a1cd2af95b905fbaad4f327095908ac5aec0",
    },
    /* 19 */
    {
        "38a5cfba7cf090869ef121a22f9bb1c58bd0168cc31dc0cff953d1529be",
        "47404d633ed97b212408000b5bf4ed",
        "cb87e3fb5dc4a107e6ca163db46f6",
    },
    /* 20 */
    {
        "63978aa05e0d5d801833422fc7d11d8cdfe5aa1009c7a147ad",
        "1",
        "63978aa05e0d5d801833422fc7d11d8cdfe5aa1009c7a147ad",
    },
    /* 21 */
    {
        "0",
        "888888888888888888888888888888888888888888888888888888888888888",
        "0",
    },
    /* 22 */
    {
        "9f0137552e935db07cb618c8a3e132a4667c96be264008d39d6e06d168b5695105826cf3812f66fa63a64ef75d6bba50c4bfd6af25f4df3cfac6d8632fb67d513afbae9aaaf6",
        "89bac664481a10ebeeceb3de5d1d0ce42c49d6e7a42515f9089bea2756b4c5c538842d336bf6de9673db72f4c5bf32",
        "1278b613801e101935e89209b46da1c17cdbd2126575a33",
    },
    /* 23 */
    {
        "fb0a884509ad18639f2",
        "2",
        "7d85442284d68c31cf9",
    },
    /* 24 */
    {
        "14b0aa742a6e1eb44ba27f78836677d7e0a54294e2deb79d74f395f848bf50e86abd0776d2684ae22b8db1dc7700",
        "168d4f1cea60486e0b65a368da024c7ae980ee983506b7ad0ae6a6017edae91fb1d02a61e0",
        "eadd5c844ec71a00c8",
    },
    /* 25 */
    {
        "ea9b687c6c7ec3bef800c3c37",
        "f580d36286777",
        "f4a351518141",
    },
    /* 26 */
    {
        "e176dd4aae4390324114f2d7feabac254e9de19368e0b12c1844509e15d1a6beef3315211a16bb5ead7b3da3b4de9e9adbadb1b9b71b7ed2b885a234a8864f905ff1b2e17385b2f19084c62349e8b81b84f8dbf36033d9a6680d8cc7",
        "7d884e6a1beb37e0d932cf12e1b6b7bb111921a10e59f8e86f2ca55c43967aa8595f7286951ed0f11b317bbd96aac9f",
        "1cbcada7f6c41522ffb0800388b1fe131d86a7f21efc9aa7286659bc72f16c5b1747e642b357fa8f6f6dfa86d9",
    },
    /* 27 */
    {
        "8e4d88bad70fdf1ff20c20dee16bbcdc1d611adbe058519abf95b5b4e8df9aca9bb8e84467598e740e79af8",
        "153245daa98799027a218bc33370b73eabeebdccb766",
        "6b6aaf84c5b46dfa1e2e23b1f389887775026f310e94",
    },
    /* 28 */
    {
        "47472a6d4bd302b42a8b57de89747ee2110b5de51b0539fffe3a7b2ee",
        "2906cf2d78ae8d5d377980475d3f407d53a7d",
        "1bcc3a7f4780ca2a5e406",
    },
    /* 29 */
    {
        "2ab00f9d98893460d8c85ab6a95389d3b82f",
        "2e2a6a865",
        "ecb6c3839dc4d7be84c423b5303",
    },
    /* 30 */
    {
        "28d9456f89971a96e258c54a256918bb8dac83e5d0d27d2cd43409f8789ac0343a008e96c27d65163738be1748b1e6126c4",
        "3de3e25d1165176e909b546f619a531fdcb536242fe7f5f09e4",
        "a8f70852751a87580eadd963329036967a81fb3454a4b2f9",
    },
    /* 31 */
    {
        "",
        "12345678abcdeffedcba9876543210",
        "",
    },
    /* 32 */
    {
        "",
        "",
        "12345678abcdeffedcba987654321012345678abcdeffedcba9876543210",
    }
};

int ll_mul_test_vector(void)
{
    unsigned int i;
    u64 tr[12], r[12], a[6], b[6];
    size_t trl, rl, al, bl, max;

    for (i = 0; i < sizeof(mul_test_vector) / sizeof(MUL_TEST_VECTOR); i++) {
        // /* clear */
        // ll_set_zero(tr, 12);
        // ll_set_zero(r, 12);
        // ll_set_zero(a, 6);
        // ll_set_zero(b, 6);

        ll_from_hex(r, &rl, (u8*)mul_test_vector[i].r, strlen(mul_test_vector[i].r));
        ll_from_hex(a, &al, (u8*)mul_test_vector[i].a, strlen(mul_test_vector[i].a));
        ll_from_hex(b, &bl, (u8*)mul_test_vector[i].b, strlen(mul_test_vector[i].b));

        /* tr = a * b */
        ll_mul(tr, a, b, al, bl);
        trl = ll_num_limbs(tr, al + bl);
        if (ll_cmp_limbs(tr, r, trl, rl) != 0) {
            max = (trl > rl ? trl : rl);
            printf("ll_mul_test_vector %d failed\n", i + 1);
            test_print_hex("r = ", tr, max);
            test_print_hex("a = ", a, max);
            test_print_hex("b = ", b, max);
            printf("a * b should be :\n");
            test_print_hex("r = ", r, max);
            return FP256_ERR;
        }
    }

    printf("ll_mul_test_vector passed\n");
    return FP256_OK;
}

int main(int argc, char **argv)
{
    TEST_ARGS args;

    if (fp256_init() != FP256_OK)
        return -1;

    RETURN_IF_ERROR(get_test_args(argc, argv, &args));
    test_rand_init();

    RETURN_IF_ERROR(run_test("ll_mul", ll_mul_test_vector, NULL, args.N, args.T));

    fp256_deinit();
    return 0;
}

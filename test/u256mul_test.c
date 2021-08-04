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
        "e54ff64e6fb4513471fb",
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
    /* 17 */
    {
        "ffffffffffff222222222222ffffff8888900000003456722255551a2b42c59ccca06d3a070dddddddddddd000000eeeee0000000666666660000000000002",
        "ffffffffffff222222222222ffffffffffffffffffffffffffffffffffffffe",
        "ffffffffffffffffffffffffffffff88888fffffffccccccccfffffffffffff",
    },
    /* 18 */
    {
        "19c1c84ec46a860cc1e26a2cb20210b797b3b37c9b4fa08c3d6a265a06ac982f34bfc6a207aba2510",
        "7ee5d0cff6c4cb21956da88556e955df827fe2b0",
        "33f61dff7ec6c15c19321f7bb46c5d9765c67df73",
    },
    /* 19 */
    {
        "a9ab6f985a8961183c9a85b7734bf993e2b0338c348588118c87cd3102172bfe13b13601042b02b8cee8d0d3a29be0f1d854599a5eb73",
        "ad82c83dd6d49a32b6ecd0dbb566a8f340f9dcd418c6f9b7a4b5b933",
        "fa5528c15786ca767c1086e79c3e4b287a1593bef18fa083d84c1",
    },
    /* 20 */
    {
        "a47c2209e5ea9d08493d437286c2620d101a2c280b45aa0da3559eb10a2b968a9121e904e84cc6ce8c3022b327e488",
        "1537f95930323ffa0909f69b2fe68c9f5e09eb23ddf1bf739af5f36c3a56734",
        "7c07caea74ad192b0d1aa6de7632b3aa",
    },
    /* 21 */
    {
        "3a8aab462a76395c8ba3f36aac87a583667212fdbf8f5cd53cfaab7bb7cd56c3eafa678858e4194e594ce8425e2aa840eab692390f6c4304c0",
        "163a34bf1910937359d47cdf1dc65436b752595fb3f8b2489dc78",
        "2a23e76b1cb14c4c330e7e24962af0387e3391c6b0a6cc95bbc33f9457a9e8",
    },
    /* 22 */
    {
        "10f40eb385c5ada88cbe52beb173ecf4063e9990d5957a272983f03e36b49155fce2b66f87f0593e331b70",
        "1b7699f9dbc8294ebd3cc5d3b3ce0176a30bc349b64ed0",
        "9e07ffa774fc43a0bb6f6de6f0bd00ab735620b3",
    },
    /* 23 */
    {
        "56f7a170bccb26abb1472c82d41dc786b68415e93e9e32c79d2d914607c2a8f3c3155990dd1f61fc8badbbe3b9d0074517fb9c124855f246f5",
        "f2dee6a288efe657e17e1761d80c9de25bd5a887704864a1a32f5a67",
        "5bab2cddfa51e39cd63919ef8421c7a59b0ae9c4e6229011ea037eb243",
    },
    /* 24 */
    {
        "10b3ac70a2434bc21774c654a57bf0938713a526eae837b691cb65dfa27828eb32a218158e711767c4a1f0cc",
        "de4f2c6763c3e68d68c4a8c6369be9ed",
        "133ba5b471b4be459687f6d146096146077846f69f62812bf132ea7c",
    },
    /* 25 */
    {
        "31570edf83f9d0df0ff703e3cbc847716c5a0848f707cbf77c6ead0d386834ecba6334b1433def53e4574c93d17c10972e6e6beee5784ad1d810",
        "7159a41ee05607ab883ece715096ecd5ae0e6ad2e0dbeda3ea1e6e5f30",
        "6f6f187d7c9496e742007fb490cebba2bba6a0e35897a4b50a3b4087bb",
    },
    /* 26 */
    {
        "1ed90186946115be1a558ea0948a9bcdd5fdda8668bfef0008a47cf598fcb85f75caaee872b08006bfb2b73181da92c29e139bfff27fb1875deb81098",
        "12c08dee5d10f85ebb5bee934af63a659481406b859481e5eb8cf2a6ecb66558",
        "1a52000a1a9c164f7eca161fcef09b3786c09a45dfa30ee2a793ea5039",
    },
    /* 27 */
    {
        "165ec7def1310085a4d91ad8b28245c0ffe9808637929618b8e4258f29ca39aee14719f5b47e22042064b95d1d53b0de522345fef6824",
        "c646c134120c1ecb87d342a83ae334f07405482240d81fdb54d",
        "1ce2005b4343d8bb120424d23303681f8395adc845f5fff05f1feca6b4",
    },
    /* 28 */
    {
        "a6c2d755da20a1399c8b60bc6b0f5b65d46939cf8df8c38aa0c91b64024c23d37700e0b09c37357a164a9d860",
        "19d2f2b2da153c627df96abcbcabdd31ceb404f25fb8a9f4b79a65c",
        "675251b1a81be1ff52c2bbf238d90fb8da8",
    },
    /* 29 */
    {
        "14e89d9b6e72cf301eb19ddae2e8573c661e7aa6c5ac8bab5488118890d92665aeee15f8c",
        "127b79a13724d43cf3b500f3d6b13288189b504",
        "1219b76438d5cb870d5fb815dd29ca16823",
    },
    /* 30 */
    {
        "7c600f2d2f1ecd99823329d6f82cee3d3026f58eabd496a348f47bce62aafd00624c385868b44134e7e4cd94518101187e965e0e99",
        "83f3e038a42711e677dae7b20e2a8347b9064411f9",
        "f14c94ca5c8f8560a3587e831dddcebf193b2ccb2fed437d660583f3e39509a1",
    },
};

int ll_u256_mul_test_vector(void)
{
    unsigned int i;
    u64 tr[8], r[8], a[4], b[4];
    size_t rl, al, bl;

    for (i = 0; i < sizeof(u256_mul_test_vector) / sizeof(U256_MUL_TEST_VECTOR); i++) {
        /* clear r, a, b first */
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

        /* r = b * a */
        ll_u256_mul(tr, b, a);
        if (ll_cmp_limbs(tr, r, 8, 8) != 0) {
            printf("ll_u256_mul_test_vector %d failed\n", i + 1);
            test_print_hex("r = ", tr, 8);
            test_print_hex("a = ", a, 4);
            test_print_hex("b = ", b, 4);
            printf("b * a should be :\n");
            test_print_hex("r = ", r, 8);
            return FP256_ERR;
        }

        /* tr = lower 256 bits of a*b */
        ll_u256_mullo(tr, a, b);
        if (ll_cmp_limbs(tr, r, 4, 4) != 0) {
            printf("ll_u256_mullo_test_vector %d failed\n", i + 1);
            test_print_hex("r = ", tr, 4);
            test_print_hex("a = ", a, 4);
            test_print_hex("b = ", b, 4);
            printf("a * b should be :\n");
            test_print_hex("r = ", r, 4);
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

    RETURN_IF_ERROR(get_test_args(argc, argv, &args));
    test_rand_init();

    RETURN_IF_ERROR(run_test("ll_u256_mul", ll_u256_mul_test_vector, NULL, args.N, args.T));

    fp256_deinit();
    return 0;
}

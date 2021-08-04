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

/* r = a^2 * 2^{-256} mod N */
typedef struct {
    char *r;
    char *a;
    char *N;
} FP256_MONTSQR_TEST_VECTOR;

static FP256_MONTSQR_TEST_VECTOR montsqr_test_vector[] = {
    /* 1 */
    {
        "1",
        "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe",
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
    },
    /* 2 */
    {
        "29082b62c6f58c4aa57d477dd7c40c6bc56ce9ab182420e40",
        "2595e14b9f48eb8d7",
        "3268de6e20a15cf8d036718837663491d065d304ab3fdd79f",
    },
    /* 3 */
    {
        "b0e01baafbfa3ce992578d1f274de4f6cc2d744e0064ad115b3ad",
        "7a9e5f61985301a3128a0e48dca",
        "1396d6f1099aab79c73289be595905ad172d8ed0396a3065be62cc3",
    },
    /* 4 */
    {
        "fb100fb3182d1062003ec4e4b5bdb70058b452bffc1c2e65bab",
        "394699bb35c5f4e32376025a243cbf9d",
        "3d7690fe87d47c1dc3124b8ff3a5b3cfab1a6268812a67a4c7f1",
    },
    /* 5 */
    {
        "148f82f74749b7859f40c1c0b05abf4cc4bc988c4adf1b459ad455297",
        "16d4013d7685d8bdac23972f1488044b3d87f74",
        "1876fd9051f39024a10c53bce68a57cb6191b379f98beb0c973c01c5d",
    },
    /* 6 */
    {
        "5c50b70f167c5ca50a69c0dacf754cbd8dada9f7a37f28ecfc0a3e017d69",
        "1",
        "1cfb8f4f2a2104de194863e6f96a094fe66f5632f2cb550b4342d674561c5",
    },
    /* 7 */
    {
        "6ca20c2a42d840ae20324825c85f49303aa1311d92209f232c9e654538e7633",
        "16f786d9b70a7593e130125003da29d1b0e19a394df483aa3af73b9db3f4b94",
        "fdeddf44e238aec2a481f036627a0e3b7687a111c9e7bde437db011ad415213",
    },
    /* 8 */
    {
        "1f5863ec3b67d08a59774ff51160a9315000b3472658d6a17e",
        "281a1c429979e8a5fabbdee9ed84167b75a11c5c3020f3601b",
        "34c3f03941bda741f9c8833ddd012f96d8f3aa2391250789d5",
    },
    /* 9 */
    {
        "be8a2b9e157006d668f201fc4812fa85e21b01d053efff78fcb91d68851da7",
        "fdccac6dad21c3d69e8cfd790b9a9f9f4f7062ff2f908e11834005c",
        "17ac998477136ec99b42b35d930bc60adc35d19c5d6e944380583f73b2b35b7",
    },
    /* 10 */
    {
        "6c5c4a92adeed96a41362031cfde7f40458064bc24d13b6ae1e23e60f268997",
        "2c4ca344647557190a414d062a3f74",
        "71374d0d4bd53ac9baa9518fd8d05d6d134d6310a321b5f12a50fdd5be90a457",
    },
    /* 11 */
    {
        "76a18445d5814b53d51ca1d1bb837eb3ea57c0ab9a22e8fb6cd77d9e0c7",
        "61484a0bc9cc9c976b83614be8a7ca21d8a50f732fc129ffe387d38125a",
        "e85615144b0e54a826ec0ebdc41359e5c06e719d660cdb4056e78c01d79",
    },
    /* 12 */
    {
        "574d52ff913b52d4550462ee7fd0ae6eb55f6c9169c553d8efc17e09d46d451",
        "ad2eac3192caab92b21a9f6",
        "2daf4ed7acc717aea8c36efe7c04750e236424ed36b0542c357d03673e953027",
    },
    /* 13 */
    {
        "58d2cb5e2048d19d6b44742a315c7f83e5a43adca7ab396c01bbbf96e569495",
        "ac",
        "88fe44a2ed84f363459d02fcf3cfe4328d88608e6d1f6621d925056cb94f535",
    },
    /* 14 */
    {
        "47c884c2fb4a064b12bbc4e4dc90c258260d110231c16ea0a10d8b4bbed45a4",
        "afcfb",
        "683c2cd777d05181f815281070b78ec9a61c2a8e75e891c26a9c05817e6ce7b",
    },
    /* 15 */
    {
        "1b9d62dcbee8c4ec439645efd61043997bf8e388ec8104bb9",
        "11b4b9215087",
        "b325b8754246c2f6a411ad2ab1057fca7e725ff7a4544b52d",
    },
    /* 16 */
    {
        "3de6b1bce21c22638926dc82d1ddfd6cb1444d954ba1b03a559143",
        "29d924c6f871bd76939daadc48df90a1b01578e09ac8be54c8e0aa4",
        "2b8c582cd46f591c8aca06b7a7394274260cd4f944ff2b40d7ca321",
    },
    /* 17 */
    {
        "286ba72d9c06c9b8175100d8b439eed1012ece391ec832db565f423255d",
        "5a6e859fbbf8416fd6e76c9bedc6d767950970e9",
        "10bbb2cf7f02856b1330ce5fc57167c4a214b9712cdb49398583490dcb83",
    },
    /* 18 */
    {
        "84836efc08cb889f595ad117dbdc8e53aff90f5b4af4bce5e177dd",
        "84613b6aaeaafe468baa6fec7935f03630e0526f17db8716dffe9a",
        "7e76fa39efdefe49a9fb40b3ffa7a6debed2d9defc007066a15b313",
    },
    /* 19 */
    {
        "f6089113e3f082db0ad58c7c2bc23c3f56593301c6dcab4abfea64cc0252c",
        "2",
        "c4cf6a958d5a8cc71e1c0847a95fb57e7468fdc2dc1a90055303271a631ef3",
    },
    /* 20 */
    {
        "aeba95f0ba328ccdc8b83f8fdaa222c54e21de1a552e69190ef0b68427f3d5e8",
        "9bac1dbf577e737b76f220035cf071ead1f357a7fd9db31702c76123808695",
        "c6b309ddec4e3e514bc7b2cc24ae5b23c40bef092a6e141c56d9de07b5e42a5d",
    },
};

int fp256_mont_sqr_test_vector(void)
{
    unsigned int i;
    fp256 tr, r, a, N;
    mont_ctx mctx;

    for (i = 0; i < sizeof(montsqr_test_vector) / sizeof(FP256_MONTSQR_TEST_VECTOR); i++) {
        fp256_from_hex(&r, (u8*)montsqr_test_vector[i].r, strlen(montsqr_test_vector[i].r));
        fp256_from_hex(&a, (u8*)montsqr_test_vector[i].a, strlen(montsqr_test_vector[i].a));
        fp256_from_hex(&N, (u8*)montsqr_test_vector[i].N, strlen(montsqr_test_vector[i].N));
        fp256_mont_ctx_init(&mctx, 4, &N);

        fp256_mont_sqr(&tr, &a, &mctx);
        if (fp256_cmp(&tr, &r) != 0) {
            printf("fp256_mont_sqr_test_vector %d failed\n", i + 1);
            test_fp256_print_mont_ctx("mont ctx:\n", &mctx);
            test_fp256_print_hex("r = ", &tr);
            test_fp256_print_hex("a = ", &a);
            printf("r should be :\n");
            test_fp256_print_hex("r = ", &r);
            return FP256_ERR;
        }
    }

    printf("fp256_mont_sqr_test_vector passed\n");
    return FP256_OK;
}

void* fp256_mont_sqr_test(void *p)
{
    int64_t i, N;
    fp256 r, tr, a, ta, A, odd, mod, k0;
    size_t al, ol, w;
    mont_ctx mctx;
    TEST_THREAD_DATA *td;

    td = (TEST_THREAD_DATA*)p;
    N = td->N;

    /* mod = 2^64 */
    fp256_clear_set_bit(&mod, 64);
    for (i = 0; i < N; i++) {
        /* random odd modulo and corresponding R */
        do {
            al = rand() % 4 + 1;
            fp256_rand_limbs(&a, al);
            ol = 4;
            fp256_rand_limbs(&odd, ol);
            w = ol;
        } while (fp256_is_odd(&odd) != 1);
        fp256_mont_ctx_init(&mctx, w, &odd);
        /* a should be less than modulo */
        fp256_mod(&a, &a, &odd);

        /* check k0 * N = -1 mod 2^64 */
        fp256_set_limb(&k0, mctx.k0);
        /* k0 * N mod 2^64 */
        fp256_mod_mul(&r, &k0, &mctx.N, &mod);
        fp256_mod_neg(&r, &r, &mod);
        if (fp256_is_one(&r) != 1) {
            printf("fp256_mont_sqr check k0 %" PRId64 " failed\n", i + 1);
            test_fp256_print_mont_ctx("mont ctx:\n", &mctx);
            test_fp256_print_hex("r  = ", &r);
            goto end;
        }

        /* A = aR mod N */
        fp256_to_mont(&A, &a, &mctx);
        /* ta = a */
        fp256_from_mont(&ta, &A, &mctx);
        /* r = aaR mod N */
        fp256_mont_sqr(&r, &A, &mctx);
        /* tr = Aa mod N = aaR mod N */
        fp256_mod_mul(&tr, &A, &a, &mctx.N);

        /* check a = ta */
        if (fp256_cmp(&a, &ta) != 0) {
            printf("fp256_from_mont %" PRId64 " failed\n", i + 1);
            test_fp256_print_mont_ctx("mont ctx:\n", &mctx);
            test_fp256_print_hex("a   = ", &a);
            test_fp256_print_hex("A   = ", &A);
            test_fp256_print_hex("ta  = ", &ta);
            goto end;
        }

        /* check r = tr */
        if (fp256_cmp(&r, &tr) != 0) {
            printf("fp256_mont_sqr %" PRId64 " failed\n", i + 1);
            test_fp256_print_mont_ctx("mont ctx:\n", &mctx);
            test_fp256_print_hex("a   = ", &a);
            test_fp256_print_hex("A   = ", &A);
            test_fp256_print_hex("r   = ", &r);
            test_fp256_print_hex("tr  = ", &tr);
            goto end;
        }
    }

    printf("fp256_mont_sqr test passed \n");
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

    RETURN_IF_ERROR(run_test("fp256_mont_sqr", fp256_mont_sqr_test_vector, fp256_mont_sqr_test, args.N, args.T));

    fp256_deinit();
    return 0;
}

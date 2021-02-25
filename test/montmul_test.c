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

/* r = a * b * 2^{-256} mod N */
typedef struct {
    char *r;
    char *a;
    char *b;
    char *N;
} FP256_MONTMUL_TEST_VECTOR;

static FP256_MONTMUL_TEST_VECTOR montmul_test_vector[] = {
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
        "7b5da5c88e1c91a5b0a40c3f4709b0426ba8cc07c4a889681",
        "80",
        "2ca69a50bb237917025e9e07e7cfe5d37e6f22c4325b65a2",
        "341f7a4877611b647de8ec1b6bb2a0b37434aa8d4da25e28e5",
    },
    /* 4 */
    {
        "11d1a2a18fe75c38db1a8f0f414b4c4d6b7935c56580c5a0d31e16",
        "2382997362c6e53bdaac15191dca7288f7",
        "19db2091ad4a65cf0071c8e3c728e9273e371a5acd360f1a6093a7",
        "31e1790bc7817debfccdf8868501130ff788a38e9b680faec5c1b3",
    },
    /* 5 */
    {
        "6fc7471d58e78e2d6b0053bcacb7fd99f0f4e7f5addeb61a539a",
        "13a10ff21caf40d141c8d0756fcc51b07b0f4fecd9c6bef4805b",
        "5c03b70671c25ec783df3d0688a3ae92febb416f50773146a5a8",
        "76e48fa0b8b4ada1a38b68888ca48183dd720ea6dd724f352a8d",
    },
    /* 6 */
    {
        "82e5a19c31a1c6c0b3056b8ea190c259cfe81b48aba977cc7d38854a145",
        "232ddbce3d9f733f0a9402bd5dc62dde3",
        "8b740e1b910f20c6a09aa84f589",
        "11b040f019265107be9999f2908673b742b210f9e763335e3f7d74cffe0b",
    },
    /* 7 */
    {
        "c4340462bf428043db05c86b0e039c71bb1fa53e309f4e9282eb24d",
        "349dc7ddf00a2e",
        "298fcc4",
        "baf3ddaf37b16e05174b46adde23dade6aa86cadd9e59e573b008c3b",
    },
    /* 8 */
    {
        "e3eda28022a2b2c82a5fae389bff6667a49fd5d435795622ebc61e11522a8",
        "b4e6a239cbb0313070b608d2",
        "8ebba643c96d80a302bd9f24",
        "1b5778828bf550dec10ddefb603e7ad59755f89d2a0f31ad2243f9a97623bd",
    },
    /* 9 */
    {
        "1cfff29a5fcdf58e7cd8792084f167a78c33822ad462c8fd44dde715789cbdb9",
        "1d9920e5f3a7231dc62149ba1bf6a9bdfa7d0ba3483e8ee5d288a87c46b",
        "6a4a58f3c7846652803abca4d567e6630b0a1d4d05d044cedef9a5374fbd208f",
        "76307b36a8b9d9150e36273ea9e6c8b9c20a11aaf511f13677817915326d042b",
    },
    /* 10 */
    {
        "3c55e05008084201bc1dd6602a2ec1a8dfc3b1345b58f9e2d",
        "8",
        "bd4ea",
        "e88bc8dc7592d5b8435a5ebda99d07be3ae3898541ab7d39b",
    },
    /* 11 */
    {
        "1a4ef905b3659ce4f027f0ad5296d20deb5435ea95c40707fea6d2b98a23f4",
        "25f11234abc526e9e19cda4add73ea5bc2e4dfdb4f08c08b72df71edd946",
        "30f3c3aa49f7d3d4a6b72e7ed016e573294cc31d61530ac82c3682c89afb",
        "2d5a422a12f76cb523fca65deffae593902b91a3359b47f33032e25d568685",
    },
    /* 12 */
    {
        "32a42792f2c62e1ead3f358ab9b69825c6814c9929327c",
        "b75589090",
        "3c4eb01c66560a06b28",
        "190cfe443ff83b01769af4209fc9c4e92f339a09cc345785",
    },
    /* 13 */
    {
        "c1e23498fb8e150a86b6e00433485309ffaa4c0b2c97046867",
        "86e2a5",
        "e4b765c52",
        "15d566d687714281b8b6653acf2b71532c460724a1a5973170d",
    },
    /* 14 */
    {
        "4cee2d972ead25d98835c0f1cfc49f868ea78eeb9cd3c8f4f2260a8be6a80",
        "628015c49c015d034e36c2ec1f5cad345c845839",
        "74d1ab71f2ed281aca56aac4faecae883cfddd87ef43",
        "7bc18217573c2acd844131e00bffa505fc5d68bc31af0d429880991780f7b",
    },
    /* 15 */
    {
        "520dce5519e5612579a99aa52e690b6b33a80e25f1fb8643aa27d65a8",
        "1be3a9d0bf",
        "463648d982abf61643768c",
        "14e8bf85d5008d9bf416c95791d6650d13c86cce2b1d4aae4cc930ea7b",
    },
    /* 16 */
    {
        "14950f497a944403e79127865f8fb61a40648aedb2cd95d7e",
        "3db6cd1c99822b71c6bb1",
        "95aa7bb8b6b8f57d1e96a90ec4632b92b956e9",
        "1a21ee38522d6323fb559c5188c899a9407828160005efc0d",
    },
    /* 17 */
    {
        "c28079ff5ebe83b981e2d7cfcf695873c6545acf2ce204b5592459b708",
        "1",
        "39ff1ce6db",
        "175e50c3312180f78ca4b21873b834c880f7a40fffe1ec67910a9830919",
    },
    /* 18 */
    {
        "2a10add9bee6b7a29682e8538c6e5bcc160107f7171b64dd7039764fa003c1",
        "6fd82f42b762dc5c853afa69d4af62c5f849e7f6dfd7d1cf290",
        "21",
        "1bceee6177043fa63687ebc14a4c905e4ded0f038c316b2d729e5cea5a23b9a3",
    },
    /* 19 */
    {
        "248b95077a84fac4dda4d2fd903813cd92827eb78cbe1834297c1092badf4",
        "6b34ed9cb277f",
        "3b2e25b008",
        "8076f6de39a3b190aa63f35781440133a3a7d5566b3d7975a4f11aaf53f1b",
    },
    /* 20 */
    {
        "1673557d6cd013e5634b35c2795f503be32ab7c4c261998bd18b58b7c",
        "1ea6611183d3cd8e29007f81464fb0f72240ac39d9bf372bfc5bc7fa8",
        "2209f9ea1eb112e3e660cd8c9198c03b8b3e153f106ff7ac527ab2040",
        "2d851c5cc072055b63ca6771e8fe51d945edbdee4551137b85b2084a1",
    },
    /* 21 */
    {
        "36594bbbfe35c74096962ae2c5c977a90b592f9f2d278303f7e8a78ae5",
        "3ce35ff6152c1ac1a195c605b2eee02f75273b5f231",
        "6e1401840f6264f6aee9a6432bc434d9c437b26c1eb0e9d30a",
        "53b29a0467d1b5bbdd10b1ebaeb67ae2eca733a49aea809938c87d2b25f",
    },
    /* 22 */
    {
        "4dd849c9502a0fa6f0d92db9130b5759f080fa54532372f2996478cbedef",
        "6f670037f2160d854ba135fdddd51d",
        "77a8890197395bead8d655f62d45aae93b9",
        "a31828c498eff7b7155d73b9d6bf8cb57558dc923e23eb7ea00b4ddadf85",
    },
    /* 23 */
    {
        "266bbb09dea88701cc7037a8c47f7951d36481e7deccf053b2cee22",
        "b",
        "35433",
        "2894ec4964ed2e9559dca04cfc7622f56b3955f1edecffb25d3aa7d",
    },
};

int fp256_mont_mul_test_vector(void)
{
    unsigned int i;
    fp256 tr, r, a, b, N;
    mont_ctx mctx;

    for (i = 0; i < sizeof(montmul_test_vector) / sizeof(FP256_MONTMUL_TEST_VECTOR); i++) {
        fp256_from_hex(&r, (u8*)montmul_test_vector[i].r, strlen(montmul_test_vector[i].r));
        fp256_from_hex(&a, (u8*)montmul_test_vector[i].a, strlen(montmul_test_vector[i].a));
        fp256_from_hex(&b, (u8*)montmul_test_vector[i].b, strlen(montmul_test_vector[i].b));
        fp256_from_hex(&N, (u8*)montmul_test_vector[i].N, strlen(montmul_test_vector[i].N));
        fp256_mont_ctx_init(&mctx, 4, &N);

        fp256_mont_mul(&tr, &a, &b, &mctx);
        if (fp256_cmp(&tr, &r) != 0) {
            printf("fp256_mont_mul_test_vector %d failed\n", i + 1);
            test_fp256_print_mont_ctx("mont ctx:\n", &mctx);
            test_fp256_print_hex("r = ", &tr);
            test_fp256_print_hex("a = ", &a);
            test_fp256_print_hex("b = ", &b);
            printf("r should be :\n");
            test_fp256_print_hex("r = ", &r);
            return FP256_ERR;
        }
    }

    printf("fp256_mont_mul_test_vector passed\n");
    return FP256_OK;
}

void* fp256_mont_mul_test(void *p)
{
    int64_t i, N;
    fp256 r, tr1, tr2, a, b, ta, tb, A, B, odd, mod, k0;
    size_t al, bl, ol, w;
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
            bl = rand() % 4 + 1;
            fp256_rand_limbs(&b, bl);
            ol = 4;
            fp256_rand_limbs(&odd, ol);
            w = ol;
        } while (fp256_is_odd(&odd) != 1);
        fp256_mont_ctx_init(&mctx, w, &odd);
        /* a, b should be less than modulo */
        fp256_mod(&a, &a, &odd);
        fp256_mod(&b, &b, &odd);

        /* check k0 * N = -1 mod 2^64 */
        fp256_set_limb(&k0, mctx.k0);
        /* k0 * N mod 2^64 */
        fp256_mod_mul(&r, &k0, &mctx.N, &mod);
        fp256_mod_neg(&r, &r, &mod);
        if (fp256_is_one(&r) != 1) {
            printf("fp256_mont_mul check k0 %" PRId64 " failed\n", i + 1);
            test_fp256_print_mont_ctx("mont ctx:\n", &mctx);
            test_fp256_print_hex("r  = ", &r);
            goto end;
        }

        /* A = aR mod N */
        fp256_to_mont(&A, &a, &mctx);
        /* ta = a */
        fp256_from_mont(&ta, &A, &mctx);
        /* B = bR mod N */
        fp256_to_mont(&B, &b, &mctx);
        /* ta = a */
        fp256_from_mont(&tb, &B, &mctx);
        /* r = abR mod N */
        fp256_mont_mul(&r, &A, &B, &mctx);
        /* tr1 = Ab mod N = abR mod N */
        fp256_mod_mul(&tr1, &A, &b, &mctx.N);
        /* tr2 = aB mod N = abR mod N */
        fp256_mod_mul(&tr2, &a, &B, &mctx.N);

        /* check a = ta, b = tb */
        if (fp256_cmp(&a, &ta) != 0 || fp256_cmp(&b, &tb) != 0) {
            printf("fp256_from_mont %" PRId64 " failed\n", i + 1);
            test_fp256_print_mont_ctx("mont ctx:\n", &mctx);
            test_fp256_print_hex("a   = ", &a);
            test_fp256_print_hex("A   = ", &A);
            test_fp256_print_hex("ta  = ", &ta);
            test_fp256_print_hex("b   = ", &b);
            test_fp256_print_hex("B   = ", &B);
            test_fp256_print_hex("tb  = ", &tb);
            goto end;
        }

        /* check r = tr1 = tr2 */
        if (fp256_cmp(&r, &tr1) != 0 || fp256_cmp(&r, &tr2) != 0) {
            printf("fp256_mont_mul %" PRId64 " failed\n", i + 1);
            test_fp256_print_mont_ctx("mont ctx:\n", &mctx);
            test_fp256_print_hex("a   = ", &a);
            test_fp256_print_hex("A   = ", &A);
            test_fp256_print_hex("b   = ", &b);
            test_fp256_print_hex("B   = ", &B);
            test_fp256_print_hex("r   = ", &r);
            test_fp256_print_hex("tr1 = ", &tr1);
            test_fp256_print_hex("tr2 = ", &tr2);
            goto end;
        }
    }

    printf("fp256_mont_mul test passed \n");
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

    RETURN_IF_ERROR(run_test("fp256_mont_mul", fp256_mont_mul_test_vector, fp256_mont_mul_test, args.N, args.T));

    fp256_deinit();
    return 0;
}

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

/* gcd = greatest common divisor of (a, b) */
typedef struct {
    char *gcd;
    char *a;
    char *b;
} FP256_GCD_TEST_VECTOR;

static FP256_GCD_TEST_VECTOR gcd_test_vector[] = {
    /* 1 */
    {
        "1111111111111111111",
        "88888888888888888888888888888888888888",
        "1111111111111111111",
    },
    /* 2 */
    {
        "ff",
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffff",
    },
    /* 3 */
    {
        "1",
        "1",
        "ffffffffffffffeeeeeeeeeeeeefffffffffffffffffffff",
    },
    /* 4 */
    {
        "80000000800000008",
        "123456781234567812345678",
        "1000000010000000100000000",
    },
    /* 5 */
    {
        "80000000800000008",
        "123456781234567812345678",
        "100000001000000010000000",
    },
    /* 6 */
    {
        "ffffeeeeffffeeeeffffeeeeffffeeeeffffee",
        "ffffeeeeffffeeeeffffeeeeffffeeeeffffee",
        "efefdff112120123445433234454332344543243435342212132",
    },
    /* 7 */
    {
        "af368de01bc2da45964b",
        "230ae92cd25a2ba7847566666666435b7d39940c3abee1f1",
        "a438269ad4bfa8e38c2c951d6e115a248f82729ebf6",
    },
    /* 8 */
    {
        "36",
        "c0528588e3572782",
        "2132271ed05c1c73bccf1ef9279025e08571b8ca982093949554",
    },
    /* 9 */
    {
        "123456781234567812345678",
        "123456781234567812345678",
        "123456781234567812345678123456781234567812345678",
    },
    /* 10 */
    {
        "dfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdf",
        "dfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdf",
        "dfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdf",
    },
    /* 11 */
    {
        "7",
        "92080effeeef938f89988471b2c17edb326a0f5a33cded",
        "2f40d2bb22453e6990ec57c6cbb648928b239f0dc9448be1d5c6e3c8",
    },
    /* 12 */
    {
        "1",
        "f5dda591559e0b6c21f0c2a16fb97106a6e5ec212773869513021d11877207a4",
        "16d1",
    },
    /* 13 */
    {
        "1fc0d9283be8c8e5cd2d4f9a2a47a714015160a",
        "a55d5f4f1fb99260e15718e1fad97cd45ac7e31e4a0cbd663e770989f16ab26a",
        "18ef9312c3408d479a72e2d23dab2311ee84bd2cad766",
    },
    /* 14 */
    {
        "6",
        "c",
        "12",
    },
    /* 15 */
    {
        "0",
        "1",
        "0",
    },
    /* 16 */
    {
        "1",
        "c2056245b505d3573486bd347102d8574f3e34548f7",
        "16d0eb2f6f62948cea841a2ab60dffefdf895d7fcfb913e155fa1039249f",
    },
    /* 17 */
    {
        "35",
        "10605c2f31980a51d2498f84ef85717a805e6bbb32a4afeba0cf5cd04870e",
        "317abc681e827f74aa1b54d",
    },
    /* 18 */
    {
        "540c52db470d09f3f17",
        "eda11fbaaaafe365cd5bd26",
        "837ce171fceb70c984591a87b5f219fa6a63c13",
    },
    /* 19 */
    {
        "104ecbc32a3bf9d889124d4fb2fee",
        "1dcef745413fd6c443345b1e90ef0e932",
        "a4b294ae68e3853f44580e632854fc30ead70ef05341a",
    },
    /* 20 */
    {
        "37e12b1044e5af9bf1aba25e7759b5fc56f7",
        "a240a2d493cc92fccafeee3980b6b9a4bac9845cf2a933474bd1cdbd1539de39",
        "303d622d0b7a48999fa12b2b8d08721ad7133b",
    },
    /* 21 */
    {
        "139374574e44ae28b1ad682f42fb35177cb2a88e3",
        "a7eb3f3d490e43198089c4fa994a7f0e63df9b3c16a2ae6b58",
        "ca51539531a66c71d71c7c028870e291c259384ea321b39eed1b",
    },
    /* 22 */
    {
        "7777a935c30fc5f822ffbd9ff43d25f1cdaaadf168",
        "a1fd3fbeddd84607d3f57d923c34b45b1404c15bdbc4655e3a9050e9459c8",
        "15e853ce7facd8b381c87d2e4bd1381cb8f71cc2337c04b6ba5372edde408",
    },
    /* 23 */
    {
        "d94e3b37018138d3b002f2d1d8a3c2b7afcd91b5f50818",
        "4066d7eee2914e21e84ad066afdce1f0e022a59b53562f67aa5a7f48",
        "1e13aea10427c3f7790da0ff6ca93dd9831cb7f5d2dedd560d385a28",
    },
    /* 24 */
    {
        "ed99160e4dc79af1927b5a8e585a6e16528bbf7b13f2ac7",
        "b52c3972079eab15dda2676bf4e9b3c553fd9913075b3e6e82976f",
        "1044693335807d4bd5a53a0d75fad02a973b96e581c1e75519a1e",
    },
    /* 25 */
    {
        "4fbf302254911efc0cbe26536eff8122afd24ea105f83",
        "1cf16f73bf0a83f8c589c20131ab8da58a784d9512d8b41f6d7770dfc36d9d89",
        "7bf2ed1aa4e9d1d135d770a1f5d567208f76968df16e0ab7213163af",
    },
    /* 26 */
    {
        "a08",
        "1054cffc749a5d42918440",
        "af40f628",
    },
    /* 27 */
    {
        "2",
        "8ebe18d2d21a90e9ccc1bc25045264d7fcc7e292c29aad78",
        "4a586095d42c8de3249503a655217a529d4fb02336c040ef756a3529bf1798a2",
    },
};

int fp256_gcd_test_vector(void)
{
    unsigned int i;
    fp256 tgcd, gcd, a, b;

    for (i = 0; i < sizeof(gcd_test_vector) / sizeof(FP256_GCD_TEST_VECTOR); i++) {
        fp256_from_hex(&gcd, (u8*)gcd_test_vector[i].gcd, strlen(gcd_test_vector[i].gcd));
        fp256_from_hex(&a,   (u8*)gcd_test_vector[i].a,   strlen(gcd_test_vector[i].a));
        fp256_from_hex(&b,   (u8*)gcd_test_vector[i].b,   strlen(gcd_test_vector[i].b));

        fp256_gcd(&tgcd, &a, &b);
        if (fp256_cmp(&tgcd, &gcd) != 0) {
            printf("fp256_gcd_test_vector %d failed\n", i + 1);
            test_fp256_print_hex("gcd = ", &tgcd);
            test_fp256_print_hex("a   = ", &a);
            test_fp256_print_hex("b   = ", &b);
            printf("gcd should be :\n");
            test_fp256_print_hex("gcd = ", &gcd);
            return FP256_ERR;
        }
    }

    printf("fp256_gcd_test_vector passed\n");
    return FP256_OK;
}

void* fp256_gcd_test(void *p)
{
    int64_t i, N;
    fp256 r, a, b, adr, bdr, amr, bmr;
    size_t al, bl;
    TEST_THREAD_DATA *td;

    td = (TEST_THREAD_DATA*)p;
    N = td->N;

    for (i = 0; i < N; i++) {
        /* a and b should not be 0, it has been tested in test_vector */
        do {
            al = rand() % 5;
            bl = rand() % 5;
            fp256_rand_limbs(&a, al);
            fp256_rand_limbs(&b, bl);
        } while (fp256_is_zero(&a) || fp256_is_zero(&b));

        /* r = gcd(a, b) */
        if (fp256_gcd(&r, &a, &b) != FP256_OK)
            goto end;

        /* amr = a % r, adr = a / r */
        if (fp256_div(&amr, &adr, &a, &r) != FP256_OK)
            goto end;
        /* bmr = b % r, bdr = b / r */
        if (fp256_div(&bmr, &bdr, &b, &r) != FP256_OK)
            goto end;

        /* adr and bdr should be coprime, amr and bmr should be 0 */
        if (fp256_is_coprime(&adr, &bdr) != 1 ||
            fp256_is_zero(&amr) != 1 || 
            fp256_is_zero(&bmr) != 1) {
            printf("fp256_gcd test failed, %" PRId64 "\n", i+1);
            test_fp256_print_hex("a     = ", &a);
            test_fp256_print_hex("b     = ", &b);
            test_fp256_print_hex("gcd   = ", &r);
            test_fp256_print_hex("a%gcd = ", &amr);
            test_fp256_print_hex("b%gcd = ", &bmr);
            test_fp256_print_hex("a/gcd = ", &adr);
            test_fp256_print_hex("b/gcd = ", &bdr);
            goto end;
        }
    }

    printf("fp256_gcd test passed \n");
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

    RETURN_IF_ERROR(run_test("fp256_gcd", fp256_gcd_test_vector, fp256_gcd_test, args.N, args.T));

    fp256_deinit();
    return 0;
}

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

/* r = z + a * b */
typedef struct {
    char *r;
    char *z;
    char *a;
    char *b;
} MULADD_LIMB_TEST_VECTOR;

static MULADD_LIMB_TEST_VECTOR muladd_limb_test_vector[] = {
    /* 1 */
    {
        "7",
        "1",
        "2",
        "3",
    },
    /* 2 */
    {
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff0000000000000000",
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
        "ffffffffffffffff",
    },
    /* 3 */
    {
        "1fffffffffffffffeffffffffffffffffffffffffffffffff0000000000000000",
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
        "ffffffffffffffffffffffffffffffffffffffffffffffff",
        "ffffffffffffffff",
    },
    /* 4 */
    {
        "0",
        "0",
        "0",
        "0",
    },
    /* 5 */
    {
        "4a153aaaf99e50d0034273f407820557e382dd7b3d232ed7381f9a58e699525e878f09aa6a809d936fe2345633853",
        "3acad8aa548e083",
        "160e5c778d16353d1c2a0ffc7decb8c4ceb5eaac26e5f42d20d4dd4e55d278e61eaaa2d38ced361c4ab02621",
        "35bdd0",
    },
    /* 6 */
    {
        "ca1e7fa1accd294ef637e2e2885f3d533bf926e71",
        "1dc264c11c9710e7464ffef20f6f2f35a4cd",
        "64c31d058f6aaea7a93dd8b2f04936",
        "20182d94f696",
    },
    /* 7 */
    {
        "19990033393a00e1673bcf965d",
        "7f75c6f004cc171",
        "1b45f814a70e919",
        "f0458135dcc",
    },
    /* 8 */
    {
        "2d0d00cbbcba62eb7dab960558f445ea",
        "49a53301629927ff34f76d6b55a",
        "4c8c8f7c9910007840d0fc",
        "96a94e02dc",
    },
    /* 9 */
    {
        "34eaa5e52723e2f2a26ef4c2bcf4de453731e52f2e6f1be4e54a7d8d85c2c67292ac559f0dcde46a951b7b7ae4adce48a29d8d6d253769157e",
        "4499b486174a2f343b26db8d4be5479d8755118cc0d082b1fe6de67c37c18c8fc2526c41cf9eb32d01bc055f48e5e8c8aaed25bee",
        "dbd64d68e80a119175139a48e6fb2a0a989e46130dd8ed6ff4854875ef1d463e0996e1b5ce61ac10abc8b3f010a964f2e4ee",
        "3d9f0d36f03378",
    },
    /* 10 */
    {
        "3f5a9dd58a73ed0ff66385095a88d6cd1f94c5772a507d76dc2a1019c8be3b412e5def540d7b357fcd96c98f04050ea918a5f9441ef985b98289560acffe1",
        "3f428df6b1d287fec217daa4552b4e4f046a792b9690d5d8863bfd59529d7eacada692d41cd9b3f283083ce15934a8dab1ee898383148d9e46f4b08ce72a9",
        "7ab4218f7caa5bf7b6d0b9518daf2998f5994a7f79b0fd18210999e8f363b9ec8dc66064c6a08995fd4db7d2a583af589cb4287b64a",
        "32338233011c976c",
    },
    /* 11 */
    {
        "f2d9451cffc6beb33a51676151aa66e26dc0f042fb1dd",
        "c96bc47133b6f46b2389cacdea34427c4b4dd",
        "f2d9451c365afa42069a72f62e209c14838cadc6afd00",
        "1",
    },
    /* 12 */
    {
        "6df8ec4f61e97a135e5a1d78ec9a79cbb620d82bd6a62bdd95f3e77702fd14c8e4fe86686b4886233c3dcbf60654bb",
        "844f36e396af75a9492cc70ae43dacdd77b954094193fdb9a83771adea12",
        "1ba654d26009e42866cb0631bfe17e910ab28b293186aa5bf62dcf18b9de28e8011c022f13f4736926b",
        "3fa32e31043b",
    },
    /* 13 */
    {
        "5ef6630a99c27cfaaa22316ffe2c2fca3e40c29433e1f1f15d88773cfc368a438a1b",
        "5ef6630a99c27cfaaa22316ffe2c2fca3e22c33cd0860f531f425bfa56d5913b23a3",
        "123d9f8d4ab93b219c571cf8eef51318",
        "1a5",
    },
    /* 14 */
    {
        "11ffb9c30bdacac201a06625c9eb",
        "22945a",
        "15eaabb225110483929f09e5",
        "d23d",
    },
    /* 15 */
    {
        "212a0493a2b6a9f6a59792c263c094f4a5ae37e1730c0d",
        "3452d51f38b16155d1e27a7016a23a",
        "d8f796027bf64f107c5bff2fa0ab3bbee1",
        "27216410c333",
    },
    /* 16 */
    {
        "b5befbbdf462c881d0ad8c685b",
        "b46d74c4c3c2ec805b89b8451b",
        "42bacab12e7587f8",
        "50ee19398",
    },
    /* 17 */
    {
        "c1d6b274ffeacaab67349066e8ccfcfb9f54669f8f9a1701a37ed5591a6cce99f1975d79a96d2b970b9d751409280e72e5b523cd893444b1413361f037f151af63e712291c6894b23c133f36",
        "176eaa097101cc45d29b694111b6f12978ddc0592c956db60",
        "31aa58aab39b169c8c8fb2e463ebaaa88c3f47a4284a87e06da9bc77f3595122c18888aa8ee37c03e1b650e2ac664c1fb7b066cc13ccba531277e83b48dd5d638f97698426bc73d64a",
        "3e723cf",
    },
    /* 18 */
    {
        "8eb8b686c15f724e4e48889ffed91f89ec334e26520476495874467341d39d535731e35df3c9ffe00e10fd29887ee7f64bb4223f86f7b574a7b33c57adf214d9f93bd",
        "63e29ac21630d2ca4187e1bb548b1df93b40c75a0c637af8f4da610a23d45f6a5fa99a789de464e9d42e833648c1150a73442ac4a6ee878ac04dc1bffa8018d76cd5c",
        "c68d49ae0384063e773004beaa0807c0e4c2b89d33a0ea293c94715cf237214565ed10b2ede12a8f5bb3772497411679e027d63ce8f853ec6a151933",
        "373afde806a1b",
    },
    /* 19 */
    {
        "139acdb7fbd14ba4c7058bf9c6ba79d08a0717cf47eb0eb7ee834fa5a9cf66643e8aed4bb464c2444ae56c5d7d818293b9d1f2fa94e8f50ac51",
        "4c59e050ac25994a5e180",
        "5279799b96b4a531b7b9d6a609a3c1827961f0f543409c81fb608c4476173a7c81091ebe9b9c818d12435ae256e141529632de171a98bd",
        "3cda5",
    },
    /* 20 */
    {
        "740db2af474ef82136767b20264de37a4410b765f8291890965ded389af6c52",
        "2",
        "d2d2ce9a842a325aec9d9e45da3b3b468690d924e65642d5a6",
        "8cec12a6f4838",
    },
    /* 21 */
    {
        "88888888888888888888888888888888888888888888888888888888888888888888888888888888888888888",
        "88888888888888888888888888888888888888888888888888888888888888888888888888888888888888888",
        "",
        "666",
    }
};

int ll_muladd_limb_test_vector(void)
{
    unsigned int i;
    u64 tr[12], r[12], z[12], a[12], b[1];
    size_t trl, rl, zl, al, bl, max;

    for (i = 0; i < sizeof(muladd_limb_test_vector) / sizeof(MULADD_LIMB_TEST_VECTOR); i++) {
        /* clear */
        ll_set_zero(tr, 12);

        ll_from_hex(r, &rl, (u8*)muladd_limb_test_vector[i].r, strlen(muladd_limb_test_vector[i].r));
        ll_from_hex(tr, &trl, (u8*)muladd_limb_test_vector[i].z, strlen(muladd_limb_test_vector[i].z));
        ll_from_hex(z, &zl, (u8*)muladd_limb_test_vector[i].z, strlen(muladd_limb_test_vector[i].z));
        ll_from_hex(a, &al, (u8*)muladd_limb_test_vector[i].a, strlen(muladd_limb_test_vector[i].a));
        ll_from_hex(b, &bl, (u8*)muladd_limb_test_vector[i].b, strlen(muladd_limb_test_vector[i].b));

        /* tr += a * b */
        ll_muladd_limb(tr, a, b[0], trl, al);
        max = ((trl + 1) > (al + 2) ? (trl + 1) : (al + 2));
        trl = ll_num_limbs(tr, max);
        if (ll_cmp_limbs(tr, r, trl, rl) != 0) {
            printf("ll_muladd_limb_test_vector %d failed\n", i + 1);
            test_print_hex("r+ab = ", tr, max);
            test_print_hex("r    = ", z, zl);
            test_print_hex("a    = ", a, al);
            test_print_hex("b    = ", b, bl);
            printf("r+ab should be :\n");
            test_print_hex("r+ab = ", r, rl);
            return FP256_ERR;
        }

        ll_copy_limbs(r, tr, trl);
        max = trl;
        /* tr -= a * b */
        ll_mulsub_limb(tr, a, b[0], trl, al);
        trl = ll_num_limbs(tr, trl);
        if (ll_cmp_limbs(tr, z, trl, zl) != 0) {
            printf("ll_mulsub_limb_test_vector %d failed\n", i + 1);
            test_print_hex("r-ab = ", tr, max);
            test_print_hex("r    = ", r, max);
            test_print_hex("a    = ", a, max);
            test_print_hex("b    = ", b, 1);
            printf("r-ab should be :\n");
            test_print_hex("r-ab = ", z, max);
            return FP256_ERR;
        }
    }

    printf("ll_muladd_limb_test_vector passed\n");
    return FP256_OK;
}

int main(int argc, char **argv)
{
    TEST_ARGS args;

    if (fp256_init() != FP256_OK)
        return -1;

    RETURN_IF_ERROR(get_test_args(argc, argv, &args));
    test_rand_init();

    RETURN_IF_ERROR(run_test("ll_muladd_limb", ll_muladd_limb_test_vector, NULL, args.N, args.T));

    fp256_deinit();
    return 0;
}

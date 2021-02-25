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

/* r = (a - b) mod m */
typedef struct {
    char *r;
    char *a;
    char *b;
    char *m;
} FP256_MODSUB_TEST_VECTOR;

static FP256_MODSUB_TEST_VECTOR modsub_test_vector[] = {
    /* 1 */
    {
        "0",
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
        "ffffffffffffffffffffffffffffffff",
        "ffffffffffffffff",
    },
    /* 2 */
    {
        "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffd",
        "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe",
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
        "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe",
    },
    /* 3 */
    {
        "13b25c1f3edd6818044def6825d3309b7292d0ea602d54c4",
        "13b25c1f3edd6818044def6825d3309b73ea9e14f9594511",
        "157cd2a992bf04d",
        "5f3238d80810accfeab2cad9623e5671eebc1358b5d594c43d432b",
    },
    /* 4 */
    {
        "3d94c0c57fb36a97beec5743a",
        "297319a5518b183096933c5b5f45815cb1",
        "325fb",
        "ad3a6e2e706de3dd2ceb9c38b1",
    },
    /* 5 */
    {
        "24440ac59445afa55c6b7c11e02ae5053a",
        "8",
        "1aa886be1cf3a1943b4e6a31ed28fd18ba084f521d322edab2f9799a5",
        "55c49ca58a5d0c9f78364c36bb4f50860d",
    },
    /* 6 */
    {
        "15f9987babade8ccf337bab4a66055db747bcbcb88e074b244c32e73",
        "25b25654",
        "3f0b59a7768e30b1199db50dd202320172e2f573fec",
        "15f9987babadecbda8d2321d896b67754fcca8ebac008be04e68180b",
    },
    /* 7 */
    {
        "1db4e183b7d278a30737f7f8c8e9331dd6ffe83f19299757c07e7c31",
        "70202b8ccaeed22c2841e79c85aaf71b8a685202b22283ecb56f7f1d7535",
        "b5ee0bf82035db57a6c7bd787f445d5348b56513184c5f580dc14",
        "249d7fe2222ddfe7ba5d003cc82ab9e7d6b24ead70c7e2a1e1c67f31",
    },
    /* 8 */
    {
        "65",
        "19960a0f2ce8dc00134d0405104eac4f6c26900b2e4b",
        "11ea8423ea4784",
        "66",
    },
    /* 9 */
    {
        "66078ef38f76de4",
        "696e532159acfb4f62631139719c1389593d830b27be0fc2d517b4b2967a0912",
        "53d40b76e6952aec1e533c002b990f8cd87aac",
        "7bc96865e8a5e46",
    },
    /* 10 */
    {
        "933bd39f19880a2c218eb71ba44de638cbc2ce02c44f87f180c3ce",
        "14edbf05c1718be3dee4f2084",
        "15c45cb9a76a679344296aa0931b8c3e5",
        "933bd39f19880a2c218eb877ea197f60964ba62e423b5334ea672f",
    },
    /* 11 */
    {
        "6",
        "1",
        "cbc50350844c5e217a834ba2370ea5684d918961b0f2bab893b4cf",
        "7",
    },
    /* 12 */
    {
        "0",
        "19a614a5ec7dc6",
        "251d1f9ef9157c96062ae3b9a44f39de6f7a2c8ee",
        "7",
    },
    /* 13 */
    {
        "9",
        "3d77c1a8ee0e4b8593ab8c41d8ac5b2d93a81faa1000e4ff8d653882a",
        "7d06c33ceb12532c9640f53af918444965b1e91e2406bf799c008d32f",
        "d",
    },
    /* 14 */
    {
        "28f706075a9a8451e68c37477c06b65f5b70cdf31123b5864bb3bc187a",
        "17ee788ce0",
        "daca7d75699bb8872d44f048ac0cf55e9f656a7e956bb17718fdbb3f8611",
        "8172f76aae638475f2809548b117a3d14a5930a8390bb9bf554026241b",
    },
    /* 15 */
    {
        "2ebd9ad0cf0bc8f07f1ca44be9064dc42ccc06e0",
        "dfde31d88035452af2d8581dbe15437a8553b7d13f06d37759eff7926a928bb",
        "193ba3bc75b195063844cc2d23446373f61bec7d",
        "e0e501d42392669656054ef7f5d62eb94603788e",
    },
    /* 16 */
    {
        "1122aed336bdaea5f74a168ba7411b1d492b0a6243d4e60827aeaec594f15b",
        "1f1",
        "176b32c40e18fdd7d9a03116dadb9a935ed2",
        "1122aed336bdaea5f74a168ba75886500d3923601bae86393e898a60284e3c",
    },
    /* 17 */
    {
        "2745136fce99d16ba2930c42c19af5b4b",
        "27f2eb186e37218caf1ab7cffbd04b3d6fbe21891435666d2072992924",
        "4e0ebd2c0d0649eafa5d384b2385edac567baedbc13afdeb50a90b554e",
        "114cc581a262429e96b4b57db841ae16e1",
    },
};

int fp256_mod_sub_test_vector(void)
{
    unsigned int i;
    fp256 tr, r, a, b, m;

    for (i = 0; i < sizeof(modsub_test_vector) / sizeof(FP256_MODSUB_TEST_VECTOR); i++) {
        fp256_from_hex(&r, (u8*)modsub_test_vector[i].r, strlen(modsub_test_vector[i].r));
        fp256_from_hex(&a, (u8*)modsub_test_vector[i].a, strlen(modsub_test_vector[i].a));
        fp256_from_hex(&b, (u8*)modsub_test_vector[i].b, strlen(modsub_test_vector[i].b));
        fp256_from_hex(&m, (u8*)modsub_test_vector[i].m, strlen(modsub_test_vector[i].m));

        /* r = (a - b) mod m */
        fp256_mod_sub(&tr, &a, &b, &m);
        if (fp256_cmp(&tr, &r) != 0) {
            printf("fp256_mod_sub_test_vector %d failed\n", i + 1);
            test_fp256_print_hex("r = ", &tr);
            test_fp256_print_hex("a = ", &a);
            test_fp256_print_hex("b = ", &b);
            test_fp256_print_hex("m = ", &m);
            printf("(a - b) mod m should be :\n");
            test_fp256_print_hex("r = ", &r);
            return FP256_ERR;
        }
    }

    printf("fp256_mod_sub_test_vector passed\n");
    return FP256_OK;
}

int main(int argc, char **argv)
{
    TEST_ARGS args;

    if (fp256_init() != FP256_OK)
        return -1;

    RETURN_IF_ERROR(get_test_args(argc, argv, &args));
    test_rand_init();

    RETURN_IF_ERROR(run_test("fp256_mod_sub", fp256_mod_sub_test_vector, NULL, args.N, args.T));

    fp256_deinit();
    return 0;
}

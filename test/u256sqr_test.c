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

/* r = a^2 */
typedef struct {
    char *r;
    char *a;
} U256_SQR_TEST_VECTOR;

static U256_SQR_TEST_VECTOR u256_sqr_test_vector[] = {
    /* 1 */
    {
        "1",
        "1",
    },
    /* 2 */
    {
        "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe0000000000000000000000000000000000000000000000000000000000000001",
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
    },
    /* 3 */
    {
        "13b0d127979db535d62723a1a4da01cb8e94621251f8abc05216e4e3292edf8c2f72a8bd014508436c6a26ce36ed3eacfdd68e556b839e1db9dd1c8789f0f959",
        "46ffab8bb03bcd8de040a2066ed0569a8f1be6459510f8fa1bb3c0a580718ddb",
    },
    /* 4 */
    {
        "5438463da7e8d85011634e145be3923d740daf91c3b47f84418c92934fc8518483e89",
        "24b56317674506c13ae25724882498207c3",
    },
    /* 5 */
    {
        "0",
        "0",
    },
    /* 6 */
    {
        "40",
        "8",
    },
    /* 7 */
    {
        "35b56845dad71961666731d555c41360616100",
        "7542038e3ed8abec310",
    },
    /* 8 */
    {
        "28785d6728eb581a513953434234f867b3cd24d236c1127e9204aa8b5b3e74bf5cb2835ee149046e77e6657a946f1fdf1bf4f8b7906d10807e910",
        "19724bd113ef83d8af1e70ba79eda299cd96d397e501fe5a4c83e3770a4",
    },
    /* 9 */
    {
        "a15cbf5bc698db6157238fc50b13b341ad064fb2183139248b1fd8c5767c313c5aeae2931d084595fe4025e67af43a892e9e02a46def9711160f91dbc40",
        "32cfb6d8660a90ec62d035a0b81ecca87531687ed0b54328b6f77810b062c8",
    },
    /* 10 */
    {
        "76d79205139c77a7a66ffe613fe3f9c4a100",
        "ae6c6cf200c727dd10",
    },
    /* 11 */
    {
        "3f40906a204f499979c3c485c267973660e3738284512995b791a3ed120259765a32ed4c5f930290563f4c9a8b7bd81a4",
        "1fd0001ab01bf12f29e21335c685fc7ed031951f617d3bada",
    },
    /* 12 */
    {
        "2827f9144998b62879",
        "6563e43f5",
    },
    /* 13 */
    {
        "24295a4ee555c17b20182c8cf588a0c4d2e5fb6e60a6a6a82140c96cbbfc13c1835ba90",
        "180dc4d1de63f1e6744c806aecd9377c71cc",
    },
    /* 14 */
    {
        "672872d5d950ff8719fd55f700420ed952cb14bcb6060f9532aef8991fc71814ab862c4e49165c16c3df4f4c04c1e682304",
        "28a06ed53cf620f71aa349dd9a2b91e6da5461e16bc44f9b3e",
    },
    /* 15 */
    {
        "b02478391323048c31f03f4bd0fb60278955e19174882694e133d34b6293842a9109b5660b1461cbf3189ad77fb86abe83c06c6f598ec8bec24",
        "3516645c2b7ef9774b08f9663fe67ab5b33c8062c066e24e385eb4e906",
    },
    /* 16 */
    {
        "4a172fe2deb3db29fe13efe92be544ed9fee58a44332b4108e38fe21bc7dbc9a7bb1e2058bd876e34f466d3d5920cb5de6600b5ff096c9f63213fc57d444",
        "89b8ae64a3b8917eb84785d5b86181559b46ca38711074f0c29cb83992c52e",
    },
    /* 17 */
    {
        "590b2189fb5a20208668163932516bc022c8b453a3fa2081ee02b9616427997d0d77ca141f0a3fe2a10f05e5413435e05484",
        "96fb064f07059842dc98d4098029428bce7da8e7390cd92bde",
    },
    /* 18 */
    {
        "937657b0d47883fa9a70ee13aa826314ab7fe3cec0a65864b16b9d90000",
        "3092d8a88f13e3081de57ce4956500",
    },
    /* 19 */
    {
        "ff104181fc1e93656114251602d24e75a236d372b0474d4ada1db0a2d85d3f67f4aaa54f4c82cb02c4c792d6ce6cf8b617746f6996c74e9",
        "3fe20128caa01d50045ad856281472ffdc01fa108fa31a9787b1ff53",
    },
    /* 20 */
    {
        "245c6806dc759507599d7511e8df79026586952c4dd71b5408d239457914ecd169b76cffdfd661acd168bd9bc31d62844c74c596933c316349c400",
        "607ae6b2e2bf62e6c69ca05ef9821a2b1543edd58e344d1c625346afce0",
    },
    /* 21 */
    {
        "8bc07f36b656ab384fcb5c3c8fc87a7bc7fd910e64cb8535c52de943470fd5ae826420bba0518f1182e32cd48120b92e5a4b71da09fb9bf90",
        "2f49648546952923b3d8c1baf31bbb6410465751f2f5cc59356305ad4",
    },
    /* 22 */
    {
        "3144308400b68f73a85a594ca968ae661",
        "1c1374d0ec78416b1",
    },
    /* 23 */
    {
        "92f4c54400f4953d8b7a330e7c91cb43f04fc98607fdd017e7c9ff826b155e49699cabb6496d1e79b81f3c1e7afb0ee6945cb229341afb5b26c1",
        "c1f5f360fff10380e27aef94c3930fe5809cce5ac4313974bdacd0a161",
    },
    /* 24 */
    {
        "a91b473b34e24e3d97beede64c0ec0208d6ecef2f3b6d7fc5a2414ebf3773c50db6d80f0397a249574c2b7afb69d31d09edfb5a0352959cfca945cfc3b2a640",
        "3404322c8a0ad5db8c1f01a7483e4697841c25ea2666e88f3c95cb4549129dd8",
    },
    /* 25 */
    {
        "8f70bb818085f5ab3c4436714533a7c4e2642eb5cecb8fd11b118c2d32a20e04f879ddbbb03f01dd3c77d20fe4ac1932f55ba6e90",
        "2fe8194cdfdb4f1d64f71cf1ac51f4c764840744bf958a1ee76b4",
    },
    /* 26 */
    {
        "c7eba706d34662b03db908d00078aee862a625fa191d48d6a58d3b5e286d32416bae46d1288e82207fcdde1bc6ba77ba62cedd5c99c525a5766229b9",
        "e23aad1217d3e57ce31643beb80d042b4f3af0039a0c7bb617e4d70a3beb",
    },
    /* 27 */
    {
        "55f192e8ebdaea646407c2489d3ff98c959de4f4467a21bd74877cee8bc73cb8a5dcd99c890c55f3df5f05fd097dc2ee9e2944bdaea8aa789930de1f0c61f1",
        "94544b6051b032bb3bdd2eec25a2640af919e86ce85def06357fb3463292227",
    },
    /* 28 */
    {
        "8e17b67d2ca16e138601714ef219f1fe06c11f28c4d37928cddb3c672054a531803c8356ec9f423a153eb38c89148f6702aa6cf1a8798afcebc4f7a19",
        "2fae58f7309b9be8c21cea88bae32719201fd31ff716ed214cbef156fd905",
    },
    /* 29 */
    {
        "31d11ec2e0792ff87b74d4c1407bf8f0561087b34bb1a3c7e865bc2c5279f2902168c06f60ea0fdd1c6721df0272ec1c4f75a2df4e125b8cb5f6ab11",
        "70ee01b1f8a007b5e2cc1c8a349a849e833d7f8a39512f5014f3d046bf97",
    },
    /* 30 */
    {
        "a4a8dc6a60d2b0338127445832bb3c87356869ec7f9395f53c03722fef35b5d6699da6acc6cd9fdade148832ea4d96d64d36ebf37862c5f024a015868e92590",
        "3353f191dba79fbd69bef042a49138af59e94ce28f73c167f396b22de375ed6c",
    },
};

int ll_u256_sqr_test_vector(void)
{
    unsigned int i;
    u64 tr[8], r[8], a[4];
    size_t rl, al;

    for (i = 0; i < sizeof(u256_sqr_test_vector) / sizeof(U256_SQR_TEST_VECTOR); i++) {
        /* clear r, a first */
        ll_set_zero(r, 8);
        ll_set_zero(a, 4);

        ll_from_hex(r, &rl, (u8*)u256_sqr_test_vector[i].r, strlen(u256_sqr_test_vector[i].r));
        ll_from_hex(a, &al, (u8*)u256_sqr_test_vector[i].a, strlen(u256_sqr_test_vector[i].a));

        /* r = a^2 */
        ll_u256_sqr(tr, a);
        if (ll_cmp_limbs(tr, r, 8, 8) != 0) {
            printf("ll_u256_sqr_test_vector %d failed\n", i + 1);
            test_print_hex("r = ", tr, 8);
            test_print_hex("a = ", a, 4);
            printf("a^2 should be :\n");
            test_print_hex("r = ", r, 8);
            return FP256_ERR;
        }

        /* r = a * a */
        ll_u256_mul(tr, a, a);
        if (ll_cmp_limbs(tr, r, 8, 8) != 0) {
            printf("ll_u256_mul_test_vector %d failed\n", i + 1);
            test_print_hex("r = ", tr, 8);
            test_print_hex("a = ", a, 4);
            printf("a*a should be :\n");
            test_print_hex("r = ", r, 8);
            return FP256_ERR;
        }

        /* tr = lower 256 bits of a^2 */
        ll_u256_sqrlo(tr, a);
        if (ll_cmp_limbs(tr, r, 4, 4) != 0) {
            printf("ll_u256_sqrlo_test_vector %d failed\n", i + 1);
            test_print_hex("r = ", tr, 4);
            test_print_hex("a = ", a, 4);
            printf("a^2 should be :\n");
            test_print_hex("r = ", r, 4);
            return FP256_ERR;
        }
    }

    printf("ll_u256_sqr_test_vector passed\n");
    return FP256_OK;
}

int main(int argc, char **argv)
{
    TEST_ARGS args;

    if (fp256_init() != FP256_OK)
        return -1;

    RETURN_IF_ERROR(get_test_args(argc, argv, &args));
    test_rand_init();

    RETURN_IF_ERROR(run_test("ll_u256_sqr", ll_u256_sqr_test_vector, NULL, args.N, args.T));

    fp256_deinit();
    return 0;
}

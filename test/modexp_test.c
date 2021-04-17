#include "test.h"

/* r = (a ^ e) mod N */
typedef struct {
    char *r;
    char *a;
    char *e;
    char *N;
} FP256_MODEXP_TEST_VECTOR;

/* even modulo is not supported yet */
static FP256_MODEXP_TEST_VECTOR modexp_test_vector[] = {
    // /* 1 */
    // {
    //     "d68d381d8d490768b18934275699ef0fc9805b43",
    //     "fffffffffffffffffffffffffffffffffffffff",
    //     "fffffffffffffffffffffffffffffffffffffff",
    //     "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee",
    // },
    // /* 2 */
    // {
    //     "1",
    //     "1",
    //     "fffffffffffffffffffff",
    //     "eeeeeeeeeeeeeeeeeeeeeeeeeeeeee",
    // },
    /* 3 */
    {
        "8888888888ccccccccccccccccccccccc88888",
        "8888888888ccccccccccccccccccccccc88888",
        "1",
        "ffffffffffffffffffffffffffffffffffffff",
    },
    /* 4 */
    {
        "1",
        "2",
        "100",
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
    },
    /* 5 */
    {
        "8000",
        "2",
        "fffffffffffffffffffff",
        "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
    },
    /* 6 */
    {
        "694da28143df5d4049feddec0dde8b1301284f7f7f98976b0c26cb805dc0d305",
        "38931b8847c13b3d2a72b5637591576922ae2f0501f24946fa89cacb0",
        "14eef9cb6d203ea4388b74a4ae5eca2b98aae1dda8a518e",
        "efa64f9b0d079ae72a53d95c28260a544dba2a5a386078c4d2c7997953a2860b",
    },
    /* 7 */
    {
        "1dd99a1b4b8b8db9b13db3797210abee9610e8fcf56a2fc1d3a7e95f22526ce3",
        "1404cc0acf6d31da85f28acd1b5290780c69142e560cd6083dac",
        "14389160387c2e1de18c96710cc0f98e0ce10083693eae72d11a3b19",
        "43971e68c750d89bc85821c464a75c31988ae39f27451c59fa8d157590582e27",
    },
    /* 8 */
    {
        "4914761077e1da9a60343143adf6a546b91277a742aa5c40b00e7bab",
        "1bdf8e7a3537bce4fc0eca4779afbe635417d1b84a55f11c8744f",
        "45862d0dc057e0d1055b7906ae31abeb4ed442aa36a6bc09ae644b3fc",
        "581544ef3372e8d18f426f244557159a0f120610aa891bf68b6d2629",
    },
    // /* 9 */
    {
        "57e4ac1610799af578943235243eb0191077b0bec956349e4",
        "562e3112a1e62f86584fccbd3aee847b4ea",
        "477a3f2202e09673883ce048f4216137739109d198f28bfb8",
        "5f1aa35ac451812e6a386ce3bacc3f0585573e704857df35d",
    },
    // /* 10 */
    // {
    //     "51f0dcb7d5203b9e057b660d7e0606b04d87778209857cd50bdbf3",
    //     "3d68c1040e6eaa8daa5c18006c1b8e50a347b172d5407e8c563fd6d9",
    //     "41f0ba0e03e54877d6ebb18cd726f34f4aae5f1a5c06f5bcbada34",
    //     "758255fb9b64943fcb4a244be298cdc49ee4b7d47b69ede084fcaa",
    // },
    // /* 11 */
    // {
    //     "160792c3e06718e0c09f7dbffe3582dc17f08bd656cf988cd51ddaf9",
    //     "7f99d331f491a479b465fc7e56789afacbab515",
    //     "1a610ee179d6073b4a7de7ca7e06d17e1edc32aef1ccc59f51279716",
    //     "7607b86859bc46d7632c242479524560990c0dc1b720973cd3f1a198",
    // },
    // /* 12 */
    {
        "3c949c8aa18de2965cd3963596e9aade71d8d2bae6572fd52ccc6c8ed",
        "7",
        "382b55ef3cbe4ec9ab771689dc948035fe09c9e1dfe91fb8cd8900cb4",
        "3fefdb2ee5a2ade130216d1bc7f8dd28c206572369ed1c390b0560e37",
    },
    /* 13 */
    // {
    //     "340109b1b500dcba7f17dd3441511fda58f44988c2f4e76285fb9a1285",
    //     "65cb8db79f73eb9ca182832bef7b13d04d0e74d5dce478ffc10a70b8c58b",
    //     "6fcc92db730583f8140fd810d1c7c7a3aa5f52d01048bd7c93aed67b46",
    //     "72b91f0a6a8960a83af01779516fd2b54bd7635d7e18deacf9d3fcdefc",
    // },
    /* 14 */
    {
        "135b84040d45c70a99ba2ecd0726ead7c7d14ec7ebd512b4773029302",
        "a8",
        "3ab5eb3d003d681b50f6c190c99952c5a3b54e99152d2ac0bcfa859527",
        "1afeadcafa73dd913bd46351378649e7ebf8ef32d56a565507d553239",
    },
    /* 15 */
    {
        "1a6026be5418c20f959028dda8814f5dd7c34ec7cf7829777eb236fc93cd9844",
        "29b6ffe1aa6f778",
        "1677eff166c388150666d6802b0ace1213da9e9b0a6699e0f0a373dfc09d9a",
        "1c12eba394075c9534d79651b70c32722af45e86eac49318cb1fd9c49d37ab53",
    },
    /* 16 */
    // {
    //     "8ce69118d8e4c50309d9691ca95c041ccece0957acb38a92c",
    //     "55d802611ffa72253a0749a8a30d790f98c4d182bb0588",
    //     "1ce99af1385a2c47d094708ea9a262d5ae6336e22cdaa04bd1",
    //     "2c33548cacf8f63901d800f2aa0c5d27fe2c269d663dc0ef4e",
    // },
    // /* 17 */
    // {
    //     "155ede6c214d2a471a1efccacd16ca45d16e891622df0fc2fea5b886b0ebf",
    //     "231f04d10187ddf7766a8ee90952a1ceb1fb422611",
    //     "209c8d8bd69d61a1a4f625",
    //     "2ba23119899c3058b114dfa844bf26cd4fabd737604d723073f2909736b2a",
    // },
    // /* 18 */
    // {
    //     "643c4ce8e4e68a72f8e2834dcc764b03e16b3c2c33ed6e51cf27b949b8b7fe1",
    //     "19f",
    //     "5033750407a",
    //     "82ec87098c8a8b9bb90d134025d86e3a98ee967005b2d40597ab573fdfa8f50",
    // },
    // /* 19 */
    // {
    //     "1f46f1e6621964b6de94dadc3def41cd1f5469b40d1ba1d4022a0",
    //     "2b0bb2e05dfbfc5ce16c905f13889bb626af9ddb81f0b15f68e",
    //     "3756ad23e1f2f",
    //     "2a9e51abe2364ddc432aa73fca7cbc6551aed9543634144d2a364",
    // },
    // /* 20 */
    // {
    //     "121b9e05985a7e4a614e29f345e5f86e056208741eb5a17502e18848c",
    //     "1c08",
    //     "192c545fdfa8b49d188013e6901cb52d10dad6085b511169a7",
    //     "1c5b2e932e86e16846983cddeeb543ca7ff7eb8944ac72381949e570c",
    // },
    // /* 21 */
    // {
    //     "2c503991912780afa0cfdeb1fa3437d404593a9dbc5ac2cb5a8b70",
    //     "59afc0f82471cc55e3245fb244b14e213ca60c",
    //     "bc7f",
    //     "adb8c87403f24b90384e12dc9781db32817c342fd69289d75475f4",
    // },
    // /* 22 */
    // {
    //     "e2d054275f21f585147240e147414f740dc2bc277651bf7b80",
    //     "3d58a",
    //     "19aa7",
    //     "57ed86022a1ba6c10aaabd7211bfd1ebde4847947739361e6c0",
    // },
    /* 23 */
    {
        "3d4012286016cfd61535a3c079223839d81201f0307a5720413eb41fd1",
        "c7155238d392002ed5dd4c5d2f86bdb98ef4",
        "3e0302d90cb4f82d64a99",
        "a370b21a203818f6de46b77d7ab78d424f561c027ecb7f856d6d6b72af",
    },
    /* 24 */
    {
        "35512185eca47906c212a959ca43a96102123ba1c4a331e73b5d0e628b27b4a3",
        "9200141712315000740b3ed89a0ae",
        "404728a2c56550677a48a187a8c9e596",
        "3de8a46b07a1f73dc0de4d990f723b0745c79f1b3089cc330252f6c71310036b",
    },
    /* 25 */
    {
        "6c7de5735540a6cb0df16626ea3aac12d0e0e3a77fe9a400451c4585854",
        "3ab9c2e41b70c837e82893f2c0b01259360d0043cb4db5595e5cc655760d",
        "66bb5d5b614c1e2ddf53e56fd155ce39925eb2c7b9bc2ad3bcb34767",
        "7915ee09ba36a516a2d91642663a63991591d687a4503867b640c0eba483",
    },
    /* 26 */
    // {
    //     "2f7b1000",
    //     "a8",
    //     "7c18001cf46bf8227df3ed8d252daf24e75166b0597727e5f04",
    //     "7c18001cf46bf8227df3ed8d252daf24e75166b0597727e5f04",
    // },
    /* 27 */
    {
        "c3e117d9adb92253e6a3cf10d8aa933b0ce8ddf64ea",
        "c3e117d9adb92253e6a3cf10d8aa933b0ce8ddf64ea",
        "2cd955ae98f59d641bd33e3a6b52c0ecee97f3a001afa921cc7d407205949",
        "2cd955ae98f59d641bd33e3a6b52c0ecee97f3a001afa921cc7d407205949",
    },
    /* 28 */
    {
        "1b2514fdc0cb8e948984",
        "1b2514fdc0cb8e948984",
        "395af19735a6fd7e5bf666bcacdbcf8d0a7df62aa0d324b0b1",
        "395af19735a6fd7e5bf666bcacdbcf8d0a7df62aa0d324b0b1",
    },
    /* 29 */
    {
        "907c10879b4d84826f85054c510",
        "907c10879b4d84826f85054c510",
        "b46956125dd2d4b163c420d1de90de5ab8c9f0fec1c80487fa4b9ef",
        "b46956125dd2d4b163c420d1de90de5ab8c9f0fec1c80487fa4b9ef",
    },
    /* 30 */
    // {
    //     "409c5e378f4a9d2cc9df21dec4bee0ed90a35ebd1f94c292bb3c92279fe316",
    //     "1ba95949af92f36a2e3d623d2af08489842a7aed8ef241e9ae2ccd0116a6",
    //     "19fd4fb1e44af0ed1016c6cc3f6ac5dc8dc2e292dc8040b20140fcd6e93af92",
    //     "19fd4fb1e44af0ed1016c6cc3f6ac5dc8dc2e292dc8040b20140fcd6e93af92",
    // },
    /* 31 */
    {
        "26228aa2abd8cc40f199dfdfa2aad842365c9",
        "26228aa2abd8cc40f199dfdfa2aad842365c9",
        "2b4bd4b535d5b870ac7aba79178335e20725307a49157ea128c29713d",
        "2b4bd4b535d5b870ac7aba79178335e20725307a49157ea128c29713d",
    },
    /* 32 */
    // {
    //     "0",
    //     "0",
    //     "ddddddddddddddddddddddd",
    //     "0ddddddddddddddddddddddddddddddddd0",
    // },
    /* 33 */
    {
        "3202eaebb292a1f60fbf746fcaad753d01faf7c44c4d7ca3fccf9853f11f50",
        "ea62",
        "36d3",
        "3258c427fe070787944e8b64b68cadb7f4aba0fa58af3698236667b681cf6d",
    },
    /* 34 */
    {
        "40000000000000000000000000000000000000",
        "2",
        "96",
        "30f58b8bdb4d63456f02d43def555eb31386e9afce90d08aa0bbb486f",
    },
    /* 35 */
    {
        "306676e2f20aa969725996dabc181edf8c9125b77a5510",
        "6f4ffd91d52fdf0932d05dc",
        "2",
        "294dd019bc848882457706837ff0426d0afe98efd8d57cc587d",
    },
    /* 36 */
    // {
    //     "43aa207883028e31e6d08c1c64afecff8fefc22cdd3ea08e450fc20e9",
    //     "6e70f0031b",
    //     "8",
    //     "b649fba6c82c71e2a25af285fd7b8923a5a561969cebf916aa8fe5c78",
    // },
    /* 37 */
    {
        "15d20f72fef6c13fd5f6687dce10dc47c594997769cff88dcf9f2e67a",
        "2b39dd9523f550945cd341b1fb29214f6ea549e597a008",
        "990621b03b3a521c5d752ada8f5018163536f33cde4c97529794af3cafeb",
        "49d6a7a7b4f6a30e050df617a38916dfd965f8d78963d2560427210bd",
    },
    /* 38 */
    // {
    //     "1afe9b86e31b397e2de1",
    //     "305",
    //     "8",
    //     "1e6f1410f56c1bf9bf05e0245328d0a18bbc4e29224855d6d16",
    // },
    /* 39 */
    {
        "7e5c2141b3b05dd95c219fb59399c5d9cbd230504c119fa5a18831",
        "bdd545e61830af9d241ad051b9e02c7e85728af88",
        "1539744cf413c5c08bec84ea6d084c1cac224f599ec1e291773463",
        "56ccb6b7c701b83cf396cfff41259a8526a837fc987617af79ef0ef",
    },
    /* 40 */
    {
        "2c90ec0c633baa3a607e858ad5c8e45f2c0c46ea30c608509f4",
        "2118392533eba571f213b31afcb646d6ec7d6ae5b5",
        "ce4d436222a2357f594e0391e3c52f598dc",
        "3d755cf926c59605248ad6b432cee5247c20bd7301c6432f5c5",
    },
    /* 41 */
    {
        "7e5c2141b3b05dd95c219fb59399c5d9cbd230504c119fa5a18831",
        "bdd545e61830af9d241ad051b9e02c7e85728af88",
        "1539744cf413c5c08bec84ea6d084c1cac224f599ec1e291773463",
        "56ccb6b7c701b83cf396cfff41259a8526a837fc987617af79ef0ef",
    },
    /* 42 */
    {
        "7b2d159b289e373823e5e3bb1e36fdde3780e76b7c25d1413041e25b8706460",
        "92ec83dad2d073e350812fe76545fb5283bc788f8d1cedeb",
        "2e826b576bbcf3d1bc3be07f92da6d8e",
        "21df0aa3e2250fb8fe6fb3f9f6dc6b394563184fb798527401b66efdff7f2ecf",
    },
};

int fp256_mod_exp_test_vector(void)
{
    unsigned int i;
    fp256 tr, r, a, e, N;

    for (i = 0; i < sizeof(modexp_test_vector) / sizeof(FP256_MODEXP_TEST_VECTOR); i++) {
        fp256_from_hex(&r, (u8*)modexp_test_vector[i].r, strlen(modexp_test_vector[i].r));
        fp256_from_hex(&a, (u8*)modexp_test_vector[i].a, strlen(modexp_test_vector[i].a));
        fp256_from_hex(&e, (u8*)modexp_test_vector[i].e, strlen(modexp_test_vector[i].e));
        fp256_from_hex(&N, (u8*)modexp_test_vector[i].N, strlen(modexp_test_vector[i].N));

        fp256_mod_exp(&tr, &a, &e, &N);
        if (fp256_cmp(&tr, &r) != 0) {
            printf("fp256_mod_exp_test_vector %d failed\n", i + 1);
            test_fp256_print_hex("r = ", &tr);
            test_fp256_print_hex("a = ", &a);
            test_fp256_print_hex("e = ", &e);
            test_fp256_print_hex("N = ", &N);
            printf("r should be :\n");
            test_fp256_print_hex("r = ", &r);
            return FP256_ERR;
        }
    }

    printf("fp256_mod_exp_test_vector passed\n");
    return FP256_OK;
}

/* naive mul_sqr algorithm */
static void fp256_naive_mod_exp(fp256 *r, const fp256 *a, const fp256 *e, const fp256 *m)
{
    size_t ebits;

    ebits = fp256_num_bits(e);
    /* r = 1 */
    fp256_set_one(r);

    while (ebits > 0) {
        ebits--;
        fp256_mod_sqr(r, r, m);
        if (fp256_test_bit(e, ebits) == 1)
            fp256_mod_mul(r, r, a, m);
    }
}

void* fp256_mod_exp_test(void *p)
{
    int64_t i, N;
    fp256 r, tr, a, e, m;
    size_t al, el;
    TEST_THREAD_DATA *td;

    td = (TEST_THREAD_DATA*)p;
    N = td->N;

    for (i = 0; i < N; i++) {
        /* random a, e, m */
        do { 
            al = rand() % 4 + 1;
            el = rand() % 4 + 1;
            fp256_rand_limbs(&a, al);
            fp256_rand_limbs(&e, el);
            fp256_rand_limbs(&m, 4);
        } while (fp256_is_zero(&a) || fp256_is_zero(&e) || fp256_is_zero(&m) || fp256_is_even(&m));

        /* r = a^e mod m */
        if (fp256_mod_exp(&r, &a, &e, &m) != FP256_OK) {
            printf("fp256_mod_exp_test failed %" PRId64 "\n", i+1);
            test_fp256_print_hex("r = ", &r);
            test_fp256_print_hex("a = ", &a);
            test_fp256_print_hex("e = ", &e);
            test_fp256_print_hex("m = ", &m);
            goto end;
        }

        /* naive mul_sqr algorithm */
        fp256_naive_mod_exp(&tr, &a, &e, &m);

        /* compare */
        if (fp256_cmp(&r, &tr) != 0) {
            printf("fp256_mod_exp_test failed %" PRId64 "\n", i+1);
            test_fp256_print_hex("r  = ", &r);
            test_fp256_print_hex("tr = ", &tr);
            test_fp256_print_hex("a  = ", &a);
            test_fp256_print_hex("e  = ", &e);
            test_fp256_print_hex("m  = ", &m);
            goto end;
        }
    }

    printf("fp256_mod_exp test passed \n");
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

    /* emmmm */
    args.N = 20000;

    RETURN_IF_ERROR(run_test("fp256_mod_exp", fp256_mod_exp_test_vector, fp256_mod_exp_test, args.N, args.T));

    fp256_deinit();
    return 0;
}

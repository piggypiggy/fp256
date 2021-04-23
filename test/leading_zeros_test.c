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

/* alz = number of leading zeros of a */
typedef struct {
    u64 a;
    size_t alz;
} LEADING_ZEROS_TEST_VECTOR;

static LEADING_ZEROS_TEST_VECTOR lz_test_vector[] = {
    /* 1 */
    {
        0,
        64,
    },
    /* 2 */
    {
        0x1,
        63,
    },
    /* 3 */
    {
        0x2,
        62,
    },
    /* 4 */
    {
        0x3,
        62,
    },
    /* 5 */
    {
        0x4,
        61,
    },
    /* 6 */
    {
        0xf,
        60,
    },
    /* 7 */
    {
        0x3f,
        58,
    },
    /* 8 */
    {
        0xf6,
        56,
    },
    /* 9 */
    {
        0x111,
        55,
    },
    /* 10 */
    {
        0x600,
        53,
    },
    /* 11 */
    {
        0x8888,
        48,
    },
    /* 12 */
    {
        0xedcba,
        44,
    },
    /* 13 */
    {
        0x345678,
        42,
    },
    /* 14 */
    {
        0x987654,
        40,
    },
    /* 15 */
    {
        0xfdb9753,
        36,
    },
    /* 16 */
    {
        0x22222222,
        34,
    },
    /* 17 */
    {
        0x776655440,
        29,
    },
    /* 18 */
    {
        0x876543210,
        28,
    },
    /* 19 */
    {
        0x456789abcd,
        25,
    },
    /* 20 */
    {
        0xddddddddddd,
        20,
    },
    /* 21 */
    {
        0x545454545454,
        17,
    },
    /* 22 */
    {
        0x2ffffffffffff,
        14,
    },
    /* 23 */
    {
        0x14285799999999,
        11,
    },
    /* 24 */
    {
        0x314159265358979,
        6,
    },
    /* 25 */
    {
        0x693147180559945,
        5,
    },
    /* 26 */
    {
        0x1414213562373095,
        3,
    },
    /* 27 */
    {
        0x2718281828459045,
        2,
    },
    /* 28 */
    {
        0x7000000000000000,
        1,
    },
    /* 29 */
    {
        0x8765432100abcdef,
        0,
    },
    /* 30 */
    {
        0xffffffffffffffff,
        0,
    },
};

int ll_leading_zeros_test_vector(void)
{
    unsigned int i;
    size_t alz, ab;

    for (i = 0; i < sizeof(lz_test_vector) / sizeof(LEADING_ZEROS_TEST_VECTOR); i++) {
        alz = ll_leading_zeros(lz_test_vector[i].a);
        ab = ll_num_bits(lz_test_vector[i].a);

        if (alz != lz_test_vector[i].alz) {
            printf("ll_leading_zeros_test_vector %d failed\n", i + 1);
            printf("alz : %" PRId64 "\n", alz);
            printf("alz should be: %" PRId64 "\n", lz_test_vector[i].alz);
            return FP256_ERR;
        }

        if (ab != (64 - lz_test_vector[i].alz)) {
            printf("ll_num_bits_test_vector %d failed\n", i + 1);
            printf("bits : %" PRId64 "\n", ab);
            printf("bits should be: %" PRId64 "\n", 64 - lz_test_vector[i].alz);
            return FP256_ERR;
        }
    }

    printf("ll_leading_zeros_test_vector passed\n");
    return FP256_OK;
}

int main(int argc, char **argv)
{
    TEST_ARGS args;

    if (fp256_init() != FP256_OK)
        return -1;

    RETURN_IF_ERROR(get_test_args(argc, argv, &args));
    test_rand_init();

    RETURN_IF_ERROR(run_test("ll_leading_zeros", ll_leading_zeros_test_vector, NULL, args.N, args.T));

    fp256_deinit();
    return 0;
}

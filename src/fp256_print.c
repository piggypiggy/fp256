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

#include <fp256/fp256.h>
#include <fp256/fp256_ll.h>
#include <stdio.h>

/* convert 256 bit number to hex string and print it */
int fp256_print_hex(const fp256 *r)
{
    u8 hex[65];

    if (r == NULL)
        return FP256_ERR;

    fp256_to_hex(hex, r);
    hex[64] = '\0';
    printf("%s\n", hex);
    return FP256_OK;
}

int fp256_print_mont_ctx(const mont_ctx *mctx)
{
    if (mctx == NULL)
        return FP256_ERR;

    printf("w  = %zu\n", mctx->w);
    printf("N  = ");
    fp256_print_hex(&mctx->N);
    printf("RR = ");
    fp256_print_hex(&mctx->RR);
    printf("k0 = %llu\n\n", mctx->k0);

    return FP256_OK;
}

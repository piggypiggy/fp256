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

#include <fp256/fp256_ll.h>
#include "ll_local.h"
#include <stdio.h>

int ll_print_hex(const u64 *ad, size_t rl)
{
    u8 *hex;
    size_t len;

    len = 1 + rl * sizeof(u64) * 2;
    hex = malloc(len);
    hex[len - 1] = '\0';
    /* u64 to hex */
    ll_to_hex(hex, NULL, ad, rl);
    printf("%s\n", hex);

    free(hex);
    return FP256_OK;
}

void print_hex(const char *desp, const u8 *s, size_t slen)
{
    size_t i;

    for(i = 0; i < strlen(desp); i++)
        printf("%c", desp[i]);

    u8 *hex = (u8*)malloc(2*slen);
    /* byte to hex */
    u8_to_hex(hex, s, slen);
    for(i = 0; i < 2*slen; i++)
        printf("%c", hex[i]);
    printf("\n");
    free(hex);
}

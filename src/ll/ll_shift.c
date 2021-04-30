/******************************************************************************
 *                                                                            *
 * Copyright 2020-2021 Jiang Mengshan                                         *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License");            *
 * you may not use this file except in compliance with the License.           *
 * You may obtain ad copy of the License at                                    *
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

#ifndef USE_ASM_SHIFT
u64 ll_lshift(u64 *rd, const u64 *ad, size_t al, size_t n)
{
    u64 tmp;
    u64 *tr;
    size_t i, l, b;

    if (al == 0)
        return 0;

    /* n = 64 * l + b */
    b = n & 0x3f;
    l = n >> 6;
    tr = rd + al + l;

    tmp = 0;

    /* if b = 0, just left shift l limbs */
    if (b == 0) {
        ad += (al - 1);
        rd += (al - 1 + l);
        /* most significant limb is 0 */
        rd[1] = 0;
        /* copy */
        for (i = 0; i < al; i++) {
            rd[0] = ad[0];
            rd--;
            ad--;
        }
    }
    else {
        /* start shifting from the most significant limb */
        ad += (al - 1);
        rd += (al + l);
        /* lshift */
        for (i = 0; i < al; i++) {
            rd[0] = (tmp << b) | (ad[0] >> (64 - b));
            tmp = ad[0];
            rd--;
            ad--;
        }
        rd[0] = tmp << b;
        rd--;
    }

    /* set (l = shift / 64) lower limbs to 0 */
    for(i = 0; i < l; i++) {
        rd[0] = 0;
        rd--;
    }

    /* return most significant limb */
    return tr[0];
}

u64 ll_rshift(u64 *rd, const u64 *ad, size_t al, size_t n)
{
    u64 tmp;
    u64 *tr;
    size_t i, l, b;

    if (al == 0)
        return 0;

    /* n = 64 * l + b */
    b = n & 0x3f;
    l = n >> 6;
    tr = rd + al - l;

    /* result is 0 */
    if (l >= al) {
        tr = rd;
        goto set_zero;
    }

    /* start shifting from the (l + 1)'s limb */
    ad += l;
    tmp = ad[0];

    /* if b = 0, just right shift l limbs */
    if (b == 0) {
        /* copy */
        for (i = 0; i < (al - l); i++) {
            rd[0] = ad[0];
            ad++;
            rd++;
        }
    }
    else {
        /* rshift */
        ad++;
        for (i = 0; i < (al - l - 1); i++) {
            rd[0] = (ad[0] << (64 - b)) | (tmp >> b);
            tmp = ad[0];
            ad++;
            rd++;
        }
        rd[0] = tmp >> b;
        rd++;
    }

set_zero:
    /* set (l = shift / 64) higher limbs to 0 */
    for(i = 0; i < l; i++) {
        rd[0] = 0;
        rd++;
    }

    /* return most significant limb */
    return tr[0];
}
#endif

/******************************************************************************
 *                                                                            *
 * Copyright 2020-2021 Meng-Shan Jiang                                        *
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

#ifndef USE_ASM_SHIFT
u64 ll_lshift(u64 *r, const u64 *a, size_t al, size_t n)
{
    u64 tmp;
    u64 *tr;
    size_t i, l, b;

    if (al == 0)
        return 0;

    b = n & 0x3f;
    l = n >> 6;
    tr = r + al + l;

    tmp = 0;

    if (b == 0) {
        a += (al - 1);
        r += (al - 1 + l);
        r[1] = 0;
        /* copy */
        for (i = 0; i < al; i++) {
            r[0] = a[0];
            r--;
            a--;
        }
    }
    else {
        /* start shifting from the most significant limb */
        a += (al - 1);
        r += (al + l);
        /* lshift */
        for (i = 0; i < al; i++) {
            r[0] = (tmp << b) | (a[0] >> (64 - b));
            tmp = a[0];
            r--;
            a--;
        }
        r[0] = tmp << b;
        r--;
    }

    /* set (shift / 64) lower limbs to 0 */
    for(i = 0; i < l; i++) {
        r[0] = 0;
        r--;
    }

    return tr[0];
}

u64 ll_rshift(u64 *r, const u64 *a, size_t al, size_t n)
{
    u64 tmp;
    u64 *tr;
    size_t i, l, b;

    if (al == 0)
        return 0;

    b = n & 0x3f;
    l = n >> 6;
    tr = r + al - l;

    if (l >= al) {
        tr = r;
        goto set_zero;
    }

    /* start shifting from the (l + 1)'s limb */
    a += l;
    tmp = a[0];

    if (b == 0) {
        /* copy */
        for (i = 0; i < (al - l); i++) {
            r[0] = a[0];
            a++;
            r++;
        }
    }
    else {
        /* rshift */
        a++;
        for (i = 0; i < (al - l - 1); i++) {
            r[0] = (a[0] << (64 - b)) | (tmp >> b);
            tmp = a[0];
            a++;
            r++;
        }
        r[0] = tmp >> b;
        r++;
    }

set_zero:
    /* set (shift / 64) higher limbs to 0 */
    for(i = 0; i < l; i++) {
        r[0] = 0;
        r++;
    }

    return tr[0];
}
#endif

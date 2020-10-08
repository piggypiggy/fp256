#include <fp256/fp256_ll.h>

#ifndef USE_ASM
size_t ll_lshift(u64 *r, const u64 *a, size_t al, size_t n)
{
    u64 tmp;
    u64 *tr;
    size_t i, l, b;

    if (al == 0)
        return 0;

    tr = r;
    b = n & 0x3f;
    l = n >> 6;

    tmp = 0;

    if (b == 0) {
        a += (al - 1);
        r += (al - 1 + l);
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

    return ll_num_limbs(tr, al + l + 1);
}

size_t ll_rshift(u64 *r, const u64 *a, size_t al, size_t n)
{
    u64 tmp;
    u64 *tr;
    size_t i, l, b;

    if (al == 0)
        return 0;

    tr = r;
    b = n & 0x3f;
    l = n >> 6;

    if (l >= al)
        goto set_zero;

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

    return ll_num_limbs(tr, al);
}
#endif

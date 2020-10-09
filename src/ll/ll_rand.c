#include <fp256/fp256_ll.h>

/* fill l = (nbits/64 + 1) limbs with random data, then clear higher bits of ad[l-1] */
int ll_rand_bits(u64 *ad, size_t nbits)
{
    int ret;
    size_t l, b;
    u64 mask;

    if (nbits == 0)
        return FP256_OK;

    l = ((nbits - 1) >> 6) + 1;
    b = nbits & 0x3f;
    mask = (1 << b) - 1;
    if (b == 0)
        mask = (u64)-1;

    ret = ll_rand_buf((unsigned char*)ad, sizeof(u64) * l);
    ad[l - 1] &= mask;

    return ret;
}

int ll_rand_bytes(u64 *ad, size_t nbytes)
{
    return ll_rand_bits(ad, 8 * nbytes);
}

int ll_rand_limbs(u64 *ad, size_t nlimbs)
{
    return ll_rand_bits(ad, 64 * nlimbs);
}

int ll_rand_range(u64 *ad, const u64 *range, size_t rl)
{
    int ret, count;
    size_t nbits;

    ret = FP256_ERR;
    count = 50;
    if (rl == 0)
        return FP256_OK;

    nbits = ll_num_bits(range[rl - 1]) + (rl - 1) * 64;

    while (count > 0) {
        count--;
        if (count < 0) {
            ll_set_zero(ad, rl);
            return FP256_ERR;
        }

        ret = ll_rand_bits(ad, nbits);
        if (ret != FP256_OK)
            return ret;

        if (ll_cmp_limbs(ad, range, rl, rl) < 0)
            return FP256_OK;
    }

    return ret;
}

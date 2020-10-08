#include <fp256/fp256.h>
#include "ll/ll_local.h"

int fp256_has_neon(void)
{
    return cpu_flags.has_neon;
}

int fp256_has_sse2(void)
{
    return cpu_flags.has_sse2;
}

int fp256_has_sse3(void)
{
    return cpu_flags.has_sse3;
}

int fp256_has_avx(void)
{
    return cpu_flags.has_avx;
}

int fp256_has_avx2(void)
{
    return cpu_flags.has_avx2;
}

int fp256_has_bmi2(void)
{
    return cpu_flags.has_bmi2;
}

int fp256_has_adx(void)
{
    return cpu_flags.has_adx;
}

int fp256_has_rdrand(void)
{
    return 0;
    // return cpu_flags.has_rdrand;
}

int fp256_has_rdseed(void)
{
    return cpu_flags.has_rdseed;
}
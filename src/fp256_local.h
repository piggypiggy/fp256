#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/* initialize prng */
int fp256_rand_init(void);

int fp256_get_cpu_flags(void);

#ifdef __cplusplus
}
#endif

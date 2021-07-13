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

CPU_FLAGS cpu_flags;

#ifdef ARCH_X86_64

/* eax = 0x1. see https://www.felixcloutier.com/x86/cpuid#tbl-3-10 */
# define CPUID_ECX_SSE3    0x00000001
# define CPUID_ECX_PCLMUL  0x00000002
# define CPUID_ECX_AVX     0x10000000
# define CPUID_ECX_RDRAND  0x40000000
# define CPUID_EDX_SSE2    0x04000000

/* eax = 0x7, ecx = 0. see https://www.felixcloutier.com/x86/cpuid#tbl-3-8 */
# define CPUID_EBX_AVX2    0x00000020
# define CPUID_EBX_BMI2    0x00000100
# define CPUID_EBX_RDSEED  0x00040000
# define CPUID_EBX_ADX     0x00080000

# ifndef USE_ASM
void ll_cpuid_x86(u32 ieax, u32 iecx, u32 *oeax, u32 *oebx, u32 *oecx, u32 *oedx)
{
    (void) ieax;
    (void) iecx;
    (void) oeax;
    (void) oebx;
    (void) oecx;
    (void) oedx;
    return;
}
# endif

int fp256_get_cpu_flags(void)
{
    unsigned int eax, ebx, ecx, edx;

    eax = ebx = ecx = edx = 0;

    ll_cpuid_x86(1, 0, &eax, &ebx, &ecx, &edx);
    if (ecx & CPUID_ECX_SSE3)
        cpu_flags.has_sse3 = 1;

    if (ecx & CPUID_ECX_AVX)
        cpu_flags.has_avx = 1;

    if (ecx & CPUID_ECX_RDRAND)
        cpu_flags.has_rdrand = 1;

    if (edx & CPUID_EDX_SSE2)
        cpu_flags.has_sse2 = 1;

    ll_cpuid_x86(7, 0, &eax, &ebx, &ecx, &edx);
    if (ebx & CPUID_EBX_AVX2)
        cpu_flags.has_avx2 = 1;

    if (ebx & CPUID_EBX_BMI2)
        cpu_flags.has_bmi2 = 1;

    if (ebx & CPUID_EBX_RDSEED)
        cpu_flags.has_rdseed = 1;

    if (ebx & CPUID_EBX_ADX)
        cpu_flags.has_adx = 1;

    return FP256_OK;
}

#else

// #error "fp256 only supports x86_64 processor at this time."


#endif

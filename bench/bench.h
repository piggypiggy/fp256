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

#pragma once
/* ------------------------------------------------------------------------- */

#if defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))

#include "sys/time.h"

#define BENCH_VARS                                                            \
  uint32_t start_tick_lo, start_tick_hi;                                                     \
  uint32_t ticks_lo, ticks_hi;                                                  \
  int64_t start_time, end_time

#define COUNTER_START()                                                         \
  __asm__ volatile                                                            \
    ("\n        rdtsc"                                                        \
     : "=a" (start_tick_lo), "=d" (start_tick_hi))

#define COUNTER_STOP()                                                          \
  __asm__ volatile                                                            \
    ("\n        rdtsc"                                                        \
     "\n        subl %2, %%eax"                                               \
     "\n        sbbl %3, %%edx"                                               \
     : "=&a" (ticks_lo), "=&d" (ticks_hi)                                     \
     : "g" (start_tick_lo), "g" (start_tick_hi)) 

static inline int64_t gettime_i64()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000UL + tv.tv_usec;
}

#define TIMER_START()                                                          \
    start_time = gettime_i64()

#define TIMER_STOP()                                                          \
    end_time = gettime_i64()

#define TICKS() (ticks_lo + 4294967296UL * ticks_hi)
#define USEC() (end_time - start_time)

/* ------------------------------------------------------------------------- */

#elif defined(_MSC_VER) && (defined(_M_AMD64) || defined(_M_X64))
#include <intrin.h>
#pragma intrinsic(__rdtsc)

#define BENCH_VARS                                                                            \
unsigned __int64 start_ticks, end_ticks;                                                      \
unsigned __int64 start_time, end_time                                                                  

#define COUNTER_START() start_ticks = __rdtsc()                                                   

#define COUNTER_STOP() end_ticks = __rdtsc()                                                     

static inline unsigned __int64 gettime_i64()
{
    FILETIME ft;
    unsigned __int64 ret;
    /* precision : 100 ns */
    GetSystemTimeAsFileTime(&ft);
    ret = (unsigned __int64)ft.dwLowDateTime | ((unsigned __int64)ft.dwHighDateTime) << 32;
    return ret;
}

#define TIMER_START()                                                              \
    start_time = gettime_i64()

#define TIMER_STOP()                                                              \
    end_time = gettime_i64()

//unsigned __int64 __rdtsc(void);
#define TICKS() (end_ticks - start_ticks)
#define USEC() ((end_time - start_time) / 10)

/* ------------------------------------------------------------------------- */

#elif defined(__GNUC__) && defined(ARCH_AARCH64)
#include "sys/time.h"
#define BENCH_VARS               \
    u64 start_ticks, end_ticks;      \
    int64_t start_time, end_time                                                              

#define COUNTER_START()          \
  __asm__ volatile               \
    ("isb  \n"                   \
     "mrs %0, cntvct_el0  \n"    \
     : "=r" (start_ticks))

#define COUNTER_STOP()           \
  __asm__ volatile               \
    ("isb  \n"                   \
     "mrs %0, cntvct_el0  \n"    \
     : "=r" (end_ticks))

static inline int64_t gettime_i64()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000UL + tv.tv_usec;
}

#define TIMER_START()            \
    start_time = gettime_i64()

#define TIMER_STOP()             \
    end_time = gettime_i64()

#define TICKS() (end_ticks - start_ticks)
#define USEC() (end_time - start_time)

/* ------------------------------------------------------------------------- */

#elif defined(__GNUC__) && defined(ARCH_RISCV64)
#include "sys/time.h"
#define BENCH_VARS               \
    u64 start_ticks, end_ticks;      \
    int64_t start_time, end_time                                                              

#define COUNTER_START()          \
  __asm__ volatile               \
    ("rdcycle %0          \n"    \
     : "=r" (start_ticks))

#define COUNTER_STOP()           \
  __asm__ volatile               \
    ("rdcycle %0          \n"    \
     : "=r" (end_ticks))

static inline int64_t gettime_i64()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000UL + tv.tv_usec;
}

#define TIMER_START()            \
    start_time = gettime_i64()

#define TIMER_STOP()             \
    end_time = gettime_i64()

#define TICKS() (end_ticks - start_ticks)
#define USEC() (end_time - start_time)

/* ------------------------------------------------------------------------- */
#else

#error "timer not implemented"

#endif

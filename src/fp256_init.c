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
#include "fp256_local.h"

#ifdef _WIN32
# include <windows.h>
#elif defined(HAVE_PTHREAD)
# include <pthread.h>
#endif

static volatile int initialized;
static volatile int locked;

#ifdef _WIN32

static CRITICAL_SECTION _lock;
static volatile long    _lock_initialized;

static int _crit_init(void)
{
    long status = 0L;

    while ((status = InterlockedCompareExchange(&_lock_initialized,
                                                1L, 0L)) == 1L) {
        Sleep(0);
    }

    switch (status) {
    case 0L:
        InitializeCriticalSection(&_lock);
        return InterlockedExchange(&_lock_initialized, 2L) == 1L ? 0 : -1;
    case 2L:
        return 0;
    default: /* should never be reached */
        return -1;
    }
}

int fp256_crit_enter(void)
{
    if (_crit_init() != 0)
        return -1; /* LCOV_EXCL_LINE */

    EnterCriticalSection(&_lock);
    assert(locked == 0);
    locked = 1;

    return 0;
}

int fp256_crit_leave(void)
{
    if (locked == 0) {
# ifdef EPERM
        errno = EPERM;
# endif
        return -1;
    }
    locked = 0;
    LeaveCriticalSection(&_lock);

    return 0;
}

#elif defined(HAVE_PTHREAD) && !defined(__EMSCRIPTEN__)

static pthread_mutex_t _lock = PTHREAD_MUTEX_INITIALIZER;

int fp256_crit_enter(void)
{
    int ret;

    if ((ret = pthread_mutex_lock(&_lock)) == 0) {
        assert(locked == 0);
        locked = 1;
    }
    return ret;
}

int fp256_crit_leave(void)
{
    if (locked == 0) {
# ifdef EPERM
        errno = EPERM;
# endif
        return -1;
    }
    locked = 0;

    return pthread_mutex_unlock(&_lock);
}

#else

int fp256_crit_enter(void)
{
    return 0;
}

int fp256_crit_leave(void)
{
    return 0;
}

#endif

static int init_globals(void)
{
    return FP256_OK;
}

static void deinit_globals(void)
{
    return;
}

int fp256_init(void)
{
    if (fp256_crit_enter() != 0) 
        return FP256_ERR;

    if (initialized != 0) {
        if (fp256_crit_leave() != 0) 
            return FP256_ERR;
        return FP256_OK;
    }

    if (fp256_rand_init() != FP256_OK)
        abort();

    // TODO : cpuid

    if (init_globals() != FP256_OK) 
        abort();

    initialized = 1;
    if (fp256_crit_leave() != 0)
        return FP256_ERR;

    return FP256_OK;
}

void fp256_deinit(void)
{
    if (fp256_crit_enter() != 0) 
        return;

    if (initialized != 1)
        return;

    deinit_globals();

    initialized = 0;
    fp256_crit_leave();
    return;
}

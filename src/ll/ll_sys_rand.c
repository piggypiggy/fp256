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

#if defined(OS_LINUX)
# if defined(HAVE_SYS_RANDOM_H)
# include <sys/random.h>
#  define HAVE_RANDOM
#   if !defined(HAVE_GETRANDOM)
#    include <sys/syscall.h>
#    include <unistd.h>
#    define getrandom(buf, len, flag) syscall(SYS_getrandom, (buf), (int) (len), (flag))
#   endif

#  ifdef USE_DEV_RANDOM
#   define RANDOM_FLAG GRND_RANDOM
#  else
#   define RANDOM_FLAG GRND_NONBLOCK
#  endif

int ll_rand_buf(u8 *buf, size_t buflen)
{
    ssize_t len;

    len = 0;
    while (buflen > 0) {
        buf += len;
        buflen -= len;
        len = getrandom(buf, buflen, RANDOM_FLAG);

        if (len == -1) {
            if (errno == EAGAIN || errno == EINTR)
                len = 0;
            else
                return FP256_ERR;
        }
    }

    return FP256_OK;
}
# endif
#endif

#if defined(OS_WINDOWS)
# define HAVE_RANDOM
# include <windows.h>
# define RtlGenRandom SystemFunction036
# if !defined(__MINGW64__)
#  pragma comment(lib, "advapi32.lib")
# endif
BOOLEAN NTAPI RtlGenRandom(PVOID RandomBuffer, ULONG RandomBufferLength);

int ll_rand_buf(u8 *buf, size_t buflen)
{
    BOOLEAN ret;

    ret = RtlGenRandom(buf, buflen);
    return (ret == TRUE ? FP256_OK : FP256_ERR);
}
#endif

#if defined(OS_OPENBSD)
# define HAVE_RANDOM
int ll_rand_buf(u8 *buf, size_t buflen)
{
    arc4random_buf(buf, buflen);
    return FP256_OK;
}
#endif

#if defined(OS_IOS) || defined(OS_MACOSX)
# if defined(HAVE_SYS_RANDOM_H) && defined(HAVE_GETENTROPY)
#  define HAVE_RANDOM
#  include <sys/random.h>
int ll_rand_buf(u8 *buf, size_t buflen)
{
    int ret;

    while (buflen > 256) {
        ret = getentropy(buf, 256);
        if (ret == -1)
            return FP256_ERR;
        buf += 256;
        buflen -= 256;
    }

    ret = getentropy(buf, buflen);
    return (ret == 0 ? FP256_OK : FP256_ERR);
}
# endif
#endif

#if !defined HAVE_RANDOM
# include <fcntl.h>
# include <unistd.h>
# include <sys/stat.h>

static int fd;
static int initialized = 0;

/* use /dev/(u)random directly */
int ll_rand_buf(u8 *buf, size_t buflen)
{
    ssize_t n;
    ssize_t readn;

    if (initialized == 0) {
#  ifdef USE_DEV_RANDOM
        fd = open("/dev/random", O_RDONLY);
#  else
        fd = open("/dev/urandom", O_RDONLY);
#  endif
        if (fd == -1) {
            abort();
        }
        initialized = 1;
    }

    readn = 0;
    do {
        n = read(fd, buf, buflen);
        if (n < 0) {
            if (errno == EINTR || errno == EAGAIN)
                continue;
            else
                return FP256_ERR;
        }

        if (n == 0)
            break;

        readn += n;
        buf += n;
    } while ((size_t)readn < buflen);

    return FP256_OK;
}
#endif

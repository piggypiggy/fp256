#include <fp256/fp256_ll.h>

#if defined(OS_LINUX)
# include <sys/random.h>

# ifdef USE_DEV_RANDOM
#  define RANDOM_FLAG GRND_RANDOM
# else
#  define RANDOM_FLAG GRND_NONBLOCK
# endif

int ll_rand_buf(unsigned char *buf, size_t buflen)
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
#endif

#if defined(OS_WINDOWS)
# include <windows.h>
# define RtlGenRandom SystemFunction036
# if !defined(__MINGW64__)
#  pragma comment(lib, "advapi32.lib")
# endif
BOOLEAN NTAPI RtlGenRandom(PVOID RandomBuffer, ULONG RandomBufferLength);

int ll_rand_buf(unsigned char *buf, size_t buflen)
{
    BOOLEAN ret;

    ret = RtlGenRandom(buf, buflen);
    return (ret == TRUE ? FP256_OK : FP256_ERR);
}
#endif

#if defined(OS_OPENBSD)
int ll_rand_buf(unsigned char *buf, size_t buflen)
{
    arc4random_buf(buf, buflen);
    return FP256_OK;
}
#endif

#if defined(OS_IOS) || defined(OS_MACOSX)
# include <sys/random.h>
int ll_rand_buf(unsigned char *buf, size_t buflen)
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
#endif
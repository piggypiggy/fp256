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

#include "test.h"

#ifdef HAVE_PTHREAD

int test_start_n_thread(void*(*func)(void*), TEST_THREAD_DATA *thread_data)
{
    int ret;
    int64_t i;
    int64_t T;
    pthread_t *t;

    ret = FP256_OK;
    T = thread_data->T;
    t = (pthread_t*)malloc(sizeof(pthread_t) * thread_data->T);
    TEST_THREAD_DATA *ttd = calloc(1, T*sizeof(TEST_THREAD_DATA));

    /* copy arguments */
    for (i = 0; i < T; i++)
        memcpy(&ttd[i], thread_data, sizeof(TEST_THREAD_DATA));

    /* start threads */
    for (i = 0; i < T; i++)
        if (pthread_create(&t[i], NULL, func, (void*)&ttd[i])) {
            ret = FP256_ERR;
            goto end;
        }

    for (i = 0; i < T; i++) {
        pthread_join(t[i], NULL);
        if (ttd[i].ok != FP256_OK)
            ret = FP256_ERR;
    }

    ret = FP256_OK;
end:
    free(t);
    return ret;
}

#elif defined HAVE_WIN32_THREAD

int test_start_n_thread(void*(*func)(void*), TEST_THREAD_DATA *thread_data)
{
    int ret;
    int64_t i;
    int64_t T;

    ret = FP256_OK;
    T = thread_data->T;
    HANDLE *thread_handle = (HANDLE*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, T * sizeof(HANDLE));
    TEST_THREAD_DATA *ttd;
    ttd = calloc(1, T * sizeof(TEST_THREAD_DATA));

    for (i = 0; i < T; i++) {
        memcpy(&ttd[i], thread_data, sizeof(TEST_THREAD_DATA));
        thread_handle[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)func, (LPVOID)&ttd[i], 0, NULL);
    }

    for (i = 0; i < T; i++) {
        WaitForSingleObject(thread_handle[i], INFINITE);
        CloseHandle(thread_handle[i]);
        if (ttd[i].ok != FP256_OK)
            ret = FP256_ERR;
    }
    HeapFree(GetProcessHeap(), 0, thread_handle);

    return ret;
}

#else

int test_start_n_thread(void (*func)(void*), TEST_THREAD_DATA *thread_data)
{
    printf("multithread not supported \n");
    return FP256_ERR;
}

#endif

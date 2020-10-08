#include "../test/test.h"
#include "bench.h"

/* number of data used in each bench */
#define NUM 16

/*************************** fp256_add ***************************/
typedef struct {
    fp256 r;
    fp256 a;
    fp256 b;
} FP256_ADD_DATA;

static void* bench_fp256_add_setup(void)
{
    int k;
    FP256_ADD_DATA *data;

    data = malloc(NUM * sizeof(FP256_ADD_DATA));
    /* random a, b */
    for (k = 0; k < NUM; k++) {
        fp256_rand_limbs(&data[k].a, 4, 0);
        fp256_rand_limbs(&data[k].b, 4, 0);
    }

    return data;
}

static void bench_fp256_add_run(void *_data, int64_t N)
{
    int k;
    int64_t i;
    FP256_ADD_DATA *data;

    k = 0;
    data = (FP256_ADD_DATA*)_data;
    for(i = 0; i < N; i++) {
        fp256_add(&data[k].r, &data[k].a, &data[k].b);
        k = (k + 1) % NUM;
    }
}

static void bench_fp256_add_clear(void *data)
{
    free(data);
}

/*************************** fp256_shift ***************************/
typedef struct {
    fp256 r;
    fp256 a;
} FP256_SHIFT_DATA;

static void* bench_fp256_shift_setup(void)
{
    int k;
    FP256_SHIFT_DATA *data;

    data = malloc(NUM * sizeof(FP256_SHIFT_DATA));
    /* random a */
    for (k = 0; k < NUM; k++)
        fp256_rand_limbs(&data[k].a, 4, 0);

    return data;
}

static void bench_fp256_shift_run(void *_data, int64_t N)
{
    int k;
    int64_t i;
    FP256_SHIFT_DATA *data;

    k = 0;
    data = (FP256_SHIFT_DATA*)_data;
    for(i = 0; i < N; i++) {
        fp256_rshift(&data[k].r, &data[k].a, 30);
        k = (k + 1) % NUM;
    }
}

static void bench_fp256_shift_clear(void *data)
{
    free(data);
}

/*************************** fp256_convert ***************************/
typedef struct {
    fp256 r;
    char hex[64];
    size_t hexlen;
} FP256_CONVERT_DATA;

static void* bench_fp256_convert_setup(void)
{
    int k;
    FP256_CONVERT_DATA *data;

    data = malloc(NUM * sizeof(FP256_CONVERT_DATA));
    for (k = 0; k < NUM; k++) {
        data[k].hexlen = rand() % 64;
        test_random_hex_string((u8*)data[k].hex, data[k].hexlen);
    }

    return data;
}

static void bench_fp256_convert_run(void *_data, int64_t N)
{
    int k;
    int64_t i;
    FP256_CONVERT_DATA *data;

    k = 0;
    data = (FP256_CONVERT_DATA*)_data;

    for(i = 0; i < N; i++) {
        fp256_from_hex(&data[k].r, (u8*)data[k].hex, data[k].hexlen);
        k = (k + 1) % NUM;
    }
}

static void bench_fp256_convert_clear(void *data)
{
    free(data);
}

/*************************** fp256_naive_div ***************************/
typedef struct {
    fp256 rem;
    fp256 quo;
    fp256 num;
    fp256 div;
} FP256_DIV_DATA;

static void* bench_fp256_naive_div_setup(void)
{
    int k;
    size_t nl, dl;
    FP256_DIV_DATA *data;

    data = malloc(NUM * sizeof(FP256_DIV_DATA));
    for (k = 0; k < NUM; k++) {
        do {
            nl = 4;
            dl = test_random_number() % 5;
            fp256_rand_limbs(&data[k].num, nl, 0);
            fp256_rand_limbs(&data[k].div, dl, 0);
        } while (fp256_is_zero(&data[k].num) == 1 ||
                 fp256_is_zero(&data[k].div) == 1 ||
                 fp256_cmp(&data[k].num, &data[k].div) < 0);
    }

    return data;
}

static void bench_fp256_naive_div_run(void *_data, int64_t N)
{
    int k;
    int64_t i;
    FP256_DIV_DATA *data;

    k = 0;
    data = (FP256_DIV_DATA*)_data;
    for (i = 0; i < N; i++) {
        fp256_naive_div(&data[k].rem, &data[k].quo, &data[k].num, &data[k].div);
        k = (k + 1) % NUM;
    }
}

static void bench_fp256_naive_div_clear(void *data)
{
    free(data);
}

/*************************** fp256_gcd ***************************/
typedef struct {
    fp256 r;
    fp256 a;
    fp256 b;
} FP256_GCD_DATA;

static void* bench_fp256_gcd_setup(void)
{
    int k;
    FP256_GCD_DATA *data;

    data = malloc(NUM * sizeof(FP256_GCD_DATA));
    /* random a, b */
    for (k = 0; k < NUM; k++) {
        fp256_rand_limbs(&data[k].a, 4, 0);
        fp256_rand_limbs(&data[k].b, 4, 0);
    }

    return data;
}

static void bench_fp256_gcd_run(void *_data, int64_t N)
{
    int k;
    int64_t i;
    FP256_GCD_DATA *data;

    k = 0;
    data = (FP256_GCD_DATA*)_data;
    for (i = 0; i < N; i++) {
        fp256_gcd(&data[k].r, &data[k].a, &data[k].b);
        k = (k + 1) % NUM;
    }
}

static void bench_fp256_gcd_clear(void *data)
{
    free(data);
}

/*************************** fp256_mod_inv ***************************/
typedef struct {
    fp256 inv;
    fp256 a;
    fp256 m;
} FP256_MODINV_DATA;

static void* bench_fp256_mod_inv_setup(void)
{
    int k;
    FP256_MODINV_DATA *data;

    data = malloc(NUM * sizeof(FP256_MODINV_DATA));
    /* random a, m */
    for (k = 0; k < NUM; k++) {
        fp256_rand_limbs(&data[k].a, 4, 0);
        fp256_rand_limbs(&data[k].m, 4, 0);
    }

    return data;
}

static void bench_fp256_mod_inv_run(void *_data, int64_t N)
{
    int k;
    int64_t i;
    FP256_MODINV_DATA *data;

    k = 0;
    data = (FP256_MODINV_DATA*)_data;
    for (i = 0; i < N; i++) {
        fp256_mod_inv(&data[k].inv, &data[k].a, &data[k].m);
        k = (k + 1) % NUM;
    }
}

static void bench_fp256_mod_inv_clear(void *data)
{
    free(data);
}

/*************************** fp256_mul ***************************/
typedef struct {
    fp256 r;
    fp256 a;
    fp256 b;
} FP256_MUL_DATA;

static void* bench_fp256_mul_setup(void)
{
    int k;
    FP256_MUL_DATA *data;

    data = malloc(NUM * sizeof(FP256_MUL_DATA));
    /* random a, b */
    for (k = 0; k < NUM; k++) {
        fp256_rand_limbs(&data[k].a, 4, 0);
        fp256_rand_limbs(&data[k].b, 4, 0);
    }

    return data;
}

static void bench_fp256_mul_run(void *_data, int64_t N)
{
    int k;
    int64_t i;
    FP256_MUL_DATA *data;

    k = 0;
    data = (FP256_MUL_DATA*)_data;
    for (i = 0; i < N; i++) {
        fp256_mul(&data[k].r, &data[k].a, &data[k].b);
        k = (k + 1) % NUM;
    }
}

static void bench_fp256_mul_clear(void *data)
{
    free(data);
}

/*************************** fp256_sqr ***************************/
typedef struct {
    fp256 r;
    fp256 a;
} FP256_SQR_DATA;

static void* bench_fp256_sqr_setup(void)
{
    int k;
    FP256_SQR_DATA *data;

    data = malloc(NUM * sizeof(FP256_SQR_DATA));
    /* random num */
    for (k = 0; k < NUM; k++)
        fp256_rand_limbs(&data[k].a, 4, 0);

    return data;
}

static void bench_fp256_sqr_run(void *_data, int64_t N)
{
    int k;
    int64_t i;
    FP256_SQR_DATA *data;

    k = 0;
    data = (FP256_SQR_DATA*)_data;

    for (i = 0; i < N; i++) {
        fp256_sqr(&data[k].r, &data[k].a);
        k = (k + 1) % NUM;
    }
}

static void bench_fp256_sqr_clear(void *data)
{
    free(data);
}

/*************************** fp256_mod_add ***************************/
typedef struct {
    fp256 r;
    fp256 a;
    fp256 b;
    fp256 m;
} FP256_MOD_ADD_DATA;

static void* bench_fp256_mod_add_setup(void)
{
    int k;
    FP256_MOD_ADD_DATA *data;

    data = malloc(NUM * sizeof(FP256_MOD_ADD_DATA));
    /* random a, b, m */
    for (k = 0; k < NUM; k++) {
        fp256_rand_limbs(&data[k].a, 4, 0);
        fp256_rand_limbs(&data[k].b, 4, 0);
        do {
            fp256_rand_limbs(&data[k].m, 4, 0);
        } while (fp256_is_zero(&data[k].m));
    }

    return data;
}

static void bench_fp256_mod_add_run(void *_data, int64_t N)
{
    int k;
    int64_t i;
    FP256_MOD_ADD_DATA *data;

    k = 0;
    data = (FP256_MOD_ADD_DATA*)_data;
    for (i = 0; i < N; i++) {
        fp256_mod_add(&data[k].r, &data[k].a, &data[k].b, &data[k].m);
        k = (k + 1) % NUM;
    }
}

static void bench_fp256_mod_add_clear(void *data)
{
    free(data);
}

/*************************** fp256_mod_mul ***************************/
typedef struct {
    fp256 r;
    fp256 a;
    fp256 b;
    fp256 m;
}FP256_MOD_MUL_DATA;

static void* bench_fp256_mod_mul_setup(void)
{
    int k;
    FP256_MOD_MUL_DATA *data;

    data = malloc(NUM * sizeof(FP256_MOD_MUL_DATA));
    /* random num */
    for (k = 0; k < NUM; k++) {
        fp256_rand_limbs(&data[k].a, 4, 0);
        fp256_rand_limbs(&data[k].b, 4, 0);
        do {
            fp256_rand_limbs(&data[k].m, 4, 0);
        } while (fp256_is_zero(&data[k].m));
    }

    return data;
}

static void bench_fp256_mod_mul_run(void *_data, int64_t N)
{
    int k;
    int64_t i;
    FP256_MOD_MUL_DATA *data;

    k = 0;
    data = (FP256_MOD_MUL_DATA*)_data;
    for (i = 0; i < N; i++) {
        fp256_mod_mul(&data[k].r, &data[k].a, &data[k].b, &data[k].m);
        k = (k + 1) % NUM;
    }
}

static void bench_fp256_mod_mul_clear(void *data)
{
    free(data);
}

/*************************** fp256_mod_sqr ***************************/
typedef struct {
    fp256 r;
    fp256 a;
    fp256 m;
}FP256_MOD_SQR_DATA;

static void* bench_fp256_mod_sqr_setup(void)
{
    int k;
    FP256_MOD_SQR_DATA *data;

    data = malloc(NUM * sizeof(FP256_MOD_SQR_DATA));
    /* random num */
    for (k = 0; k < NUM; k++) {
        fp256_rand_limbs(&data[k].a, 4, 0);
        do {
            fp256_rand_limbs(&data[k].m, 4, 0);
        } while (fp256_is_zero(&data[k].m));
    }

    return data;
}

static void bench_fp256_mod_sqr_run(void *_data, int64_t N)
{
    int k;
    int64_t i;
    FP256_MOD_SQR_DATA *data;

    k = 0;
    data = (FP256_MOD_SQR_DATA*)_data;
    for (i = 0; i < N; i++) {
        fp256_mod_sqr(&data[k].r, &data[k].a, &data[k].m);
        k = (k + 1) % NUM;
    }
}

static void bench_fp256_mod_sqr_clear(void *data)
{
    free(data);
}

static void* _run_bench(void *thread_data)
{
    TEST_THREAD_DATA *ttd;
    void *data;

    ttd = (TEST_THREAD_DATA*)thread_data;

    /* generate random data */
    data = ttd->setup();

    BENCH_VARS;

    COUNTER_START();
    TIMER_START();

    /* bench */
    ttd->run(data, ttd->N);

    COUNTER_STOP();
    TIMER_STOP();

    ttd->clear(data);

    printf("cycles/op : %" PRId64 "\n", (TICKS()/ttd->N));
    /* TODO : USEC() might be 0 if N is to small */
    printf("op/s      : %" PRId64 "\n", (ttd->N*1000000/USEC()));
    printf("time/op   : %" PRId64 " ns\n\n", USEC()*1000/ttd->N);

    return NULL;
}

static void run_bench(char *name, void* (*setup)(void), void (*run)(void*, int64_t), void (*clear)(void*), int64_t N, int64_t T)
{
    TEST_THREAD_DATA thread_data;

    thread_data.name = name;
    thread_data.setup = setup;
    thread_data.run = run;
    thread_data.clear = clear;
    thread_data.N = N;
    thread_data.T = T;
    thread_data.ok = 1;

    printf("------------ %s ------------ \n", name);

    /* single thread */
    printf("singlethread N = %" PRId64 "\n\n", N);
    _run_bench((void*)&thread_data);

    /* multi thread */
    if (T > 0) {
        printf("multithread T = %" PRId64 "\n\n", T);
        test_start_n_thread(_run_bench, (void*)&thread_data);
    }
}

int main(int argc, char **argv)
{
    TEST_ARGS args;

    fp256_init();
    test_rand_init();

    get_test_args(argc, argv, &args);

    if (args.do_which.do_add)
        run_bench("fp256_add", bench_fp256_add_setup, bench_fp256_add_run, bench_fp256_add_clear, args.N, args.T);

    if (args.do_which.do_convert)
        run_bench("fp256_convert", bench_fp256_convert_setup, bench_fp256_convert_run, bench_fp256_convert_clear, args.N, args.T);

    if (args.do_which.do_naive_div)
        run_bench("fp256_naive_div", bench_fp256_naive_div_setup, bench_fp256_naive_div_run, bench_fp256_naive_div_clear, args.N, args.T);

    if (args.do_which.do_gcd)
        run_bench("fp256_gcd", bench_fp256_gcd_setup, bench_fp256_gcd_run, bench_fp256_gcd_clear, args.N, args.T);

    if (args.do_which.do_mod_inv)
        run_bench("fp256_mod_inv", bench_fp256_mod_inv_setup, bench_fp256_mod_inv_run, bench_fp256_mod_inv_clear, args.N, args.T);

    if (args.do_which.do_mul)
        run_bench("fp256_mul", bench_fp256_mul_setup, bench_fp256_mul_run, bench_fp256_mul_clear, args.N, args.T);

    if (args.do_which.do_shift)
        run_bench("fp256_shift", bench_fp256_shift_setup, bench_fp256_shift_run, bench_fp256_shift_clear, args.N, args.T);

    if (args.do_which.do_sqr)
        run_bench("fp256_sqr", bench_fp256_sqr_setup, bench_fp256_sqr_run, bench_fp256_sqr_clear, args.N, args.T);

    if (args.do_which.do_mod_add)
        run_bench("fp256_mod_add", bench_fp256_mod_add_setup, bench_fp256_mod_add_run, bench_fp256_mod_add_clear, args.N, args.T);

    if (args.do_which.do_mod_mul)
        run_bench("fp256_mod_mul", bench_fp256_mod_mul_setup, bench_fp256_mod_mul_run, bench_fp256_mod_mul_clear, args.N, args.T);

    if (args.do_which.do_mod_sqr)
        run_bench("fp256_mod_sqr", bench_fp256_mod_sqr_setup, bench_fp256_mod_sqr_run, bench_fp256_mod_sqr_clear, args.N, args.T);

    fp256_deinit();
    return 0;
}
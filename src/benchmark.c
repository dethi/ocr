#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>

double T_START;
double T_END;
double T_RESULT;

double get_time()
{
    struct timeval t;
    //struct timezone tz;
    gettimeofday(&t, NULL);
    return t.tv_sec + t.tv_usec * 1e-6;
}

void bench_start()
{
    T_START = get_time();
}

void bench_end()
{
    T_END = get_time();
    T_RESULT = T_END - T_START;
}

void bench_time()
{
    if (T_RESULT <= 0)
        return;

    printf("Elapsed time: %.3f ms\n", T_RESULT * 1e3);
}

void bench_write_mem(int n_byte)
{
    if (T_RESULT <= 0)
        return;

    double speed = (n_byte / T_RESULT) * 1e-6;
    printf("Writing speed: %.3f MB/s\n", speed);
}

void bench_read_mem(int n_byte)
{
    if (T_RESULT == 0)
        return;

    double speed = (n_byte / T_RESULT) * 1e-6;
    printf("Reading speed: %f MB/s\n", speed);
}

#ifndef _BENCHMARK_H_
#define _BENCHMARK_H_

double get_time();
void bench_start();
void bench_end();
void bench_time();
void bench_write_mem(int n_byte);
void bench_read_mem(int n_byte);

#endif

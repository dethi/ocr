#ifndef HEADER_BENCHMARK_H
#define HEADER_BENCHMARK_H

double get_time();
void bench_start();
void bench_end();
void bench_time();
void bench_write_mem(int n_byte);
void bench_read_mem(int n_byte);

#endif

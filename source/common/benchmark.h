#ifndef BENCHMARK_H
#define BENCHMARK_H

//forward declaration
struct Argument;

typedef double bench_time;

typedef struct Benchmark{
  //start time before benchmarking
  bench_time start;

  //start time of i'th benchmark
  bench_time single_start;

  //minimum/maximum time of benchmark
  bench_time min;
  bench_time max;

  //total time of benchmarking
  bench_time sum;

  //squared time for benchmarking
  bench_time squared_sum;
}Benchmark;

//get current time
bench_time now();

//initialize the benchmark
void setup_benchmark(Benchmark *bench);

//record and update the value for each benchmark
void benchmark(Benchmark *bench);

//give the final result of benchmark
void evaluate(Benchmark *bench, struct Argument *arg);

#endif /* BENCHMARK_H */


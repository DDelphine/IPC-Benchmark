#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "argument_parse.h"
#include "benchmark.h"

bench_time now(){
//#ifdef __MACH__
//  return ((double) clock()) / CLOCK_PER_SEC * 1e9;
//#else
  struct timespec start;
  clock_gettime(CLOCK_MONOTONIC, &start);
  return (double)(start.tv_sec)*1e9 + start.tv_nsec;
//#endif
}

void setup_benchmark(Benchmark *bench){
  bench->min = INT32_MAX;
  bench->max = 0;
  bench->sum = 0;
  bench->squared_sum = 0;
  bench->start = now();
}

void benchmark(Benchmark *bench){
  const bench_time time = now() - bench->single_start;

  if(time < bench->min)
    bench->min = time;

  if(time > bench->max)
    bench->max = time;
  
  bench->sum += time;
  bench->squared_sum += (time*time);
}

void evaluate(Benchmark *bench, Argument *arg){
  //get the cumulative time for reading and writing back message for 'arg->count' times
  const bench_time total_time = now() - bench->start;

  const double average = ((double)bench->sum) / arg->count;

  double sigma = bench->squared_sum / arg->count;
  sigma = sqrt(sigma - (average * average));

  int messageRate = (int)(arg->count * arg->size / (total_time / 1e9));

  printf("\n*************benchmark results****************\n");
  printf("Message size:           %d\n", arg->size);
  printf("Counter:                %d\n", arg->count);
  printf("Total time:             %.3f us\n", total_time);
  printf("Average time:           %.3f us\n", average);
  printf("Minimum time:           %.3f us\n", bench->min);
  printf("Maximum time:           %.3f us\n", bench->max);
  printf("Message rate:           %.3d Byte/s\n", messageRate);
  printf("\n**********************************************\n");
}

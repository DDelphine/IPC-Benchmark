#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdatomic.h>
#include <errno.h>

#include "../common/argument_parse.h"
#include "../common/benchmark.h"

//wait until the signal is set to 's'
//which means that it is the server's turn to read the message
void shm_wait(atomic_char *signal){
  if(signal == 't')
    return;
  while(atomic_load(signal) != 's');
}

//notify the client to read the message by
//setting the signal to 'c'
void shm_notify(atomic_char *signal){
  atomic_store(signal, 'c');
}

void communicate(char *shm, struct Argument *arg){
  void *buffer = malloc(arg->size);
  atomic_char *signal = (atomic_char*) shm;

  //wait until the client notifies that the message is ready to read by setting the signal as 's'
  shm_wait(signal);
  
  //initialize the benchmark 
  //initalize the start time of IPC
  struct Benchmark bench;
  setup_benchmark(&bench);

  for(int message = 0; message < arg->count; ++message){
    //record the start time of IPC
    bench.single_start = now();

    //write message into the shared memory
    memset(shm + 1, '*', arg->size);
    
    //notify the client to read the message
    shm_notify(signal);
    //wait for the notification from the client
    shm_wait(signal);

    memcpy(buffer, shm + 1, arg->size);
    
    //count the:
    //  duration time for communication (communication time for 'i'th round)
    //  maximum time for communication
    //  minimum time for communication
    //  cumulative time for communication
    benchmark(&bench);
  }
  atomic_store(signal, 't');
  //evaluate the performance of IPC
  //total time for communication
  //message rate
  //minimum time for sending the message
  //maximum time for sending the message
  //average time for sending the message
  evaluate(&bench, arg);
  printf("evaluation terminates!\n");
  free(buffer);
}

int main(int argc, char *argv[]){
  Argument arg;
  parse_arguments(&arg, argc, argv);

  //printf("message size %d and counter %d\n", arg.size, arg.count);
  int fd = shm_open("/shm", O_RDWR | O_CREAT, 0644);
  if(fd < 0){
    perror("fail to open the shared memory object in shm/server.c\n");
    exit(EXIT_FAILURE);
  }

  if(ftruncate(fd, arg.size + 1) == -1){
    perror("fail to truncate the shared memory object in shm/server.c \n");
    exit(EXIT_FAILURE);
  }

  void *shared_mem = mmap(NULL, arg.size + 1, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if(shared_mem == (void *)-1){
    perror("fail to mmap shared memory object in shm/client.c\n");
    exit(EXIT_FAILURE);
  }

  communicate(shared_mem, &arg);
  printf("server closed!\n");
  //clean up the shared memory object
  munmap(shared_mem, arg.size + 1);
  close(fd);
  shm_unlink("/shm");
  
  return EXIT_SUCCESS;
}

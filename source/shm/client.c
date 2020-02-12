//
//this file is the client.c program
//which receive the message from the server 
//
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

void shm_wait(atomic_char *signal){
  //keep waiting until server notifies the client that:
  //message is ready to be read
  //the server notifies the client by setting the signal to 'c'
  if(signal == 't')
    return;
  while(atomic_load(signal) != 'c');
}

void shm_notify(atomic_char *signal){
  //by setting the signal to 's', notifying the server that: 
  //hi, this is your turn to read the message
  atomic_store(signal, 's');
}

void communicate(char *shm, struct Argument *arg){
  void *buffer = malloc(arg->size);
  
  //set the first byte of shared memory to be signal
  atomic_char *signal = (atomic_char *)shm;
  
  //initialize the signal to be 's', making the client wait until the server notifies it
  atomic_init(signal, 's');

  for(int message = 0; message < arg->count; ++message){
    shm_wait(signal);
    //read
    memcpy(buffer, shm + 1, arg->size);

    //write
    memset(shm + 1, '*', arg->size);
    
    //after write back the message
    //notify the server to read it
    //then start another round
    shm_notify(signal);
  }

  free(buffer);
}

int main(int argc, char *argv[]){
  //parsing the commanline argument to get the message size and execution number
  Argument arg;
  parse_arguments(&arg, argc, argv);
  //printf("message size %d and counter %d\n", arg.size, arg.count);
  int fd = shm_open("/shm", O_RDWR | O_CREAT, 0644);
  if(fd < 0){
    perror("fail to open the shared memory object in shm/client.c \n");
    exit(EXIT_FAILURE);
  }

  //truncate the shared memory region to a size of (arg->size+1)
  //the extra 1 byte is used for signaling
  if(ftruncate(fd, arg.size + 1) == -1){
    perror("fail to truncate the shared memory object in shm/client.c \n");
    exit(EXIT_FAILURE);
  }

  void *shared_mem = mmap(NULL, arg.size + 1, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if(shared_mem == (void *)-1){
    perror("fail to mmap shared memory object in shm/client.c\n");
    exit(EXIT_FAILURE);
  }

  communicate(shared_mem, &arg);
  printf("client closed!\n");
  //clean up the shared memory object
  munmap(shared_mem, arg.size+1);
  close(fd);
  shm_unlink("/shm");

  return EXIT_SUCCESS;
}

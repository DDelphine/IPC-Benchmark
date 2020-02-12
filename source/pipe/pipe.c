#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include "../common/argument_parse.h"
#include "../common/benchmark.h"
#include "../common/process.h"
#include "../common/signals.h"

FILE *open_stream(int pipefd[2], int to_open){
  FILE *stream;
  char mode[1];
  //to_open = 0 -> r
  //to_open = 1 -> w
  mode[0] = to_open ? 'w' : 'r';
  
  //when using the read end, close the unused write end
  //when using the write end, close the unused read end
  close(pipefd[1-to_open]);

  if((stream = fdopen(pipefd[to_open], mode)) == NULL){
    perror("fail to open stream in pipe.c\n");
  }

  return stream;
}

void client_communicate(int pipefd[2], struct Argument *args){
  struct sigaction sig;
  FILE *stream;
  void *buffer;

  stream = open_stream(pipefd, 0);
  setup_client_signals(&sig);
  buffer = malloc(args->size);

  //after fork, client process starts firstly
  //it should notify the server by sending SIGUSR1
  notify_server();

  for(; args->count > 0; --args->count){
    //wait until the server calls notify_client
    wait_for_signal(&sig);
    
    //read args->size bytes of data from buffer, then save them to stream 
    if(fread(buffer, args->size, 1, stream) == -1){
       perror("Error reading from pipe in pipe.c\n");
    }

    notify_server();
  }

  //close the write end
  close(pipefd[1]);
  free(buffer);
}

void server_communicate(int pipefd[2], struct Argument *args){
  struct sigaction sig;
  setup_server_signals(&sig);

  //open the stream for writing message
  FILE *stream;
  stream = open_stream(pipefd, 1);

  void *buffer;
  buffer = malloc(args->size);

  struct Benchmark bench;
  setup_benchmark(&bench);

  //wait until the client send the signal SIGUSR1
  wait_for_signal(&sig);

  for(int message = 0; message < args->count; ++message){
    //record the start time of IPC
    bench.single_start = now();
    
    //write args->size bytes from stream to buffer
    if(fwrite(buffer, args->size, 1, stream) == -1){
      perror("fail to write message in pipe.c\n");
    }

    //flush the buffered data into stream so that the client can read it immediately.
    fflush(stream);

    notify_client();
    wait_for_signal(&sig);
    benchmark(&bench);
  }
  evaluate(&bench, args);
  
  close(pipefd[1]);
  free(buffer);
}

void communicate(int pipefd[2], struct Argument *args){
  pid_t pid;
  
  //pipe can only be used between related process
  pid = fork();

  if(pid == -1){
    perror("fail to fork child process in pipe.c\n");
    exit(EXIT_FAILURE);
  }

  //child process
  if(pid == 0){
    client_communicate(pipefd, args);
  }

  else{
    server_communicate(pipefd, args);
  }
}

int main(int argc, char *argv[]){
  //two pipe file descriptors
  //pipefd[0] refers to the read end of the pipe
  //pipefd[1] refers to the write end of the pipe
  int pipefd[2];

  struct Argument args;

  parse_arguments(&args, argc, argv);

  if(pipe(pipefd)){
    perror("fail to open pipe in pipe.c\n");
  }

  communicate(pipefd, &args);

  return EXIT_SUCCESS;
}

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "../common/argument_parse.h"
#include "../common/benchmark.h"
#include "../common/process.h"
#include "../common/signals.h"

#define SOCKET_PATH "/tmp/socket"

int setup_server(){
  int fd;
  //create an endpoint of communication for server and returns a file descriptor
  //use AF_UNIX to specify the local communication
  //use SOCK_STREAM for sequenced, reliable, two-way, connection-based, byte-stream-based communication.
  //use 0 to specify the particular protocal supports the socket type
  if((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1){
    printf("fail to create a file descriptor for server in socket/server.c at line: %d\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  strcpy(addr.sun_path, SOCKET_PATH);
  //remove the socket if it exists
  remove(addr.sun_path);

  //bind the socket to this file
  if(bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1){
    printf("fail to bind the socket to address in socket/server.c at line: %d\n", __LINE__);
    exit(EXIT_FAILURE);
  }
  
  //start to listen on this socket
  if(listen(fd, 10) == -1){
    printf("fail to listen to the socket in socket/server.c at line: %d\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  return fd;
}

int accept_connection(int fd, int busy_wait){
  struct sockaddr_un client;
  int connection;

  connection = accept(fd, (struct sockaddr *)&client, &(socklen_t)(sizeof(client)));

  if(connection  == -1){
    printf("error in building connection to server in socket/server.c at line: %d\n", __LINE__);
    exit(EXIT_FAILURE);
  }


}

int main(int argc, char *argv[]){
  //file descriptor for server and client
  int server, client;
  
  int busy_wait;

  struct Argument arg;
  parse_arguments(&arg, argc, argv);

  int busy_wait = check_flag("busy", argc, argv);

  //build a listening socket for server
  server = setup_server();
   
  //accept connection request from client
  connection = accept_connection(server, busy_wait);

  communicate(connection, &arg, busy_wait);

  return EXIT_SUCCESS;
}

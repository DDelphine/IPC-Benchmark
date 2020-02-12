//
//this file starts a server process and a client process
//

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <wait.h>
#include "process.h"

#define BUILD_PATH "/build/source\0"

char *find_path(){
  char path[100];
  //get current directory path, which shoule be
  //PATH_TO_IPCBENCH/source/common
  getcwd(path, sizeof(path));
  char *right = path;
  char *left = right+5;
  //Using an O(N) comparison to get rid of 'common'
  //i.e., change current directory to 
  //PATH_TO_IPCBENCH/source
  while((*right != '\0') && (*left != '\0')){
    if((*right == 'c') && (*left == 'n')){
      if(strncmp(right, "common", 6) == 0)
	break;
    }
    right++;
    left++;
  }
  char *new_path = (char*)malloc(100);
  strncpy(new_path, path, (right - path + 2));
  
  return new_path;
}

void start_child(char *argv[]){
  const pid_t parent_pid = getpid();
  const pid_t pid = fork();
  int stat;

  //printf("program path is: %s\n", argv[0]);

  if(pid == 0){
    //set child process id to the parent group id, 
    //so that they can send around signals to each other
    if(setpgid(pid, parent_pid) == -1){
      perror("fail to add child process to parent group in process.c\n");
      exit(EXIT_FAILURE);
    }
    if((stat = execv(argv[0], argv)) == -1){
      perror("fail to start process in process.c\n");
      exit(EXIT_FAILURE);
    }
  }
  return;
  //int stat;
  //pid_t wpid;
  //while((wpid = wait(&stat)) > 0);
  //exit(EXIT_SUCCESS);
}

void environ_arguments(char *arguments[], int argc, char *argv[]){
  assert(argc < 8);

  for(int i = 1; i < argc; i++){
    arguments[i] = argv[i];
  }
  arguments[argc] = NULL;
}

void start_process(char *path, int argc, char *argv[]){
  char *args[8] = {path};
  environ_arguments(args, argc, argv);
  start_child(args);
}

void start_server_client(char *prefix, int argc, char *argv[]){
  char server[100];
  char client[100];
  
  //find the path to executable server/client file
  char *path = find_path();

  //server executable file path
  //e.g., /home/user/ipc-bench/source/shm/server
  sprintf(server, "%s%s/%s", path, prefix, "server");

  //client executable file path
  sprintf(client, "%s%s/%s", path, prefix, "client");
 
  start_process(server, argc, argv);
  start_process(client, argc, argv);
  
  free(path);  
  int stat;
  pid_t wpid;
  while((wpid = wait(&stat)) > 0);
  exit(EXIT_SUCCESS);
}



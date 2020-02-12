#include "../common/process.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
  start_server_client("shm", argc, argv);
}

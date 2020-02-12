//
//this file is used for parsing the arguments passed in
//

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "argument_parse.h"

void parse_arguments(Argument *arguments, int argc, char *argv[]){
  int long_index = 0;
  int option;

  //optind is the index of the next element to be processed in argv
  //optind = 0;

  arguments->size = DEFAULT_MESSAGE_SIZE;
  arguments->count = DEFAULT_COUNT_NUMBER;

  static struct option long_options[] = {
    {"size", required_argument, NULL, 's'},
    {"count", required_argument, NULL, 'c'},
    {0, 0, 0, 0}
  };

  while(1){
    //return the option character when finding one
    //int getopt_long(int argc, char *const argv[], const char *optstring,
    //                const struct option *longopts, int *longindex);
    option = getopt_long(argc, argv, "s:c:", long_options, &long_index);

    switch (option){
      //parsed all the arguments
      case -1: 
	return;
      case 's': 
	arguments->size = atoi(optarg);
	break;
      case 'c':
	arguments->count = atoi(optarg);
	break;
      default:
	continue;
    }
  }
}



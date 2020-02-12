#ifndef PARSE_ARGUMENTS_H
#define PARSE_ARGUMENTS_H

#define DEFAULT_MESSAGE_SIZE 4096
#define DEFAULT_COUNT_NUMBER 1000

typedef struct Argument{
  int size;
  int count;
} Argument;

void parse_arguments(Argument *arguments, int argc, char *argv[]);

#endif /* PARSE_ARGUMENTS_H */

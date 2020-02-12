#ifndef PROCESS_H
#define PROCESS_H

char *find_path();

void start_process(char *path, int argc, char *argv[]);

void start_child(char *argv[]);

void environ_arguments(char *arguments[], int argc, char *argv[]);

void start_server_client(char *prefix, int argc, char *argv[]);

#endif /* PROCESS_H */

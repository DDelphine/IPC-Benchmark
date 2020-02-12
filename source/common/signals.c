#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#include "signals.h"

//set signal handler for SIGUSR1 and SIGUSR2
void signal_handler(int sig){}

void setup_ignored_signals(struct sigaction *sig, int flag){
  sig->sa_handler = signal_handler;
  //if this condition is true, which means this is client's flag
  //client needs to ignore SIGUSR2
  //only set signal handler for server who should catch SIGUSR1
  if(!(flag & BLOCK_USR1)){
    //set signal handler for signal SIGUSR1
    if(sigaction(SIGUSR1, sig, NULL)){
      perror("error registering SIGUSR1 signal handler for server in signals.c\n");
    }
  }
  //if this condition is true, which means this is server's flag
  //server needs to ignore SIGUSR1
  //only set signal handler for client who should catch SIGUSR2, 
  if(!(flag & BLOCK_USR2)){
    //set signal handler for signal SIGUSR2
    if(sigaction(SIGUSR2, sig, NULL)){
      perror("error registering SIGUSR2 signal handler for server in signals.c\n");
    }
  }
}

void setup_blocked_signals(struct sigaction *sig, int flag){
  sig->sa_handler = SIG_DFL;
  
  //set block signal for server
  //block SIGUSR1 until the signal handler returns
  if(flag & BLOCK_USR1){
    sigaddset(&sig->sa_mask, SIGUSR1);
  }

  //set block signal for client
  //block SIGUSR2 until the signal handler returns
  if(flag & BLOCK_USR2){
    sigaddset(&sig->sa_mask, SIGUSR2);
  }
  
  //add the sig->sa_mask to block set
  sigprocmask(SIG_BLOCK, &sig->sa_mask, NULL);
}

void setup_signals(struct sigaction *sig, int flag){
  //set this flag, so when returning from a handler, 
  //the interrupted syscall can be resumed
  //if this flag is not set, returning from a handler makes interrupted syscall fail
  sig->sa_flags = SA_RESTART;

  //clear the mask of signals which should be blocked during the 
  //execution of current signal handler
  sigemptyset(&sig->sa_mask);

  //set the signals that should be ignored
  setup_ignored_signals(sig, flag);

  //clear the mask of signals which should be ignored during the 
  //execution of current signal handler
  sigemptyset(&sig->sa_mask);

  //set the signals that should be blocked
  setup_blocked_signals(sig, flag);
}

void setup_parent_signals(){
  struct sigaction sig;
  setup_signals(&sig, IGNORE_USR1 | IGNORE_USR2);
}

void setup_server_signals(struct sigaction *sig){
  //server needs to catch SIGUSR1 signal, ignore SIGUSR2 signal
  setup_signals(sig, BLOCK_USR1 | IGNORE_USR2);
  usleep(1000);
}

void setup_client_signals(struct sigaction *sig){
  //client needs to catch SIGUSR2 signal, ignore SIGUSR1 signal
  setup_signals(sig, BLOCK_USR2 | IGNORE_USR1);
  usleep(1000);
}

//send signal SIGUSR1 to notify the server
void notify_server(){
  kill(0, SIGUSR1);
}

void notify_client(){
  kill(0, SIGUSR2);
}

void wait_for_signal(struct sigaction *sig){
  int signum;
  //wait until signals specified in sa_mask become pending
  //for server, wait until receiving SIGUSR1
  //for client, wait until receiving SIGUSR2
  sigwait(&(sig->sa_mask), &signum);
}


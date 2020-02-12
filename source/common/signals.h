#ifndef SIGNALS_H
#define SIGNALS_H

#include<signal.h>

#define IGNORE_USR1 0x0
#define IGNORE_USR2 0x0
#define BLOCK_USR1 0x1
#define BLOCK_USR2 0x2

#define WAIT 0x0
#define NOTIFY 0x1

struct sigaction;

void setup_signals(struct sigaction *sig, int flag);

void setup_parent_signals();

void setup_server_signals(struct sigaction *sig);

void setup_client_signals(struct sigaction *sig);

void notify_server();

void notify_client();

void wait_for_signal(struct sigaction *sig);

#endif /* SIGNALS_H */

#include <signal.h>
#include <stdio.h>

#include "signal_handler.h"

void sig_handler(int signo) {
    *daemonise = 0;
    printf("got the signal");
}

int setup_signals(){
    memset(action, 0, sizeof(struct sigaction));
    (*action).sa_handler = sig_handler;
    sigaction(SIGTERM, action, NULL);
    sigaction(SIGINT, action, NULL);
}
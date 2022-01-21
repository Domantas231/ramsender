#include <signal.h>
#include <stdio.h>

#include "signal_handler.h"

/* Signal handler - to support CTRL-C to quit */
void sigHandler(int signo) {
    signal(SIGINT, NULL);
    fprintf(stdout, "Received signal: %d\n", signo);
}

/* Set up signal handlers */
int setup_signals(){
    signal(SIGINT, sigHandler);
    signal(SIGTERM, sigHandler);
}
#include <signal.h>

volatile sig_atomic_t *daemonise;
*daemonise = 1;

struct sigaction *action;

int setup_signals();
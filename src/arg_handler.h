#ifndef ARG_HANDLER_H
#define ARG_HANDLER_H

#include <argp.h>

/* an arbitrary number, enough to fit long options */
#define N 30

struct arguments {
    char args[4][N];
};

extern const char doc[];
extern const char args_doc[];

void set_default_values(struct arguments *args);
error_t parse_opt (int key, char *arg, struct argp_state *state);

#endif
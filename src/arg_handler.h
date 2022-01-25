#ifndef ARG_HANDLER_H
#define ARG_HANDLER_H

#include <argp.h>

/* an arbitrary number, enough to fit long options */
#define N 30

struct arguments {
    char orgId[N];
    char typeId[N];
    char deviceId[N];
    char token[N];
};

extern const char doc[];
extern const char args_doc[];
extern struct argp_option options[];

error_t parse_opt (int key, char *arg, struct argp_state *state);

#endif
#include <stdio.h>
#include "arg_handler.h"

#define CONF_FD "conn_config.yaml"

error_t parse_opt (int key, char *arg, struct argp_state *state){
  /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
    struct arguments *args = state->input;

    switch (key)
    {
    case 'v':
        args->verbose = 1;
        break;

    case ARGP_KEY_ARG:
        if (state->arg_num >= 4)
            /* Too many arguments. */
            argp_usage (state);

        args->args[state->arg_num] = arg;
        break;

    case ARGP_KEY_END:
        if (state->arg_num < 4)
        /* Not enough arguments. */
            argp_usage (state);
        break;

    default:
        return 0;
    }

    return 0;
}

/* 
 * Create a configuration file
 * (.yml type, because the library can't read anything else)
 */
int create_conf_file(struct arguments *args){
    FILE *fptr;

    fptr = fopen(CONF_FD, "w");

    fprintf(fptr, "identity: \n");
    fprintf(fptr, "  orgId: %s\n", args->args[0]);
    fprintf(fptr, "  typeId: %s\n", args->args[1]);
    fprintf(fptr, "  deviceId: %s\n\n", args->args[2]);

    fprintf(fptr, "auth: \n");
    fprintf(fptr, "  token: %s\n", args->args[3]);

    fclose(fptr);

    return 0;
}
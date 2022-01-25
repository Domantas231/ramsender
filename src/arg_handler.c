#include <stdio.h>
#include <string.h>
#include "arg_handler.h"

const char doc[] = "A simple program that connects to the IBM Watson cloud "
                          "and sends data about the router's memory every ten seconds. \n";

const char args_doc[] = "orgID [STRING], typeID [STRING], deviceID [STRING], authToken [STRING]";

error_t parse_opt (int key, char *arg, struct argp_state *state){
  /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
    struct arguments *args = state->input;

    switch (key)
    {
    case ARGP_KEY_ARG:
        if (state->arg_num >= 4)
            /* Too many arguments. */
            argp_usage (state);

        strncpy(args->args[state->arg_num], arg, N);
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

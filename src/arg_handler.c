#include <stdio.h>
#include <string.h>
#include "arg_handler.h"

const struct argp_option options[] = {
    {"domain", 'd', "DOMAIN", 0, "Specify the domain to connect to"},
    {"loglevel", 'l', "LEVEL", 0, "Specify the log level"},
    {"mqtt-port", 'p', "PORT", 0, "Specify a domain port"},
    {"mqtt-transport", 't', "PROTOCOL", 0, "Specify a transfer protocol"},
    {"mqtt-clean", 'c', 0, 0, "Use to specify the program to clean start"},
    {"mqtt-expiry", 'e', "TIME", 0, "Specify a time (in seconds) when a session should expire"},
    {"mqtt-keep", 'k', "TIME", 0, "Specify for how long should the mqtt be kept alive"},
    {"mqtt-file", 'f', "FILE", 0, "Specify a path for the certificate authority file"},
    { 0 }
};

const char doc[] = "A simple program that connects to the IBM Watson cloud "
                          "and sends data about the router's memory every ten seconds. \n";

const char args_doc[] = "orgID [STRING], typeID [STRING], deviceID [STRING], authToken [STRING]";

error_t parse_opt (int key, char *arg, struct argp_state *state){
  /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
    struct arguments *args = state->input;

    switch (key)
    {
    case 'd':
        strncpy(args->domain, arg, N);
        break;

    case 'l':
        strncpy(args->log_level, arg, N);
        break;
    
    case 'p':
        args->port = atoi(arg);
        break;
    
    case 't':
        strncpy(args->protocol, arg, N);
        break;
    
    case 'c':
        args->clean_start = 1;
        break;

    case 'e':
        args->session_expiry = atoi(arg);
        break;

    case 'k':
        args->keep_alive = atoi(arg);
        break;

    case 'f':
        strncpy(args->ca_file, arg, N);
        break;

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

void set_default_values(struct arguments *args){
    /* 
     * set first bytes to 0
     * in order to check whether
     * it is empty later
     */

    args->domain[0] = 0;
    args->ca_file[0] = 0;
    args->protocol[0] = 0;
    args->log_level[0] = 0;

    args->port = 0;
    args->clean_start = 0;
    args->session_expiry = 0;
    args->keep_alive = 0;
}

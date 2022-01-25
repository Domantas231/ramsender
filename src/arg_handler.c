#include <stdio.h>
#include <string.h>
#include "arg_handler.h"

const char doc[] = "A simple program that connects to the IBM Watson cloud "
                          "and sends data about the router's memory every ten seconds. \n";

const char args_doc[] = "orgID [STRING], typeID [STRING], deviceID [STRING], authToken [STRING]";

struct argp_option options[] = {
  {"orgId", 'o', "STRING", 0, "Specify organisation ID" },
  {"typeId", 't', "STRING", 0, "Specify device type ID" },
  {"deviceId", 'd', "STRING", 0, "Specify device ID" },
  {"auth", 'a', "STRING", 0, "Provide the authentication token" },
  { 0 }
};

error_t parse_opt (int key, char *arg, struct argp_state *state){
  /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
    struct arguments *args = state->input;

    switch (key)
    {
    case 'o':
        strncpy(args->orgId, arg, N);
        break;

    case 't':
        strncpy(args->typeId, arg, N);
        break;

    case 'd':
        strncpy(args->deviceId, arg, N);
        break;

    case 'a':
        strncpy(args->token, arg, N);
        break;

    default:
        return 0;
    }

    return 0;
}

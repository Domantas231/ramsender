#include <argp.h>

struct arguments {
    char *args[4];

    int verbose;
    int create_conf;
    char *config_file;
};

// TODO: Add better descriptions of the options
// and change some of the option characters, to more applicable ones

static const struct argp_option options[] = {
    {"verbose", 'v', 0, 0, "Print program logs to the current environment \n"},
    {"domain", 'd', 0, 0, "Specify an options domain \n"},
    {"log level", 'l', 0, 0, "Specify a log level \n"},
    {"MQTT port", 'p', 0, 0, "Specify a port"},
    {"MQTT transport", 't', 0, 0, "Specify a transport"},
    {"MQTT cafile", 'c', 0, 0, "Specify a cafile"},
    {"MQTT clean start", 'x', 0, 0, "Specify a port of the MQTT protocol"},
    {"MQTT session expiry", 'y', 0, 0, "Session expiry"},
    {"MQTT keep alive", 'k', 0, 0, "To keep alive or not to keep"},
    { 0 }
};

static const char doc[] = "A simple program that connects to the IBM Watson cloud "
                          "and sends data about the router's memory every second. \n";

static const char args_doc[] = "orgID [STRING], typeID [STRING], deviceID [STRING], authToken [STRING]";

int create_conf_file(struct arguments *args);
error_t parse_opt (int key, char *arg, struct argp_state *state);
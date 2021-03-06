#include <stdlib.h>
#include <syslog.h>

#include "conn_handler.h"
#include "signal_handler.h"
#include "arg_handler.h"

/*
 * MAIN PROGRAM
 */
int main(int argc, char *argv[])
{
    /* 
     * Set up the signal handlers
     */
    syslog(LOG_DEBUG, "Setting up signal events");
    setup_signals();

    /*
     * Get arguments and options
     */
    syslog(LOG_DEBUG, "Parsing arguments");
    struct arguments args;
    struct argp argp = {options, parse_opt, args_doc, doc};
    argp_parse(&argp, argc, argv, 0, 0, &args);

    /*
     * Configure the connection to the IBM Watson
     * cloud as a device
     */
    syslog(LOG_DEBUG, "Starting IoTPDevice configuration");
    IoTPConfig *config = NULL;
    IoTPDevice *device = NULL;

    if(device_configure(&config, &device, args) != 0){
        clean_stop_device(config, device);
        exit(1);
    }

    /*
     * Sends the RAM data to the cloud
     * every 10 seconds 
     */
    syslog(LOG_DEBUG, "Starting to send data to the cloud");
    if(send_data(device) != 0){
        clean_stop_device(config, device);
        exit(1);
    }
    
    /*
     * Used to cleanly disconnect the device 
     * from the cloud platform and clean the memory
     */
    syslog(LOG_DEBUG, "Cleaning IoTPDevice and IoTPConfig");
    clean_stop_device(config, device);
    exit(0);
}

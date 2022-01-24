#include <stdlib.h>

#ifndef "conn_handler.h"
#include "conn_handler.h"
#endif

#ifndef "signal_handler.h"
#include "signal_handler.h"
#endif

#ifndef "arg_handler.h"
#include "arg_handler.h"
#endif

/*
 * MAIN PROGRAM
 */
int main(int argc, char *argv[])
{
    int rc = 0;

    /* 
     * Set up the signal handlers
     */
    setup_signals();

    /*
     * Get arguments and options
     */
    struct arguments args;
    struct argp argp = {options, parse_opt, args_doc, doc};
    argp_parse(&argp, argc, argv, 0, 0, &args);

    /*
     * Configure the connection to the IBM Watson
     * cloud as a device
     */
    IoTPConfig *config = NULL;
    IoTPDevice *device = NULL;

    /*
     * Create a config file, that 
     * the device_configure function can later read
     */
    create_conf(&config, &args);

    device_configure(&config, &device);

    /*
     * Sends the RAM data to the cloud
     * every n seconds 
     */  
    send_data(device, *daemonise);
    
    /*
     * Used to cleanly disconnect the device 
     * from the cloud platform and clean the memory
     */
    clean_stop_device(config, device);
    return 0;
}

#include <stdio.h>

#include "conn_handler.h"
#include "ubus_handler.h"

#define CONF_FD "conn_config.yaml"

void logCallback (int level, char * message)
{
    if (level > 0)
        fprintf(stdout, "%s\n", message? message:"NULL");
    fflush(stdout);
}

void MQTTTraceCallback (int level, char * message)
{
    if (level > 0)
        fprintf(stdout, "%s\n", message? message:"NULL");
    fflush(stdout);
}

/*
 * Configure/Initialise the IoTPDevice variable
 */
int device_configure(IoTPConfig **config, IoTPDevice **device){
    int rc = 0;

    /* Set IoTP Client log handler */
    rc = IoTPConfig_setLogHandler(IoTPLog_FileDescriptor, stdout);
    if (rc != 0) {
        fprintf(stderr, "WARN: Failed to set IoTP Client log handler: rc=%d\n", rc);
        exit(1);
    }

    /* Create IoTPConfig object using configuration options defined in the configuration file. */
    rc = IoTPConfig_create(config, CONF_FD);
    if (rc != 0) {
        fprintf(stderr, "ERROR: Failed to initialize configuration: rc=%d\n", rc);
        exit(1);
    }
    rc = IoTPDevice_create(device, *config);
    if (rc != 0) {
        fprintf(stderr, "ERROR: Failed to configure IoTP device: rc=%d\n", rc);
        exit(1);
    }

    /* Set MQTT Trace handler */
    rc = IoTPDevice_setMQTTLogHandler(*device, &MQTTTraceCallback);
    if (rc != 0) {
        fprintf(stderr, "WARN: Failed to set MQTT Trace handler: rc=%d\n", rc);
    }

    /* Invoke connection API IoTPDevice_connect() to connect to WIoTP. */
    rc = IoTPDevice_connect(*device);
    if (rc != 0) {
        fprintf(stderr, "ERROR: Failed to connect to Watson IoT Platform: rc=%d\n", rc);
        fprintf(stderr, "ERROR: Returned error reason: %s\n", IOTPRC_toString(rc));
        exit(1);
    }

    return rc;
}

int clean_stop_device(IoTPConfig *config, IoTPDevice *device){
    int rc = 0;

    /* Disconnect device */
    rc = IoTPDevice_disconnect(device);
    if (rc != IOTPRC_SUCCESS) {
        fprintf(stderr, "ERROR: Failed to disconnect from  Watson IoT Platform: rc=%d\n", rc);
        exit(1);
    }

    /* Destroy client */
    IoTPDevice_destroy(device);

    /* Clear configuration */
    IoTPConfig_clear(config);

    return rc;
}

int format_ram(struct memory_info mem, char *output, int n){
    get_memory_info(&mem);

    snprintf(output, n, "{\"RAM\" : "
                            "{\"TOTAL\": \"%lld\", " 
                            "\"FREE\": \"%lld\", "
                            "\"SHARED\": \"%lld\", " 
                            "\"BUFFERED\": \"%lld\"}}", mem.total, mem.free, mem.shared, mem.buffered);

    return 0;
}

/*
 * Sends json formated data
 * to the IBM Watson cloud
 */
int send_data(IoTPDevice *device){
    int rc;

    /*
     * Start the ubus context,
     * which is used for getting data about the
     * router's current RAM
     */  
    start_ubus_ctx();

    while(1)
    {
        struct memory_info mem;

        char data[100];
        format_ram(mem, data, 100);

        fprintf(stdout, "Send status event\n");
        rc = IoTPDevice_sendEvent(device, "status", data, "json", QoS0, NULL);
        fprintf(stdout, "RC from publishEvent(): %d\n", rc);

        sleep(10);
    }

    close_ubus_ctx();

    return 0;
}
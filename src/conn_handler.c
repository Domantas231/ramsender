#include <stdio.h>
#include <syslog.h>

#include "conn_handler.h"
#include "ubus_handler.h"
#include "signal_handler.h"

void logCallback (int level, char * message)
{
    if (level > 0)
        syslog(LOG_INFO, "%s\n", message? message:"NULL");
}

void MQTTTraceCallback (int level, char * message)
{
    if (level > 0)
        syslog(LOG_INFO, "%s\n", message? message:"NULL");
}

/*
 * Configure the IoTPDevice variable,
 * so that it can connect to the correct cloud interface.
 */
int create_conf(IoTPConfig **config, struct arguments args){
    IoTPConfig_setProperty(*config, "identity.orgId", args.args[0]);
    IoTPConfig_setProperty(*config, "identity.typeId", args.args[1]);
    IoTPConfig_setProperty(*config, "identity.deviceId", args.args[2]);
    IoTPConfig_setProperty(*config, "auth.token", args.args[3]);

    return 0;
}

/*
 * Configure/Initialise the IoTPDevice variable
 */
int device_configure(IoTPConfig **config, IoTPDevice **device, struct arguments args){
    int rc = 0;

    /* Set IoTP Client log handler */
    rc = IoTPConfig_setLogHandler(IoTPLog_FileDescriptor, stdout);
    if (rc != 0) {
        syslog(LOG_WARNING, "WARN: Failed to set IoTP Client log handler: rc=%d\n", rc);
        exit(1);
    }

    /*
     * Configure the IoTPConfig object
     */
    rc = IoTPConfig_create(config, NULL);
    if(rc != 0){
        syslog(LOG_ERR, "ERROR: Failed to create IoTp configure object: rc=%d\n", rc);
        exit(1);
    }

    create_conf(config, args);

    rc = IoTPDevice_create(device, *config);
    if (rc != 0) {
        syslog(LOG_ERR, "ERROR: Failed to configure IoTP device: rc=%d\n", rc);
        exit(1);
    }

    /* Set MQTT Trace handler */
    rc = IoTPDevice_setMQTTLogHandler(*device, &MQTTTraceCallback);
    if (rc != 0) {
        syslog(LOG_WARNING, "WARN: Failed to set MQTT Trace handler: rc=%d\n", rc);
    }

    /* Invoke connection API IoTPDevice_connect() to connect to WIoTP. */
    rc = IoTPDevice_connect(*device);
    if (rc != 0) {
        syslog(LOG_ERR, "ERROR: Failed to connect to Watson IoT Platform: rc=%d\n", rc);
        syslog(LOG_ERR, "ERROR: Returned error reason: %s\n", IOTPRC_toString(rc));
        exit(1);
    }

    return rc;
}

int clean_stop_device(IoTPConfig *config, IoTPDevice *device){
    int rc = 0;

    /* Disconnect device */
    rc = IoTPDevice_disconnect(device);
    if (rc != IOTPRC_SUCCESS) {
        syslog(LOG_ERR, "ERROR: Failed to disconnect from  Watson IoT Platform: rc=%d\n", rc);
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
    syslog(LOG_DEBUG, "Starting ubus context");
    start_ubus_ctx();

    while(daemonise){
        struct memory_info mem;

        char data[100];
        format_ram(mem, data, 100);

        syslog(LOG_DEBUG, "Send status event\n");
        rc = IoTPDevice_sendEvent(device, "status", data, "json", QoS0, NULL);
        syslog(LOG_DEBUG, "RC from publishEvent(): %d\n", rc);

        sleep(10);
    }

    syslog(LOG_DEBUG, "Closing ubus context");
    close_ubus_ctx();
    return 0;
}
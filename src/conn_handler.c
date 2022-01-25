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
    IoTPConfig_setProperty(*config, "identity.orgId", args.orgId);
    IoTPConfig_setProperty(*config, "identity.typeId", args.typeId);
    IoTPConfig_setProperty(*config, "identity.deviceId", args.deviceId);
    IoTPConfig_setProperty(*config, "auth.token", args.token);

    return 0;
}

/*
 * Configure/Initialise the IoTPDevice variable
 */
int device_configure(IoTPConfig **config, IoTPDevice **device, struct arguments args){
    int rc = 0;

    /* 
     * Set IoTP Client log handler
     */
    rc = IoTPConfig_setLogHandler(IoTPLog_FileDescriptor, stdout);
    if (rc != 0) {
        syslog(LOG_WARNING, "WARN: Failed to set IoTP Client log handler: rc=%d\n", rc);
        return rc;
    }

    /*
     * Configure the IoTPConfig object
     */
    rc = IoTPConfig_create(config, NULL);
    if(rc != 0){
        syslog(LOG_ERR, "ERROR: Failed to create IoTp configure object: rc=%d\n", rc);
        return rc;
    }
    create_conf(config, args);

    rc = IoTPDevice_create(device, *config);
    if (rc != 0) {
        syslog(LOG_ERR, "ERROR: Failed to configure IoTP device: rc=%d\n", rc);
        return rc;
    }

    /* 
     * Set MQTT Trace handler
     */
    rc = IoTPDevice_setMQTTLogHandler(*device, &MQTTTraceCallback);
    if (rc != 0) {
        syslog(LOG_WARNING, "WARN: Failed to set MQTT Trace handler: rc=%d\n", rc);
    }

    /* 
     * Invoke connection API IoTPDevice_connect() 
     * to connect to WIoTP. 
     */
    rc = IoTPDevice_connect(*device);
    if (rc != 0) {
        syslog(LOG_ERR, "ERROR: Failed to connect to Watson IoT Platform: rc=%d\n", rc);
        syslog(LOG_ERR, "ERROR: Returned error reason: %s\n", IOTPRC_toString(rc));
        return rc;
    }


    return rc;
}

int clean_stop_device(IoTPConfig *config, IoTPDevice *device){
    int rc = 0;

    /* 
     * Disconnect device 
     */
    rc = IoTPDevice_disconnect(device);
    if (rc != IOTPRC_SUCCESS) {
        syslog(LOG_ERR, "ERROR: Failed to disconnect from  Watson IoT Platform: rc=%d\n", rc);
        return rc;
    }

    /* 
     * Destroy client
     */
    IoTPDevice_destroy(device);

    /* 
     * Clear configuration 
     */
    IoTPConfig_clear(config);

    return rc;
}

/*
 * Formats ram data into a json string format
 */
int format_ram(struct memory_info mem, char *output, int n){
    int rc = 0;
    rc = snprintf(output, n, "{\"RAM\" : "
                             "{\"TOTAL\": \"%lld\", " 
                             "\"FREE\": \"%lld\", "
                             "\"SHARED\": \"%lld\", " 
                             "\"BUFFERED\": \"%lld\"}}", mem.total, mem.free, mem.shared, mem.buffered);

    return rc;
}

/*
 * Sends json formated data
 * to the IBM Watson cloud
 */
int send_data(IoTPDevice *device){
    int rc = 0;

    /*
     * Start the ubus context,
     * which is used for getting data about the
     * router's current RAM
     */  
    syslog(LOG_DEBUG, "Starting ubus context");
    if((rc = start_ubus_ctx()) != 0){
        syslog(LOG_ERR, "Failed to connect to ubus, rc=%d", rc);
        return rc;
    }

    while(daemonise){
        struct memory_info mem;
        if((rc = get_memory_info(&mem)) != 0){
            syslog(LOG_ERR, "Cannot request memory info from procd");
            break;
        }

        char data[100];
        format_ram(mem, data, 100);

        if((rc = IoTPDevice_sendEvent(device, "status", data, "json", QoS0, NULL)) != 0){
            syslog(LOG_ERR, "Failed to send event to server, rc=%d", rc);
            break;
        }

        syslog(LOG_DEBUG, "RC from publishEvent(): %d", rc);

        sleep(10);
    }

    syslog(LOG_DEBUG, "Closing ubus context");
    close_ubus_ctx();
    return rc;
}
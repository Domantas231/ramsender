#include <iotp_device.h>

#include "arg_handler.h"

int device_configure(IoTPConfig **config, IoTPDevice **device, struct arguments args);
int send_data(IoTPDevice *device);
int clean_stop_device(IoTPConfig *config, IoTPDevice *device);
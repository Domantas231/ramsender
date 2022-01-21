#include <iotp_device.h>

int device_configure(IoTPConfig **config, IoTPDevice **device);
int send_data(IoTPDevice *device);
int clean_stop_device(IoTPConfig *config, IoTPDevice *device);
#include <iotp_device.h>

int device_configure(IoTPConfig **config, IoTPDevice **device);
int send_data(IoTPDevice *device, sig_atomic_t daemonise);
int clean_stop_device(IoTPConfig *config, IoTPDevice *device);
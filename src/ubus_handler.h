#ifndef UBUS_HANDLER_H
#define UBUS_HANDLER_H

#include <libubus.h>
#include <libubox/blobmsg_json.h>

struct memory_info {
	uint64_t total;
	uint64_t free;
	uint64_t shared;
	uint64_t buffered;
};

int start_ubus_ctx();
void close_ubus_ctx();
int get_memory_info(struct memory_info *mem);

#endif
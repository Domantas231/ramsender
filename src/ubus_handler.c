#include <stdio.h>

#include "ubus_handler.h"

/*
* global variables:
* CTX - current ubus context, needed to querry ubus
* MEM - holds info about the router memory
*/
struct ubus_context *CTX = NULL;
struct memory_info MEM;
int rc = 0;

enum {
	TOTAL_MEMORY,
	FREE_MEMORY,
	SHARED_MEMORY,
	BUFFERED_MEMORY,
	__MEMORY_MAX,
};

enum {
	MEMORY_DATA,
	__INFO_MAX,
};

/*
* TODO: what is this
*/
static const struct blobmsg_policy memory_policy[__MEMORY_MAX] = {
	[TOTAL_MEMORY] = { .name = "total", .type = BLOBMSG_TYPE_INT64 },
	[FREE_MEMORY] = { .name = "free", .type = BLOBMSG_TYPE_INT64 },
	[SHARED_MEMORY] = { .name = "shared", .type = BLOBMSG_TYPE_INT64 },
	[BUFFERED_MEMORY] = { .name = "buffered", .type = BLOBMSG_TYPE_INT64 },
};

/*
* TODO: what is this
*/
static const struct blobmsg_policy info_policy[__INFO_MAX] = {
	[MEMORY_DATA] = { .name = "memory", .type = BLOBMSG_TYPE_TABLE },
};

/*
* TODO: what is this
*/
static void board_cb(struct ubus_request *req, int type, struct blob_attr *msg) {
	struct blob_buf *buf = (struct blob_buf *)req->priv;
	struct blob_attr *tb[__INFO_MAX];
	struct blob_attr *memory[__MEMORY_MAX];

	blobmsg_parse(info_policy, __INFO_MAX, tb, blob_data(msg), blob_len(msg));

	if (!tb[MEMORY_DATA]) {
		fprintf(stderr, "No memory data received\n");
		rc=-1;
		return;
	}

	blobmsg_parse(memory_policy, __MEMORY_MAX, memory,
			blobmsg_data(tb[MEMORY_DATA]), blobmsg_data_len(tb[MEMORY_DATA]));

	MEM = (struct memory_info){ 
			.total = blobmsg_get_u64(memory[TOTAL_MEMORY]),
			.free = blobmsg_get_u64(memory[FREE_MEMORY]),
			.shared = blobmsg_get_u64(memory[SHARED_MEMORY]),
			.buffered = blobmsg_get_u64(memory[BUFFERED_MEMORY])
			};
}

extern int start_ubus_ctx(){
	CTX = ubus_connect(NULL);
	if (!CTX) {
		fprintf(stderr, "Failed to connect to ubus\n");
		return -1;
	}

	return 0;
}

extern void close_ubus_ctx(){
	ubus_free(CTX);
}

extern int get_memory_info(struct memory_info *mem){
	uint32_t id;

	if (ubus_lookup_id(CTX, "system", &id) ||
	    ubus_invoke(CTX, id, "info", NULL, board_cb, NULL, 3000)) {
		fprintf(stderr, "cannot request memory info from procd\n");
		rc=-1;
	}

    *mem = MEM;

	return rc;
}

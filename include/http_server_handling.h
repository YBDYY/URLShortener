#pragma once
// TODO: Add OptionItem MHD_OPTION_NOTIFY_COMPLETED
// TODO: Set up TLS options when implementing HTTPS
#include "core.h"
#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#define MHD_PLATFORM_H
#include <microhttpd.h>
#include <signal.h>
#include <cjson/cJSON.h>

#define MAX_CONNECTIONS 100
#define CONNECTION_TIMEOUT 15
#define BUFFER_SIZE 1024
// struct MHD_OptionItem options[] = {
// 	{ MHD_OPTION_CONNECTION_LIMIT, MAX_CONNECTIONS, NULL },
// 	{ MHD_OPTION_CONNECTION_TIMEOUT, CONNECTION_TIMEOUT, NULL },
// 	{ MHD_OPTION_END, 0, NULL }
// };

struct PostProcessorContext {
    struct MHD_PostProcessor *pp;
    char buffer[1024];
};

enum MHD_Result access_handler_callback(void *cls, struct MHD_Connection *connection,
            const char *url, const char *method,
            const char *version, const char *upload_data,
            size_t *upload_data_size, void **con_cls);

int handleAddPostRequest(struct MHD_Connection *connection, const char *upload_data, size_t *upload_data_size);

int handleResolveGetRequest(struct MHD_Connection *connection, const char *short_code);

enum MHD_Result post_iterator(void *cls,
                         enum MHD_ValueKind kind,
                         const char *key,
                         const char *filename,
                         const char *content_type,
                         const char *transfer_encoding,
                         const char *data,
                         uint64_t off,
                         size_t size);
#pragma once
// TODO: Set up TLS options when implementing HTTPS
#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#define MHD_PLATFORM_H
#include <microhttpd.h>

#define MAX_CONNECTIONS 100
#define CONNECTION_TIMEOUT 15
#define BUFFER_SIZE 1024

struct PostProcessorContext {
    struct MHD_PostProcessor *pp;
    char buffer[BUFFER_SIZE];
};

void handleMHDGetResponses(struct MHD_Connection *connection, char *error_msg, void **con_cls, int code);

void handleMHDPortResponses(struct MHD_Connection *connection, struct PostProcessorContext *ctx, char *error_msg, void **con_cls, int code);

enum MHD_Result access_handler_callback(void *cls, struct MHD_Connection *connection,
            const char *url, const char *method,
            const char *version, const char *upload_data,
            size_t *upload_data_size, void **con_cls);

#pragma once
#include "http_core_handling.h"
#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#define MHD_PLATFORM_H
#include <microhttpd.h>

struct PostProcessorContext;

int handlePostRequest(struct MHD_Connection *connection, const char *upload_data, size_t *upload_data_size, void **con_cls);

int checkPostProcessor(struct PostProcessorContext *ctx, struct MHD_Connection *connection, void **con_cls);

int initializePostContext(struct MHD_Connection *connection, size_t *upload_data_size, void **con_cls);

enum MHD_Result postIterator(void *cls,
                         enum MHD_ValueKind kind,
                         const char *key,
                         const char *filename,
                         const char *content_type,
                         const char *transfer_encoding,
                         const char *data,
                         uint64_t off,
                         size_t size);

int respondToPostRequest(struct PostProcessorContext *ctx, struct MHD_Connection *connection, void **con_cls);

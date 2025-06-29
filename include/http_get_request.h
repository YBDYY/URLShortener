#pragma once
#include "http_core_handling.h"
#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#define MHD_PLATFORM_H
#include <microhttpd.h>

int handleGetRequest(struct MHD_Connection *connection, const char *upload_data, size_t *upload_data_size, void **con_cls);

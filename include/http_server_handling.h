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

#define MAX_CONNECTIONS 100
#define CONNECTION_TIMEOUT 15

// struct MHD_OptionItem options[] = {
// 	{ MHD_OPTION_CONNECTION_LIMIT, MAX_CONNECTIONS, NULL },
// 	{ MHD_OPTION_CONNECTION_TIMEOUT, CONNECTION_TIMEOUT, NULL },
// 	{ MHD_OPTION_END, 0, NULL }
// };

enum MHD_Result *MHD_AccessHandlerCallback(void *cls, struct MHD_Connection *connection,
            const char *url, const char *method,
            const char *version, const char *upload_data,
            size_t *upload_data_size, void **con_cls);





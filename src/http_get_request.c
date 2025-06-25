#include "../include/http_get_request.h"

int handleResolveRequest(struct MHD_Connection *connection, const char *short_code)  // TODO: Implement this function
{
    int rc = handleResolve(short_code);
    if (rc != 0) {
        const char *error_msg = "Failed to resolve short code";
        struct MHD_Response *response = MHD_create_response_from_buffer(strlen(error_msg),
                                            (void *)error_msg, MHD_RESPMEM_PERSISTENT);
        MHD_queue_response(connection, MHD_HTTP_INTERNAL_SERVER_ERROR, response);
        MHD_destroy_response(response);
        return MHD_NO;
    }
    const char *msg = "Short code resolved";
    struct MHD_Response *response = MHD_create_response_from_buffer(strlen(msg),
                                        (void *)msg, MHD_RESPMEM_PERSISTENT);
    MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);
    return MHD_YES;
}


#include "../include/http_core_handling.h"

void handleMHDResponses(struct MHD_Connection *connection, struct PostProcessorContext *ctx, char *error_msg, void **con_cls, int code)
{   
    struct MHD_Response *response = MHD_create_response_from_buffer(strlen(error_msg),
                                            (void *)error_msg, MHD_RESPMEM_PERSISTENT);
    MHD_queue_response(connection, code, response);
    log_info("Response sent: %s (code=%d)", error_msg, code);
    MHD_destroy_response(response);
    if (ctx != NULL) {
        log_error("Cleaning up post processor context: %p", (void *)ctx);
        MHD_destroy_post_processor(ctx->pp);
        free(ctx);
    }
    *con_cls = NULL;
}

enum MHD_Result access_handler_callback(void *cls, struct MHD_Connection *connection,
            const char *url, const char *method,
            const char *version, const char *upload_data,
            size_t *upload_data_size, void **con_cls)
{   
    if (strcmp(method, "POST") == 0) {
        int rc = handlePostRequest(connection, upload_data, upload_data_size, con_cls);
        if (rc != MHD_YES) {
            handleMHDResponses(connection, NULL, "Failed to handle POST request", con_cls, MHD_HTTP_INTERNAL_SERVER_ERROR);
            log_error("Failed to handle POST request for URL: %s", url);
        }
        return MHD_YES;
    }

    // if (strcmp(method, "GET") == 0) { // TODO: Implement this function
    //     const char *short_code = url + 8;
    //     return (handleResolveGetRequest(connection, short_code) == MHD_HTTP_OK)
    //         ? MHD_YES : MHD_NO;
    // }
    handleMHDResponses(connection, NULL, "Not found - valid endpoints are /add (POST) and /resolve/<short_code> (GET)", con_cls, MHD_HTTP_NOT_FOUND);
    return MHD_YES;
}


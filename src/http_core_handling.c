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
        int rc = initializePostContext(connection, upload_data_size, con_cls);
        if (rc != MHD_YES) return rc;
        struct PostProcessorContext *ctx = *con_cls;
        if (*upload_data_size != 0) {
            MHD_post_process(ctx->pp, upload_data, *upload_data_size);
            *upload_data_size = 0;
            return MHD_YES;
        }
        if (ctx->pp == NULL) {
            handleMHDResponses(connection, ctx, "No data received", con_cls, MHD_HTTP_BAD_REQUEST);
            return MHD_NO;
        }   
        rc = respondToPostRequest(ctx, connection, con_cls);
        if (rc != MHD_YES) return MHD_NO;
        handleMHDResponses(connection, ctx, "URL added successfully", con_cls, MHD_HTTP_OK);
        return MHD_YES;
    }

    // if (strcmp(method, "GET") == 0) { // TODO: Implement this function
    //     const char *short_code = url + 8;
    //     return (handleResolveGetRequest(connection, short_code) == MHD_HTTP_OK)
    //         ? MHD_YES : MHD_NO;
    // }
    const char *not_found = "Not found - valid endpoints are /add (POST) and /resolve/<short_code> (GET)";
    struct MHD_Response *response = MHD_create_response_from_buffer(strlen(not_found),
                                        (void *)not_found, MHD_RESPMEM_PERSISTENT);
    MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, response);
    MHD_destroy_response(response);
    return MHD_YES;
}


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
        if (*con_cls == NULL) {
            return handlePostRequest(connection, upload_data, upload_data_size, con_cls);
        }

        if (*upload_data_size != 0) {
            return handlePostRequest(connection, upload_data, upload_data_size, con_cls);
        }

        return handlePostRequest(connection, upload_data, upload_data_size, con_cls);
    }

    return MHD_NO;
}

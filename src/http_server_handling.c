#include "../include/http_server_handling.h"

int handleResolveGetRequest(struct MHD_Connection *connection, const char *short_code)
{
    int rc = handleResolve(short_code);
    if (rc != 0) {
        const char *error_msg = "Failed to resolve short code";
        struct MHD_Response *response = MHD_create_response_from_buffer(strlen(error_msg),
                                            (void *)error_msg, MHD_RESPMEM_PERSISTENT);
        int ret = MHD_queue_response(connection, MHD_HTTP_INTERNAL_SERVER_ERROR, response);
        MHD_destroy_response(response);
        return ret;
    }
    const char *msg = "Short code resolved";
    struct MHD_Response *response = MHD_create_response_from_buffer(strlen(msg),
                                        (void *)msg, MHD_RESPMEM_PERSISTENT);
    int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);
    return ret;
}

struct PostProcessorContext {
    struct MHD_PostProcessor *pp;
    char buffer[1024];
};

enum MHD_Result post_iterator(void *cls,
                         enum MHD_ValueKind kind,
                         const char *key,
                         const char *filename,
                         const char *content_type,
                         const char *transfer_encoding,
                         const char *data,
                         uint64_t off,
                         size_t size)
{
    if (strcmp(key, "url") == 0 && data != NULL) {
        char *dest = (char *)cls;
        strncat(dest, data, size);
    }
    return MHD_NO;
}
enum MHD_Result access_handler_callback(void *cls, struct MHD_Connection *connection,
            const char *url, const char *method,
            const char *version, const char *upload_data,
            size_t *upload_data_size, void **con_cls)
{   
    if (strcmp(method, "POST") == 0) {
        if (*con_cls == NULL) {
            struct PostProcessorContext *ctx = calloc(1, sizeof(struct PostProcessorContext));
            ctx->pp = MHD_create_post_processor(connection, 1024, post_iterator, ctx->buffer);
            if (ctx->pp == NULL) {
                free(ctx);
                return MHD_NO;
            }
            ctx->buffer[0] = '\0';
            *upload_data_size = 0;
            *con_cls = ctx;
            return MHD_YES;
        }
        struct PostProcessorContext *ctx = *con_cls;
        if (*upload_data_size != 0) {
            MHD_post_process(ctx->pp, upload_data, *upload_data_size);
            *upload_data_size = 0;
            return MHD_YES;
        }
        if (ctx->pp == NULL) {
            const char *error_msg = "No data received";
            struct MHD_Response *response = MHD_create_response_from_buffer(strlen(error_msg),
                                                (void *)error_msg, MHD_RESPMEM_PERSISTENT);
            MHD_queue_response(connection, MHD_HTTP_BAD_REQUEST, response);
            MHD_destroy_response(response);
            MHD_destroy_post_processor(ctx->pp);
            free(ctx);
            *con_cls = NULL;
            return MHD_NO;
        }   
        int rc = handleAdd(ctx->buffer);
        if (rc != 0) {
            if (rc == SQLITE_DETERMINISTIC) {
                const char *msg = "Short code already exists";
                struct MHD_Response *response = MHD_create_response_from_buffer(strlen(msg),
                                                (void *)msg, MHD_RESPMEM_PERSISTENT);
                MHD_queue_response(connection, MHD_HTTP_CONFLICT, response);
                MHD_destroy_response(response);
                MHD_destroy_post_processor(ctx->pp);
                free(ctx);
                *con_cls = NULL;
                return MHD_YES;
            } else {
                const char *msg = "Failed to add URL";
                struct MHD_Response *response = MHD_create_response_from_buffer(strlen(msg),
                                                (void *)msg, MHD_RESPMEM_PERSISTENT);
                MHD_queue_response(connection, MHD_HTTP_INTERNAL_SERVER_ERROR, response);
                MHD_destroy_response(response);
                MHD_destroy_post_processor(ctx->pp);
                free(ctx);
                *con_cls = NULL;
                return MHD_NO;
            }
        }
        const char *msg = "URL added successfully";
        struct MHD_Response *response = MHD_create_response_from_buffer(strlen(msg),
                                            (void *)msg, MHD_RESPMEM_PERSISTENT);
        MHD_queue_response(connection, MHD_HTTP_OK, response);
        MHD_destroy_response(response);
        MHD_destroy_post_processor(ctx->pp);
        free(ctx);
        *con_cls = NULL;
        return MHD_YES;
    }

    if (strcmp(method, "GET") == 0) {
        const char *short_code = url + 8;
        return (handleResolveGetRequest(connection, short_code) == MHD_HTTP_OK)
            ? MHD_YES : MHD_NO;
    }
    const char *not_found = "Not found - valid endpoints are /add (POST) and /resolve/<short_code> (GET)";
    struct MHD_Response *response = MHD_create_response_from_buffer(strlen(not_found),
                                        (void *)not_found, MHD_RESPMEM_PERSISTENT);
    int ret = MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, response);
    MHD_destroy_response(response);
    return ret;
}
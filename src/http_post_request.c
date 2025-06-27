#include "../include/http_post_request.h"

int checkPostProcessor(struct PostProcessorContext *ctx, const char *key, size_t size, const char *data, struct MHD_Connection *connection, void **con_cls)
{
    if (ctx->pp == NULL) {
        handleMHDResponses(connection, ctx, "No data received", con_cls, MHD_HTTP_BAD_REQUEST);
        log_error("No post processor available for ctx=%p", (void *)ctx);
        free(ctx);
        return MHD_NO;
    }
    log_info("Post processor initialized for ctx=%p", (void *)ctx);
    return MHD_YES;
}

enum MHD_Result postIterator(void *cls,
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
        log_info("POST field received: key=%s, size=%zu, data=%.*s", key, size, (int)size, data);
    }
    return MHD_YES;
}

int initializePostContext(struct MHD_Connection *connection, size_t *upload_data_size, void **con_cls)
{
    struct PostProcessorContext *ctx = calloc(1, sizeof(struct PostProcessorContext));
    if (!ctx) return MHD_NO;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->pp = MHD_create_post_processor(connection, BUFFER_SIZE, postIterator, ctx->buffer);
    if (checkPostProcessor(ctx, NULL, 0, NULL, connection, con_cls) != MHD_YES) {
        free(ctx);
        return MHD_NO;
    }
    *upload_data_size = 0;
    *con_cls = ctx;
    log_info("Initialized POST context successfully");
    return MHD_YES;
}

int handlePostRequest(struct MHD_Connection *connection, const char *upload_data,
                      size_t *upload_data_size, void **con_cls)
{
    struct PostProcessorContext *ctx = *con_cls;
    if (!ctx)
        return initializePostContext(connection, upload_data_size, con_cls);
    if (*upload_data_size != 0) {
        MHD_post_process(ctx->pp, upload_data, *upload_data_size);
        *upload_data_size = 0;
        return MHD_YES;
    }
    return respondToPostRequest(ctx, connection, con_cls);
}

int respondToPostRequest(struct PostProcessorContext *ctx, struct MHD_Connection *connection, void **con_cls)
{
    int rc = handleAdd(ctx->buffer);
    if (rc != 0) {
        if (rc == SQLITE_DETERMINISTIC) {
            handleMHDResponses(connection, ctx, "Short code already exists", con_cls, MHD_HTTP_CONFLICT);
            return MHD_YES;
        } else {
            handleMHDResponses(connection, ctx, "Failed to add URL", con_cls, MHD_HTTP_INTERNAL_SERVER_ERROR);
            return MHD_YES;
        }
    }
	log_info("URL added successfully: %s", ctx->buffer);
    handleMHDResponses(connection, ctx, "URL added successfully", con_cls, MHD_HTTP_OK);
    return MHD_YES;
}

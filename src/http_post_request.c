#include "../include/http_post_request.h"

int initializePostProcessor(struct PostProcessorContext *ctx, struct MHD_Connection *connection, void **con_cls)
{
    if (ctx->pp == NULL) {
        handleMHDResponses(connection, ctx, "No data received", con_cls, MHD_HTTP_BAD_REQUEST);
        return MHD_NO;
    }   
    return MHD_YES;
}

int initializePostContext(struct MHD_Connection *connection, size_t *upload_data_size, void **con_cls)
{
    if (*con_cls == NULL) {
        struct PostProcessorContext *ctx = calloc(1, sizeof(struct PostProcessorContext));
        if (ctx == NULL) {
            handleMHDResponses(connection, NULL, "Failed to allocate memory for context", con_cls, MHD_HTTP_INTERNAL_SERVER_ERROR);
            return MHD_NO;
        }
        ctx->pp = MHD_create_post_processor(connection, BUFFER_SIZE, postIterator, ctx->buffer);
        if (ctx->pp == NULL) {
            free(ctx);
            return MHD_NO;
        }
        ctx->buffer[0] = '\0';
        *upload_data_size = 0;
        *con_cls = ctx;
        return MHD_YES;
    }
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
    }
    return MHD_NO;
}

int handlePostRequest(struct MHD_Connection *connection, const char *upload_data, size_t *upload_data_size, void **con_cls)
{
    int rc = initializePostContext(connection, upload_data_size, con_cls);
    if (rc != MHD_YES) return rc;
    struct PostProcessorContext *ctx = *con_cls;
    if (*upload_data_size != 0) {
        MHD_post_process(ctx->pp, upload_data, *upload_data_size);
        *upload_data_size = 0;
        return MHD_YES;
    }
    rc = initializePostProcessor(ctx, connection, con_cls);
    if (rc != MHD_YES) return MHD_NO;
    rc = handleAdd(ctx->buffer);
    if (rc != 0) {
        if (rc == SQLITE_DETERMINISTIC) {
            handleMHDResponses(connection, ctx, "Short code already exists", con_cls, MHD_HTTP_CONFLICT);
            return MHD_YES;
        } else {
            handleMHDResponses(connection, ctx, "Failed to add URL", con_cls, MHD_HTTP_INTERNAL_SERVER_ERROR);
            return MHD_NO;
        }
    }
    handleMHDResponses(connection, ctx, "URL added successfully", con_cls, MHD_HTTP_OK);
    return MHD_YES;
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
            return MHD_NO;
        }
    }
    return MHD_YES;
}
#include "../include/http_core_handling.h"
#include "../include/http_post_request.h"
#include "../include/http_get_request.h"
#include "../include/logging.h"
#include "../include/signal_handling.h"
#include "../include/db.h"
#include <microhttpd.h>
#include <string.h>
#include <stdlib.h>

void handleMHDPortResponses(struct MHD_Connection *connection, struct PostProcessorContext *ctx, char *error_msg, void **con_cls, int code)
{
    struct MHD_Response *response = MHD_create_response_from_buffer(strlen(error_msg),
                                        (void *)error_msg, MHD_RESPMEM_PERSISTENT);
	set_signal_context(ctx, response, NULL);
    MHD_queue_response(connection, code, response);
    log_info("Response sent: %s (code=%d)", error_msg, code);
    MHD_destroy_response(response);
    if (ctx != NULL) {
        log_error("Cleaning up post processor context: %p", (void *)ctx);
        MHD_destroy_post_processor(ctx->pp);
        free(ctx);
    }
    if (con_cls) *con_cls = NULL;
}

void handleMHDGetResponses(struct MHD_Connection *connection, char *error_msg, void **con_cls, int code)
{
    struct MHD_Response *response = MHD_create_response_from_buffer(strlen(error_msg),
                                        (void *)error_msg, MHD_RESPMEM_PERSISTENT);
	set_signal_context(NULL, response, NULL);
    MHD_queue_response(connection, code, response);
    log_info("Response sent: %s (code=%d)", error_msg, code);
    MHD_destroy_response(response);
    if (con_cls) *con_cls = NULL;
}

enum MHD_Result access_handler_callback(void *cls, struct MHD_Connection *connection,
        const char *url, const char *method,
        const char *version, const char *upload_data,
        size_t *upload_data_size, void **con_cls)
{
    if (strcmp(method, "POST") == 0) { 
        if (*con_cls == NULL) return handlePostRequest(connection, upload_data, upload_data_size, con_cls);
        if (*upload_data_size != 0) return handlePostRequest(connection, upload_data, upload_data_size, con_cls);
        return handlePostRequest(connection, upload_data, upload_data_size, con_cls);
    }
	if (strcmp(method, "GET") == 0) {
		if (*con_cls == NULL) return handleGetRequest(connection, upload_data, upload_data_size, con_cls);
		if (*upload_data_size != 0) return handleGetRequest(connection, upload_data, upload_data_size, con_cls);
		return handleGetRequest(connection, upload_data, upload_data_size, con_cls);
	}
	log_error("Unable to handle request: method=%s", method);
    return MHD_NO;
}

void cleanup(struct PostProcessorContext *ctx, struct MHD_Response *response, struct MHD_Daemon *daemon)
{
	if (ctx != NULL) {
		log_info("Cleaning up post processor context: %p", (void *)ctx);
		if (ctx->pp) MHD_destroy_post_processor(ctx->pp);
		free(ctx);
	}
	if (response != NULL) {
		log_info("Cleaning up MHDResponse: %p", (void *)response);
		MHD_destroy_response(response);
	}
	if (daemon != NULL) {
		log_info("Stopping MHD daemon: %p", (void *)daemon);
		MHD_stop_daemon(daemon);
	}
	dbClose();
	close_logging();
}

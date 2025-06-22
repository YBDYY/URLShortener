#include "../include/http_server_handling.h"

enum MHD_Result *MHD_AccessHandlerCallback(void *cls, struct MHD_Connection *connection,
            const char *url, const char *method,
            const char *version, const char *upload_data,
            size_t *upload_data_size, void **con_cls)
{   
    if (strcmp(method, "POST") == 0 && strcmp(url, "/add") == 0) {
        const char *web_address = upload_data;
        const char *content_type = MHD_lookup_connection_value(connection, MHD_HEADER_KIND, "Content-Type");
        if (content_type == NULL || strcmp(content_type, "application/x-www-form-urlencoded") != 0) {
            const char *error_msg = "Unsupported content type";
            struct MHD_Response *response = MHD_create_response_from_buffer(strlen(error_msg),
                                                (void *)error_msg, MHD_RESPMEM_PERSISTENT);
            int ret = MHD_queue_response(connection, MHD_HTTP_UNSUPPORTED_MEDIA_TYPE, response);
            MHD_destroy_response(response);
            return ret;
        }
        else if (strcmp(content_type, "application/x-www-form-urlencoded") == 0) {
            
        }
        int rc = handleAdd(web_address);
        if (rc != 0) {
            const char *error_msg = "Failed to add URL";
            struct MHD_Response *response = MHD_create_response_from_buffer(strlen(error_msg),
                                                (void *)error_msg, MHD_RESPMEM_PERSISTENT);
            int ret = MHD_queue_response(connection, MHD_HTTP_INTERNAL_SERVER_ERROR, response);
            MHD_destroy_response(response);
            return ret;
        }
        const char *msg = "URL added";
        struct MHD_Response *response = MHD_create_response_from_buffer(strlen(msg),
                                            (void *)msg, MHD_RESPMEM_PERSISTENT);
        int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
        MHD_destroy_response(response);
        return ret;
    }

    else if (strcmp(method, "GET") == 0 && strncmp(url, "/resolve", 9) == 0) {
        const char *short_code = url + 9;
        // Handle resolving a short code
    }
    const char *not_found = "Not found";
    struct MHD_Response *response = MHD_create_response_from_buffer(strlen(not_found),
                                        (void *)not_found, MHD_RESPMEM_PERSISTENT);
    int ret = MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, response);
    MHD_destroy_response(response);
    return ret;

}
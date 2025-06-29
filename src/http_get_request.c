#include "../include/http_get_request.h"
#include "../include/core.h"

int handleGetRequest(struct MHD_Connection *connection, const char *upload_data, size_t *upload_data_size, void **con_cls)
{
	const char *short_code = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "short_code");
	if (short_code == NULL) {
		handleMHDGetResponses(connection, "Short code not provided", con_cls, MHD_HTTP_BAD_REQUEST);
		return MHD_YES;
	}
	int rc = handleResolve(short_code);
    if (rc != 0) {
		handleMHDGetResponses(connection, "Failed to resolve short code", NULL, MHD_HTTP_INTERNAL_SERVER_ERROR);
        return MHD_YES;
    }
	handleMHDGetResponses(connection, "Short code resolved successfully", NULL, MHD_HTTP_OK);
    return MHD_YES;
}


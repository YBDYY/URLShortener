#include "../include/http_core_handling.h"

#define PORT 8888

int main(){
	// Prevent the application from terminating when writing to a closed socket.
	// libmicrohttpd does not handle SIGPIPE, so we ignore it to ensure stability.
	signal(SIGPIPE, SIG_IGN); 
	init_logging("urlshortener");
	log_info("Service started");
	struct MHD_Daemon *daemon = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD | MHD_USE_ERROR_LOG, PORT, NULL, NULL,                         &access_handler_callback, NULL, MHD_OPTION_END);

	if (daemon == NULL) {
		MHD_stop_daemon(daemon);
		dbClose();
		return -1;
	}
	getchar();
	MHD_stop_daemon(daemon);
	dbClose();
	close_logging();
	return 0;
}

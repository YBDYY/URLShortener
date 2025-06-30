#include "../include/http_core_handling.h"
#include "../include/logging.h"
#include "../include/db.h"
#include "../include/signal_handling.h"
#include <signal.h>
#include <stdio.h>
#include <string.h> 
#define PORT 8888

int main(){
	// Prevent the application from terminating when writing to a closed socket.
	// libmicrohttpd does not handle SIGPIPE, so we ignore it to ensure stability.
	signal(SIGPIPE, SIG_IGN); 
	setup_signal_handlers();
	init_logging("urlshortener");
	log_info("Service started");
	daemon_microhttpd = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD | MHD_USE_ERROR_LOG, PORT, NULL, NULL, &access_handler_callback, NULL, MHD_OPTION_END);
	if (daemon_microhttpd == NULL) {
		MHD_stop_daemon(daemon_microhttpd);
		dbClose();
		return -1;
	}
	getchar();
	MHD_stop_daemon(daemon_microhttpd);
	dbClose();
	close_logging();
	return 0;
}

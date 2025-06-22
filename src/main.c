#include "../include/hashing.h"
#include "../include/db.h"
#include "../include/core.h"
#include "../include/http_server_handling.h"

#include <microhttpd.h>
#include <string.h>

#define PORT 8888

int main(){
    // Prevent the application from terminating when writing to a closed socket.
    // libmicrohttpd does not handle SIGPIPE, so we ignore it to ensure stability.
    signal(SIGPIPE, SIG_IGN); 
	struct MHD_Daemon *daemon = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD | MHD_USE_ERROR_LOG, PORT, NULL, NULL,
                              &MHD_AccessHandlerCallback, NULL, MHD_OPTION_END);
    if (daemon == NULL) return 1;
    getchar();
    MHD_stop_daemon(daemon);
    return 0;
}
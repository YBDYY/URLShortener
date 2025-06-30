#include "../include/signal_handling.h"
#include "../include/logging.h"
#include "../include/http_core_handling.h"
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static signal_context_t *get_signal_context_static(void) 
{
    static signal_context_t context = {0};
    return &context;
}

signal_context_t *get_signal_context(void) 
{
    return get_signal_context_static();
}

void set_signal_context(struct PostProcessorContext *post_processor_context, struct MHD_Response *response, struct MHD_Daemon *daemon)
{
    signal_context_t *signal_ctx = get_signal_context();
	signal_ctx->post_processor_context = post_processor_context;
	signal_ctx->response = response;
	signal_ctx->daemon = daemon;
}

void handle_signal(int sig) 
{
	log_info("Received signal %d, cleaning up and exiting...", sig);
    signal_context_t *ctx = get_signal_context();
	if (ctx->post_processor_context) cleanup(ctx->post_processor_context, NULL, NULL);
	if (ctx->response) cleanup(NULL, ctx->response, NULL);
	if (ctx->daemon) cleanup(NULL, NULL, ctx->daemon);
    exit(EXIT_SUCCESS);
}

void setup_signal_handlers(void) 
{
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    int signals[] = {SIGINT, SIGTERM, SIGHUP, SIGSEGV, SIGCHLD};
    for (size_t i = 0; i < sizeof(signals) / sizeof(signals[0]); ++i) {
        if (sigaction(signals[i], &sa, NULL) < 0) {
            exit(EXIT_FAILURE);
        }
    }
}

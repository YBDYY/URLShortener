#pragma once

typedef struct signal_context {
    struct PostProcessorContext *post_processor_context;
    struct MHD_Response *response;
} signal_context_t;

signal_context_t *get_signal_context(void);

void set_signal_context(struct PostProcessorContext *post_processor_context);

void setup_signal_handlers(void);

void handle_signal(int sig);

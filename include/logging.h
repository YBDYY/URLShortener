#pragma once

#include <stdarg.h>
#include <syslog.h>

void init_logging(const char *ident);
void log_info(const char *format, ...);
void log_error(const char *format, ...);
void close_logging(void);

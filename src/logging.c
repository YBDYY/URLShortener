#include "../include/logging.h"
#include <stdarg.h>
#include <syslog.h>

void init_logging(const char *ident) 
{
    openlog(ident, LOG_PID | LOG_CONS, LOG_USER);
}

void log_info(const char *format, ...) 
{
    va_list args;
    va_start(args, format);
    vsyslog(LOG_INFO, format, args);
    va_end(args);
}

void log_error(const char *format, ...) 
{
    va_list args;
    va_start(args, format);
    vsyslog(LOG_ERR, format, args);
    va_end(args);
}

void close_logging(void) 
{
    closelog();
}

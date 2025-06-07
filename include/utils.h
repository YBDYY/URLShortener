#ifndef UTILS_H
#define UTILS_H

#include <argp.h>
#include <syslog.h>

#include <string.h>

struct arguments {
	char *subCommand;
	char *subCommandArgument;
};

static struct argp_option options[] = {
	{"help",  'h', 0, OPTION_ARG_OPTIONAL, "Show help message", -1},
    {"usage", 'u', 0, OPTION_ARG_OPTIONAL, "Show usage", 0},
    {0}
};

error_t parseOpt(int key, char *arg, struct argp_state *state);

int loadArguments(int argc, char **argv, struct arguments *arguments);

int handleArguments(const char *subCommand, const char *argument);

#endif
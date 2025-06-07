#pragma once

#include <argp.h>
#include <syslog.h>

#include <string.h>

struct arguments {
	char *subCommand;
	char *subCommandArgument;
};

error_t parseOpt(int key, char *arg, struct argp_state *state);

int loadArguments(int argc, char **argv, struct arguments *arguments);

int handleArguments(const char *subCommand, const char *argument);

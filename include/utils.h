#pragma once

#include <argp.h>
#include <syslog.h>

#include <string.h>

struct arguments {
	char subCommand[12];
	char subCommandArgument[512];
};

error_t parseOpt(int key, char *arg, struct argp_state *state);

int loadArguments(int argc, char **argv, struct arguments *arguments);

int handleArguments(const char *subCommand, const char *argument);

void safeCopy(char *dest, size_t size, const char *src);
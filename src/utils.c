#include "../include/utils.h"

static struct argp_option options[] = {
	{"help",  'h', 0, OPTION_ARG_OPTIONAL, "Show help message", -1},
    {"usage", 'u', 0, OPTION_ARG_OPTIONAL, "Show usage", 0},
    {0}
};

void safeCopy(char *dest, size_t size, const char *src) {
	strncpy(dest, src, size - 1);
	dest[size - 1] = '\0';
}

error_t parseOpt(int key, char *arg, struct argp_state *state)
{	
	struct arguments* cli = state->input;
	switch (key) {
        case 'h':
            argp_state_help(state, state->out_stream, ARGP_HELP_STD_HELP | OPTION_ARG_OPTIONAL);
            break;
        case 'u':
            argp_state_help(state, state->out_stream, ARGP_HELP_USAGE | ARGP_HELP_EXIT_OK | OPTION_ARG_OPTIONAL);
            break;
        case ARGP_KEY_ARG:
			if (state->arg_num == 0)
                safeCopy(cli->subCommand, sizeof(arg), arg);
			else if (state->arg_num == 1)
				safeCopy(cli->subCommandArgument, sizeof(arg), arg);
			else
				argp_usage(state);
			break;
        case ARGP_KEY_END:
			if (strcmp(cli->subCommand, "") == 0 || strcmp(cli->subCommandArgument, "") == 0)
                argp_usage(state);
			break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return UTILS_SUCCESS;
}

int loadArguments(int argc, char **argv, struct arguments *arguments)
{   
    if (argc < 2){
        fprintf(stderr, "Error: No arguments passed \n");
        return UTILS_INVALID_ARG;
    }
    static struct argp argp = {options, parseOpt, 0, 0};
    memset(arguments, 0, sizeof(*arguments)); 
    argp_parse(&argp, argc, argv, ARGP_NO_HELP, 0, arguments);
    int result = handleArguments(arguments->subCommand, arguments->subCommandArgument);
    if (result != UTILS_SUCCESS) return result;
    return UTILS_SUCCESS;
}

int handleArguments(const char *subCommand, const char *argument)
{
    if (strcmp(subCommand, "add") == 0 || strcmp(subCommand, "resolve") == 0) return UTILS_SUCCESS;
    if (strcmp(argument, "") == 0 || argument == NULL) {
        fprintf(stderr, "Error: No provided argument \n");
        return UTILS_ERROR;
    }
    else {
        fprintf(stderr, "Error: Unknown subcommand \"%s\"\n", subCommand);
        return UTILS_ERROR;
    }
}
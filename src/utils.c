#include "../include/utils.h"

// TODO: when scaling, implement custom return codes (enum)

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
	struct arguments* arguments = state->input;
	switch (key) {
        case 'h':
            argp_state_help(state, state->out_stream, ARGP_HELP_STD_HELP | OPTION_ARG_OPTIONAL);
            break;

        case 'u':
            argp_state_help(state, state->out_stream, ARGP_HELP_USAGE | ARGP_HELP_EXIT_OK | OPTION_ARG_OPTIONAL);
            break;

        case ARGP_KEY_ARG:
			if (state->arg_num == 0)
                safeCopy(arguments->subCommand, sizeof(arg), arg);
			else if (state->arg_num == 1)
				safeCopy(arguments->subCommandArgument, sizeof(arg), arg);
			else
				argp_usage(state);
			break;

        case ARGP_KEY_END:
			if (strcmp(arguments->subCommand, "") == 0 || strcmp(arguments->subCommandArgument, "") == 0)
                argp_usage(state);
			break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

int loadArguments(int argc, char **argv, struct arguments *arguments)
{   
    if (argc < 2){
        fprintf(stderr, "Error: No arguments passed \n");
        return -1;
    }
    static struct argp argp = {options, parseOpt, 0, 0};
    memset(arguments, 0, sizeof(*arguments)); 
    argp_parse(&argp, argc, argv, ARGP_NO_HELP, 0, arguments);
    int result = handleArguments(arguments->subCommand, arguments->subCommandArgument);
    if (result != 0)
        return result;
    return 0;
}

int handleArguments(const char *subCommand, const char *argument)
{
    if (strcmp(subCommand, "add") == 0 || strcmp(subCommand, "resolve") == 0) return 0;
    if (strcmp(argument, "") == 0 || argument == NULL) {
        fprintf(stderr, "Error: No provided argument \n");
        return -1;
    }
    else {
        fprintf(stderr, "Error: Unknown subcommand \"%s\"\n", subCommand);
        return -1;
    }
}
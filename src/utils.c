#include "../include/utils.h"

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
				arguments->subCommand = arg;
			else if (state->arg_num == 1)
				arguments->subCommandArgument = arg;
			else
				argp_usage(state);
			break;

        case ARGP_KEY_END:
			if (arguments->subCommand == NULL || arguments->subCommandArgument == NULL)
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
        syslog(LOG_ERR, "No arguments passed");
        return -1;
    }
    static struct argp argp = {options, parseOpt, 0, 0};
    memset(arguments, 0, sizeof(*arguments)); 
    argp_parse(&argp, argc, argv, ARGP_NO_HELP, 0, arguments);
    return 0;
}
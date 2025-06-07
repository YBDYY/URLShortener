#include "../include/utils.h"

error_t parseOpt(int key, char *arg, struct argp_state *state)
{
	switch (key) {
        case 'h':
            argp_state_help(state, state->out_stream, ARGP_HELP_STD_HELP | OPTION_ARG_OPTIONAL);
            break;

        case 'u':
            argp_state_help(state, state->out_stream, ARGP_HELP_USAGE | ARGP_HELP_EXIT_OK | OPTION_ARG_OPTIONAL);
            break;

        case ARGP_KEY_ARG:
            argp_usage(state);
            break;

        case ARGP_KEY_END:
            break;

        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

int loadArguments(int argc, char **argv)
{   
    if (argc == 0 || argc == 1){
        syslog(LOG_ERR, "No arguments passed");
        return -1;
    }
    static struct argp argp = {options, parseOpt, 0, 0};
    memset(&arguments, 0, sizeof(arguments)); 
    argp_parse(&argp, argc, argv, ARGP_NO_HELP, 0, &arguments);
    return 0;
}
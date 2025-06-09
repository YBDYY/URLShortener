#include "../include/utils.h"
#include "../include/hashing.h"
#include "../include/db.h"
#include "../include/core.h"

int main(int argc, char *argv[]){
	struct arguments arguments;
	if (loadArguments(argc, argv, &arguments) != 0)
		return -1;
	
	if (strcmp(arguments.subCommand, "add") == 0)
		return handleAdd(arguments.subCommandArgument);
	
	if (strcmp(arguments.subCommand, "resolve") == 0)
		return handleResolve(arguments.subCommandArgument);
	
	return 0;
}
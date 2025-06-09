#include "../include/utils.h"
#include "../include/hashing.h"
#include "../include/db.h"
#include "../include/core.h"

int main(int argc, char *argv[]){
	struct arguments arguments;
	if (loadArguments(argc, argv, &arguments) != 0)
		return -1;
	
	if (strcmp(arguments.subCommand, "add") == 0){
		int rc = handle_add(arguments.subCommandArgument);
		if (rc != SQLITE_OK)
			return rc;
	}
	if (strcmp(arguments.subCommand, "resolve") == 0){
		int rc = handle_resolve(arguments.subCommandArgument);
		if (rc != SQLITE_OK)
			return rc;
	}
	return 0;
}
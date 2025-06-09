#include "../include/utils.h"
#include "../include/hashing.h"
#include "../include/db.h"

#define URL_PATH "data/URLSHORTENER.db"

int main(int argc, char *argv[]){
	struct arguments arguments;
	if (loadArguments(argc, argv, &arguments) != 0)
		return -1;
	unsigned char hash[SHA256_DIGEST_LENGTH];
	char short_code[12];
	int rc = db_init(URL_PATH);
	if (rc != SQLITE_OK)
		return rc;
	if (strcmp(arguments.subCommand, "add") == 0){
		unsigned char *result = SHA256((const unsigned char *)arguments.subCommandArgument, strlen(arguments.subCommandArgument), hash);
		if (result == NULL){
			fprintf(stderr, "%s \n", result);
			db_close();
			return -1;
		}
		base62_encode(hash_to_int(hash), short_code, sizeof(short_code));
		rc = table_insert(short_code, arguments.subCommandArgument);
		if (rc == SQLITE_OK)
			return rc;
		return rc;
	}
	if (strcmp(arguments.subCommand, "resolve") == 0){
		rc = table_select(arguments.subCommandArgument);
		if (rc != SQLITE_OK)
			return rc;
	}
	return 0;
}
#include "../include/utils.h"
#include "../include/hashing.h"
#include "../include/db.h"

int main(int argc, char *argv[]){
	struct arguments arguments;
	loadArguments(argc, argv, &arguments);

	unsigned char hash[SHA256_DIGEST_LENGTH];
	char short_code[12];
	__u_char *result = SHA256((const unsigned char *)arguments.subCommandArgument, strlen(arguments.subCommandArgument), hash);
	if (result == NULL){
		fprintf(stderr, "%s \n", result);
		return -1;
	}
	base62_encode(hash_to_int(hash), short_code, sizeof(short_code));
	if (db_init("data/URLSHORTENER.db") != 0){
		fprintf(stderr, "Error: could not open database \n");
		db_close();
		return -1;
	}
	table_insert(short_code, arguments.subCommandArgument);
	db_close();
	return 0;
}
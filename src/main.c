#include "../include/utils.h"
#include "../include/hashing.h"

int main(int argc, char *argv[]){
	struct arguments arguments;
	loadArguments(argc, argv, &arguments);
	unsigned char hash[SHA256_DIGEST_LENGTH];
	char short_code[12];
	void* result = SHA256((const unsigned char *)arguments.subCommandArgument, strlen(arguments.subCommandArgument), hash);
	if (result == NULL)
		return -1;
	base62_encode(hash_to_int(hash), short_code, sizeof(short_code));
	
	
	return 0;
}
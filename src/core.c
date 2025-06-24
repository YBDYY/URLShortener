#include "../include/core.h"

int handleAdd(const char *url)
{
	unsigned char hash[SHA256_DIGEST_LENGTH];
	char short_code[SHORT_CODE_LENGTH];
	char salted_string[SALTED_STRING_SIZE] = {0};
	if (dbInit(URL_PATH) != SQLITE_OK) return SQLITE_ERROR;
	if (finalHashing(salted_string, url, hash, short_code) != 0) return HASHING_ERR_NULL_INPUT;
	return tableAdd(short_code, url);
}

int handleResolve(const char *short_code)
{
	int rc = dbInit(URL_PATH);
	if (rc != SQLITE_OK) return rc;
	return tableResolve(short_code);
}
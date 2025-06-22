#include "../include/core.h"

int handleAdd(const char *url)
{
	unsigned char hash[SHA256_DIGEST_LENGTH];
	char short_code[SHORT_CODE_LENGTH];
	char salted_string[SALTED_STRING_SIZE] = {0};
	if (dbInit(URL_PATH) != SQLITE_OK) return SQLITE_ERROR;
	if (finalHashing(salted_string, url, hash, short_code) != 0) return HASHING_ERR_NULL_INPUT;
	if (tableAdd(short_code, url) != 0) return SQLITE_ERROR;
	return 0;
}

int handleResolve(const char *short_code)
{
	int rc = dbInit(URL_PATH);
	if (rc != SQLITE_OK) return rc;
	rc = tableResolve(short_code);
	return rc;
}
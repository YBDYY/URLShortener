#include "../include/core.h"

#define URL_PATH "data/URLSHORTENER.db"

int handleAdd(const char *url) 
{
	unsigned char hash[SHA256_DIGEST_LENGTH];
	char short_code[12];
	int rc = dbInit(URL_PATH);
	if (rc != SQLITE_OK)
		return rc;
	unsigned char *result = SHA256((const unsigned char *)url, strlen(url), hash);
	if (result == NULL){
		fprintf(stderr, "%s \n", result);
		dbClose();
		return -1;
	}
	base62Encode(hash_to_int(hash), short_code, sizeof(short_code));
	rc = tableInsert(short_code, url);
	if (rc == SQLITE_OK)
		return rc;
	return rc;
}

int handleResolve(const char *short_code)
{
	int rc = dbInit(URL_PATH);
	if (rc != SQLITE_OK)
		return rc;
	rc = tableSelect(short_code);
	if (rc != SQLITE_OK)
		return rc;
	return rc;
}
#include "../include/core.h"

#define URL_PATH "data/URLSHORTENER.db"

int handle_add(const char *url) 
{
	unsigned char hash[SHA256_DIGEST_LENGTH];
	char short_code[12];
	int rc = db_init(URL_PATH);
	if (rc != SQLITE_OK)
		return rc;
	unsigned char *result = SHA256((const unsigned char *)url, strlen(url), hash);
	if (result == NULL){
		fprintf(stderr, "%s \n", result);
		db_close();
		return -1;
	}
	base62_encode(hash_to_int(hash), short_code, sizeof(short_code));
	rc = table_insert(short_code, url);
	if (rc == SQLITE_OK)
		return rc;
	return rc;
}

int handle_resolve(const char *short_code)
{
	int rc = db_init(URL_PATH);
	if (rc != SQLITE_OK)
		return rc;
	rc = table_select(short_code);
	if (rc != SQLITE_OK)
		return rc;
	return rc;
}
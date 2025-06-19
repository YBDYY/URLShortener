#include "../include/core.h"

int handleResolve(const char *short_code)
{
	int rc = dbInit(URL_PATH);
	if (rc != SQLITE_OK) return rc;
	rc = tableResolve(short_code);
	return rc;
}
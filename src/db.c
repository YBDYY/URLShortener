#include "../include/db.h"

sqlite3 *db = NULL;
char *zErrMsg = 0;

int callback_select(void *data, int argc, char **argv, char **azColName) {
	char **result = (char **)data;
	(*result) = strdup(argv[0]);
    return 0;
}

static int sql_execution(char *query, char *zErrMsg)
{
	int rc = sqlite3_exec(db, query, NULL, 0, &zErrMsg);
	sqlite3_free(query);
	if (rc != SQLITE_OK) {
		if (zErrMsg) sqlite3_free(zErrMsg);
		db_close();
		return SQLITE_ERROR;
	}
	db_close();
	return SQLITE_OK;
}

int db_init(const char *db_path) {
    int rc = sqlite3_open(db_path, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "sqlite3_open() failed: %s\n", sqlite3_errmsg(db));
        return rc;
    }
    return SQLITE_OK;
}

int table_insert(const char *short_code, const char *original_url)
{	
	if (!db) {
		fprintf(stderr, "Error: database not initalized \n");
		return SQLITE_DENY;
	}
	char *zErrMsg = NULL;
	char *query = sqlite3_mprintf(
    "CREATE TABLE IF NOT EXISTS PAYLOAD ("
    "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
    "SHORT_CODE TEXT NOT NULL, "
    "URL TEXT NOT NULL, "
    "TIMESTAMP DATETIME DEFAULT CURRENT_TIMESTAMP); "
    "INSERT INTO PAYLOAD (SHORT_CODE, URL) "
    "VALUES('%q', '%q');",
    short_code, original_url
	);
	return sql_execution(query, zErrMsg);
}

int table_select(const char *short_code)
{
	if (!db) {
		fprintf(stderr, "Error: database not initalized \n");
		return SQLITE_DENY;
	}
	char *query = sqlite3_mprintf(
		"SELECT URL FROM PAYLOAD WHERE SHORT_CODE = '%q';",
		short_code
	);
	char *zErrMsg = NULL;
	char *result = NULL;
	int rc = sqlite3_exec(db, query, callback_select, &result, &zErrMsg);
	sqlite3_free(query);

	if (rc != SQLITE_OK) {
		if (zErrMsg) sqlite3_free(zErrMsg);
		db_close();
		return SQLITE_ERROR;
	}

	if (result) {
		printf("URL: %s\n", result);
		free(result);
	} else {
		printf("No URL found for short code: %s\n", short_code);
	}
	db_close();
	return SQLITE_OK;
}

void db_close()
{	
	if (db) sqlite3_close(db);
	db = NULL;
}
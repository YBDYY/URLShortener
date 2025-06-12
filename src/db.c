#include "../include/db.h"

sqlite3 *db = NULL;
char *zErrMsg = 0;

static int callbackSelect(void *data, int argc, char **argv, char **azColName) {
	char **result = (char **)data;
	(*result) = strdup(argv[0]);
    return 0;
}

static int sqlExecution(char *query, char *zErrMsg)
{
	int rc = sqlite3_exec(db, query, NULL, 0, &zErrMsg);
	sqlite3_free(query);
	if (rc != SQLITE_OK) {
		if (zErrMsg) sqlite3_free(zErrMsg);
		dbClose();
		return SQLITE_ERROR;
	}
	return SQLITE_OK;
}

int dbInit(const char *db_path) {
    int rc = sqlite3_open(db_path, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "sqlite3_open() failed: %s\n", sqlite3_errmsg(db));
        return rc;
    }
	char *zErrMsg = NULL;
	char *query = sqlite3_mprintf(
    "CREATE TABLE IF NOT EXISTS URL_MAP ("
    "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
    "SHORT_CODE TEXT NOT NULL UNIQUE, "
    "URL TEXT NOT NULL, "
    "TIMESTAMP DATETIME DEFAULT CURRENT_TIMESTAMP); "
	);
    return sqlExecution(query, zErrMsg);
}

int checkDuplicate(const char *short_code)
{	
	sqlite3_stmt *stmt = NULL;
	// better to use ? then to hardcode the short_code parameter
	// sqlite3_bind_text() helps avoid SQL injection vulnerabilities.
	const char *sql = "SELECT URL FROM URL_MAP WHERE SHORT_CODE = ?;";
	int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "sqlite3_prepare_v2() failed: %s\n", sqlite3_errmsg(db));
		return rc;
	}
	rc = sqlite3_bind_text(stmt, 1, short_code, -1, SQLITE_STATIC);
	if (rc != SQLITE_OK) {
		sqlite3_finalize(stmt);
		return rc;
	}
	rc = sqlite3_step(stmt);
	if (rc == SQLITE_ROW) {
		const char *existing_url = (const char *)sqlite3_column_text(stmt, 0);
		printf("Short code exists, URL: %s\n", existing_url);
		sqlite3_finalize(stmt);
		return SQLITE_DETERMINISTIC;
	} else if (rc == SQLITE_DONE) {
		sqlite3_finalize(stmt);
		return 0;
	} else {
		sqlite3_finalize(stmt);
		return rc; 
	}
}

int tableAdd(const char *short_code, const char *original_url)
{	
	if (!db) {
		fprintf(stderr, "Error: database not initalized \n");
		return SQLITE_DENY;
	}
	if (!short_code) return -1;
	int rc = checkDuplicate(short_code);
	if (rc != SQLITE_OK){
		dbClose();
		return rc;
	}
	char *zErrMsg = NULL;
	char *query = sqlite3_mprintf(
    "INSERT INTO URL_MAP (SHORT_CODE, URL) "
    "VALUES('%q', '%q');",
    short_code, original_url
	);
	return sqlExecution(query, zErrMsg);
}

int tableResolve(const char *short_code)
{
	if (!db) {
		fprintf(stderr, "Error: database not initalized \n");
		return SQLITE_DENY;
	}
	char *query = sqlite3_mprintf(
		"SELECT URL FROM URL_MAP WHERE SHORT_CODE = '%q';",
		short_code
	);
	char *zErrMsg = NULL;
	char *result = NULL;
	int rc = sqlite3_exec(db, query, callbackSelect, &result, &zErrMsg);
	sqlite3_free(query);

	if (rc != SQLITE_OK) {
		if (zErrMsg) sqlite3_free(zErrMsg);
		dbClose();
		return SQLITE_ERROR;
	}

	if (result) {
		printf("URL: %s\n", result);
		free(result);
	} else {
		printf("No URL found for short code: %s\n", short_code);
	}
	dbClose();
	return SQLITE_OK;
}

void dbClose()
{	
	if (db) sqlite3_close(db);
	db = NULL;
}
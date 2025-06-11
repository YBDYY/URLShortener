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
	dbClose();
	return SQLITE_OK;
}

int dbInit(const char *db_path) {
    int rc = sqlite3_open(db_path, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "sqlite3_open() failed: %s\n", sqlite3_errmsg(db));
        return rc;
    }
    return SQLITE_OK;
}

// int check_URL_MAP()
// {
// 	const char *sql = "PRAGMA table_info(URL_MAP);";
// 	sqlite3_stmt *stmt;
// 	int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
// 	if (rc != SQLITE_OK) {
// 		fprintf(stderr, "PRAGMA failed: %s\n", sqlite3_errmsg(db));
// 		return rc;
// 	}
// 	while (sqlite3_step(stmt) == SQLITE_ROW) {
// 		printf("Name: %s, Type: %s\n",
// 		sqlite3_column_text(stmt, 1),
// 		sqlite3_column_text(stmt, 2));
// 	}
// 	sqlite3_finalize(stmt);
// }

int check_duplication(const char *short_code)
{	
	// int rc = check_payload();
	int rc = 0;
	if (rc != SQLITE_OK) return rc;

	sqlite3_stmt *stmt = NULL;
	const char *sql = "SELECT URL FROM URL_MAP WHERE SHORT_CODE = ?;";
	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
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

int tableInsert(const char *short_code, const char *original_url)
{	
	if (!db) {
		fprintf(stderr, "Error: database not initalized \n");
		return SQLITE_DENY;
	}
	if (!short_code) return -1;
	int rc = check_duplication(short_code);
	if (rc != SQLITE_OK){
		printf("not okay\n");
		dbClose();
		return rc;
	}
	char *zErrMsg = NULL;
	char *query = sqlite3_mprintf(
    "CREATE TABLE IF NOT EXISTS URL_MAP ("
    "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
    "SHORT_CODE TEXT NOT NULL, "
    "URL TEXT NOT NULL, "
    "TIMESTAMP DATETIME DEFAULT CURRENT_TIMESTAMP); "
    "INSERT INTO URL_MAP (SHORT_CODE, URL) "
    "VALUES('%q', '%q');",
    short_code, original_url
	);
	return sqlExecution(query, zErrMsg);
}

int tableSelect(const char *short_code)
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
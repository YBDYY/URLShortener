#include "../include/db.h"
#include "logging.h"
#include <sqlite3.h>
#include <stdio.h>

sqlite3 *db = NULL;

int dbInit(const char *db_path) 
{
	if (db != NULL) return SQLITE_OK;
    int rc = sqlite3_open(db_path, &db);
	char *zErrMsg = NULL;
	char *query = sqlite3_mprintf(
	"CREATE TABLE IF NOT EXISTS URL_MAP ("
	"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
	"SHORT_CODE TEXT NOT NULL UNIQUE, "
	"URL TEXT NOT NULL, "
	"TIMESTAMP DATETIME DEFAULT CURRENT_TIMESTAMP); "
	);
    if (rc != SQLITE_OK) {
        log_error("sqlite3_open() failed: %s\n", sqlite3_errmsg(db));
        return rc;
    }
	log_info("Database opened at path: %s", db_path);
	rc = sqlite3_exec(db, query, NULL, 0, &zErrMsg);
	sqlite3_free(query);
	if (rc != SQLITE_OK) {
		log_error("sqlite3_exec() failed: %s", zErrMsg);
		if (zErrMsg) sqlite3_free(zErrMsg);
		dbClose();
		return SQLITE_ERROR;
	}
	log_info("URL_MAP table ensured in database");
	return SQLITE_OK;
}

int checkDuplicate(const char *short_code)
{	
	int rc = 0;
	sqlite3_stmt *stmt = NULL;
	const char *sql = "SELECT URL FROM URL_MAP WHERE SHORT_CODE = ?;";
	if (rc != SQLITE_OK) return rc;
	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		log_error("sqlite3_prepare_v2() failed: %s\n", sqlite3_errmsg(db));
		return rc;
	}
	rc = sqlite3_bind_text(stmt, 1, short_code, -1, SQLITE_STATIC);
	if (rc != SQLITE_OK) {
		log_error("sqlite3_bind_text() failed: %s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		return rc;
	}
	rc = sqlite3_step(stmt);
	if (rc == SQLITE_ROW) {
		const char *existing_url = (const char *)sqlite3_column_text(stmt, 0);
		log_info("Short code '%s' already exists for URL '%s'", short_code, existing_url);
		sqlite3_finalize(stmt);
		return SQLITE_DETERMINISTIC;
	} else if (rc == SQLITE_DONE) {
		log_info("Short code '%s' is available", short_code);
		sqlite3_finalize(stmt);
		return SQLITE_OK;
	} else {
		log_error("sqlite3_step() failed for short_code='%s': %s", short_code, sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		return rc; 
	}
}

int tableAdd(const char *short_code, const char *original_url)
{	
	sqlite3_stmt *stmt = NULL;
	char *sql = "INSERT INTO URL_MAP (URL, SHORT_CODE) VALUES(?, ?);";
	int rc = 0;
	if (!db) {
		log_error("Error: database not initalized \n");
		return SQLITE_DENY;
	}
	if (short_code == NULL) {
		log_error("Error: short_code is NULL");
		return SQLITE_MISUSE;
	}
	rc = checkDuplicate(short_code);
	if (rc != SQLITE_OK) return rc;
	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		log_error("sqlite3_prepare_v2() failed: %s\n", sqlite3_errmsg(db));
		return rc;
	}
	sqlite3_bind_text(stmt, 1, original_url, -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, short_code, -1, SQLITE_STATIC);
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		log_error("sqlite3_step() failed: %s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		return rc;
	}
	sqlite3_finalize(stmt);
	return SQLITE_OK;
}

int tableResolve(const char *short_code)
{	
	sqlite3_stmt *stmt = NULL;
	int rc = 0;
	char *sql = "SELECT URL FROM URL_MAP WHERE SHORT_CODE = ?;";
	if (!db) {
		log_error("Error: database not initialized \n");
		return SQLITE_DENY;
	}
	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		log_error("sqlite3_prepare_v2() failed: %s\n", sqlite3_errmsg(db));
		dbClose();
		return rc;
	}
	rc = sqlite3_bind_text(stmt, 1, short_code, -1, SQLITE_STATIC);
	if (rc != SQLITE_OK) {
		log_error("sqlite3_bind_text() failed: %s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		dbClose();
		return rc;
	}
	rc = sqlite3_step(stmt);
	if (rc == SQLITE_ROW) {
		const unsigned char *url = sqlite3_column_text(stmt, 0);
		log_info("Short code '%s' resolves to URL '%s'", short_code, url);
		dbClose();
		sqlite3_finalize(stmt);
		return SQLITE_OK;
	} else if (rc == SQLITE_DONE) {
		log_error("Short code '%s' not found in database", short_code);
		dbClose();
		sqlite3_finalize(stmt);
		return SQLITE_NOTFOUND;
	} else {
		log_error("sqlite3_step() failed for short_code='%s': %s", short_code, sqlite3_errmsg(db));
		dbClose();
		sqlite3_finalize(stmt);
		return rc;
	}
}

void dbClose()
{	
	if (db != NULL) sqlite3_close(db);
	db = NULL;
}

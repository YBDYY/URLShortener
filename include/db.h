#pragma once

#include <sqlite3.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERR_DB_INIT -1
#define ERR_DB_PROCESS -2

int dbInit(const char *db_path);

int tableAdd(const char *short_code, const char *original_url);

int tableResolve(const char *short_code);

int checkDuplicate(const char *short_code);

void dbClose();

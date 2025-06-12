#pragma once

#include <sqlite3.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int dbInit(const char *db_path);

int checkURLMAP();

int check_duplication(const char *short_code);

int tableInsert(const char *short_code, const char *original_url);

int tableSelect(const char *short_code);

void dbClose();

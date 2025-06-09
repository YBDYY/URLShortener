#pragma once

#include <sqlite3.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int db_init(const char *db_path);

int table_insert(const char *short_code, const char *original_url);

int table_select(const char *short_code);

void db_close();

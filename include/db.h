#pragma once

#include <sqlite3.h>
#include <stddef.h>
#include <stdio.h>

int db_init(const char *db_path);

int table_insert(const char *short_code, const char *original_url);

int table_lookup(const char *short_code, char *output_url, size_t output_size);

void db_close();

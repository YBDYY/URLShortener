#pragma once

#include "logging.h"
#include <sqlite3.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int dbInit(const char *db_path);

int tableAdd(const char *short_code, const char *original_url);

int tableResolve(const char *short_code);

int checkDuplicate(const char *short_code);

void dbClose();

#pragma once

#include "../include/hashing.h"
#include "../include/db.h"

#define URL_PATH "data/URLSHORTENER.db"

int handleAdd(const char *url);

int handleResolve(const char *short_code);
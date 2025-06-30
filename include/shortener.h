#pragma once

#define URL_PATH "data/URLSHORTENER.db"

int handleAdd(const char *url);

int handleResolve(const char *short_code);

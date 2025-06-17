#pragma once
#include <stdio.h>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <string.h>
#include <time.h>

#define ERR_HASHING -2
#define SHORT_CODE_LENGTH 12
#define SALTED_STRING_SIZE 512

void base62Encode(uint64_t value, char *output, size_t output_size);

uint64_t hash_to_int(const unsigned char *hash);

int finalHashing(char *salted_string, const char *url, unsigned char *hash, char *short_code);
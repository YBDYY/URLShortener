#pragma once
#include <stdio.h>
#include <openssl/sha.h>
#include <string.h>

void base62_encode(uint64_t value, char *output, size_t output_size);

uint64_t hash_to_int(const unsigned char *hash);
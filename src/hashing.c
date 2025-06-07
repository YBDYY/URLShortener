#include "../include/hashing.h"

uint64_t hash_to_int(const unsigned char *hash)
{
    uint64_t val = 0;
    for (int i = 0; i < 6; i++)
        val = (val << 8) | hash[i];
    return val;
}

void base62_encode(uint64_t value, char *output, size_t output_size) {
	const char base62_chars[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    char buffer[12];
    int i = 0;
    do {
        buffer[i++] = base62_chars[value % 62];
        value /= 62;
    } while (value > 0 && i < (int)sizeof(buffer) - 1);

    int j = 0;
    while (i > 0 && j < (int)output_size - 1) {
        output[j++] = buffer[--i];
    }
    output[j] = '\0';
}

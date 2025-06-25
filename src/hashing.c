#include "../include/hashing.h"

uint64_t hash_to_int(const unsigned char *hash)
{
    uint64_t value = 0;
    for (int i = 0; i < 6; i++)
        value = (value << 8) | hash[i];
    return value;
}

void base62Encode(uint64_t value, char *output, size_t output_size) {
	const char base62_chars[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    char buffer[12];
    int i = 0;
    do {
        buffer[i++] = base62_chars[value % 62];
        value /= 62;
    } while (value > 0 && i < (int)sizeof(buffer) - 1);
    int j = 0;
    while (i > 0 && j < (int)output_size - 1) 
        output[j++] = buffer[--i];
    output[j] = '\0';
}

int finalHashing(char *salted_string, const char *url, unsigned char *hash, char *short_code)
{
    snprintf(salted_string, SALTED_STRING_SIZE, "%s%ld%d", url, time(NULL), rand());
	unsigned char *result = SHA256((const unsigned char *)url, strlen(url), hash);
	if (result == NULL) {
        log_error("SHA256 hashing failed for URL: %s", url);
        return HASHING_ERR_NULL_INPUT;
    }
	base62Encode(hash_to_int(hash), short_code, sizeof(short_code));
    return HASHING_SUCCESS;
}
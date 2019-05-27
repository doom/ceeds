/*
** Created by doom on 25/05/19.
*/

#include <ceeds/hash_utils.h>

hash_value_t fnv_one64(const char *data, size_t len)
{
    const unsigned char *ptr = (const unsigned char *)data;
    uint64_t hash;
    size_t i;

    hash = 0xcbf29ce484222325;
    i = 0;
    while (i < len) {
        hash *= 0x100000001b3;
        hash ^= ptr[i++];
    }
    return (hash);
}

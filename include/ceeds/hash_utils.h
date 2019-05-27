/*
** Created by doom on 25/05/19.
*/

#ifndef CEEDS_HASH_UTILS_H
#define CEEDS_HASH_UTILS_H

#include <ceeds/core.h>

typedef uint64_t hash_value_t;

hash_value_t fnv_one64(const char *data, size_t len);

#endif /* !CEEDS_HASH_UTILS_H */

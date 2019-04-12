/*
** Created by doom on 11/04/19.
*/

#ifndef CEEDS_ASCII_SET_H
#define CEEDS_ASCII_SET_H

#include <ceeds/bitmanip.h>

/**
 * Sets of extended ASCII characters
 */

typedef struct ascii_set
{
    unsigned int buffer[256 / bitsizeof(unsigned int)];
} ascii_set_t;

/**
 * Clear an ASCII set
 *
 * @param[out]      set     the set to clear
 */
static inline void ascii_set_clear(ascii_set_t *set)
{
    memset(set->buffer, 0, sizeof(set->buffer));
}

/**
 * Add characters to an ASCII set
 *
 * @param[out]      set     the set to insert into
 * @param[in]       chars   an array containing the characters to insert (does not have to be NUL-terminated)
 * @param[in]       len     the length of the @p chars array
 */
static inline void ascii_set_add_chars(ascii_set_t *set, const char *chars, size_t len)
{
    while (len--) {
        set_bit(set->buffer, (unsigned char)*chars);
        ++chars;
    }
}

/**
 * Initialize an ASCII set using given characters
 *
 * @param[out]      set     the set to initialize
 * @param[in]       chars   an array containing the characters to initialize with (does not have to be NUL-terminated)
 * @param[in]       len     the length of the @p chars array
 */
static inline void ascii_set_init(ascii_set_t *set, const char *chars, size_t len)
{
    ascii_set_clear(set);
    ascii_set_add_chars(set, chars, len);
}

/**
 * Test whether an ASCII set contains a given character
 *
 * @param[in]       set     the set
 * @param[in]       c       the character to test
 * @return                  true if the character is found in the set, false otherwise
 */
static inline bool ascii_set_contains(const ascii_set_t *set, char c)
{
    return is_bit_set(set->buffer, (unsigned char)c);
}

/**
 * Get the union of two given sets
 *
 * @param[out]      set     the set in which the result will be stored (does not have to be initialized)
 * @param[in]       a       the first set to consider in the union
 * @param[in]       b       the second set to consider in the union
 */
static inline void ascii_set_union(ascii_set_t *set, const ascii_set_t *a, const ascii_set_t *b)
{
    for (size_t i = 0; i < array_length(set->buffer); ++i) {
        set->buffer[i] = a->buffer[i] | b->buffer[i];
    }
}

/**
 * Get the intersection of two given sets
 *
 * @param[out]      set     the set in which the result will be stored (does not have to be initialized)
 * @param[in]       a       the first set to consider in the intersection
 * @param[in]       b       the second set to consider in the intersection
 */
static inline void ascii_set_intersection(ascii_set_t *set, const ascii_set_t *a, const ascii_set_t *b)
{
    for (size_t i = 0; i < array_length(set->buffer); ++i) {
        set->buffer[i] = a->buffer[i] & b->buffer[i];
    }
}

/**
 * Get the difference of two given sets
 *
 * @param[out]      set     the set in which the result will be stored (does not have to be initialized)
 * @param[in]       a       the first set to consider in the difference
 * @param[in]       b       the second set to consider in the difference
 */
static inline void ascii_set_difference(ascii_set_t *set, const ascii_set_t *a, const ascii_set_t *b)
{
    for (size_t i = 0; i < array_length(set->buffer); ++i) {
        set->buffer[i] = a->buffer[i] & ~b->buffer[i];
    }
}

/**
 * Get the complement of a given set
 *
 * @param[out]      set     the set in which the result will be stored (does not have to be initialized)
 * @param[in]       a       the set whose complement should be calculated
 */
static inline void ascii_set_complement(ascii_set_t *set, const ascii_set_t *a)
{
    for (size_t i = 0; i < array_length(set->buffer); ++i) {
        set->buffer[i] = ~a->buffer[i];
    }
}

#endif /* !CEEDS_ASCII_SET_H */

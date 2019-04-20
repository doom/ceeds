/*
** Created by doom on 18/04/19.
*/

#ifndef CEEDS_STR_H
#define CEEDS_STR_H

#include <ceeds/core.h>
#include <ceeds/memory.h>
#include <ceeds/string_utils.h>

/**
 * Fixed-sized, optionally-owning strings
 */

typedef struct
{
    memory_allocator_handle_t allocator_handle;
    union
    {
        const char *const_str;
        char *str;
    };
    size_t length;
} str_t;

/**
 * Format specifier to print strings using functions from the printf family
 */
#define PRIstr      ".*s"

/**
 * Macro to pass the arguments expected by the PRIstr format specifier
 *
 * @param[in]       str     the string to print
 */
#define str_printf_format(str)      (int)(str).length, (str).const_str

/**
 * Create a str_t from a pointer/length couple
 *
 * @param[in]       str     the string to create from
 * @param[in]       len     the length to use
 * @return                  a string whose content is equivalent to @p str, and whose length is @p len
 *
 * @pre                     the @p len first bytes of @p str must be accessible
 */
#define str_with_length(str, len)                                                               \
    (str_t){.allocator_handle = static_allocator_handle(), .const_str = (str), .length = (len)}

/**
 * Create a str_t from a C (i.e. NUL-terminated) string
 *
 * @param[in]       str     the string to create from
 * @return                  a string whose content is equivalent to @p str, and whose length is strlen(str)
 *
 * @pre                     @p str should be NUL-terminated
 */
#define str_from_c_string(str)                                                                  \
    str_with_length(str, strlen(str))

/**
 * Create a str_t from a string literal
 *
 * @param[in]       str     the literal string to create from
 * @return                  a string whose value is equivalent to @p str
 */
#define str_from_literal(str)                                                                   \
    str_with_length(str"", sizeof(str) - 1)

/**
 * Create an empty str_t
 *
 * @return                  an empty string
 */
#define str_empty()                                                                             \
    str_from_literal("")

/**
 * Sentinel value used for functions returning a position
 */
#define str_npos                ((size_t)-1)

/**
 * Destroy a str_t, deallocating its content
 *
 * @param[in,out]   str     the string to destroy
 */
static inline void str_destroy(str_t *str)
{
    allocator_delete(str->allocator_handle, str->str);
}

/**
 * Compare two strings
 *
 * @param[in]       s1      the first string
 * @param[in]       s2      the second string
 * @return                  0 if the strings are equal, otherwise 1 if @p a is bigger, otherwise -1
 */
static inline int str_cmp(const str_t s1, const str_t s2)
{
    return memcmp(s1.const_str, s2.const_str, MIN(s1.length, s2.length)) ?: CMP(s1.length, s2.length);
}

/**
 * Check two strings for equality
 *
 * @param[in]       str1    the first string
 * @param[in]       str2    the second string
 * @return                  true if @p a and @p b have the same length and the same content, false otherwise
 */
static inline bool str_equal(const str_t str1, const str_t str2)
{
    return str1.length != str2.length ? false : memcmp(str1.const_str, str2.const_str, str1.length) == 0;
}

/**
 * Find a given substring in a given string
 *
 * @param[in]       str1    the string to search into
 * @param[in]       str2    the substring to search for
 * @return                  the position of the substring in @p a if found, str_npos otherwise
 */
static inline size_t str_find(const str_t str1, const str_t str2)
{
    const char *result = memmem(str1.const_str, str1.length, str2.const_str, str2.length);

    return result ? (size_t)(result - str1.const_str) : str_npos;
}

/**
 * Find a given character in a given string
 *
 * @param[in]       str     the string to search into
 * @param[in]       c       the character to search for
 * @return                  the position of the character in @p a if found, str_npos otherwise
 */
static inline size_t str_findc(const str_t str, char c)
{
    const char *result = memchr(str.const_str, c, str.length);

    return result ? (size_t)(result - str.const_str) : str_npos;
}

/**
 * Find a given substring in a given string, searching backwards
 *
 * @param[in]       str1    the string to search into
 * @param[in]       str2    the substring to search for
 * @return                  the position of the substring in @p a if found, str_npos otherwise
 */
static inline size_t str_rfind(const str_t str1, const str_t str2)
{
    const char *result = memrmem(str1.const_str, str1.length, str2.const_str, str2.length);

    return result ? (size_t)(result - str1.const_str) : str_npos;
}

/**
 * Find a given character in a given string, searching backwards
 *
 * @param[in]       str1    the string to search into
 * @param[in]       c       the character to search for
 * @return                  the position of the character in @p a if found, str_npos otherwise
 */
static inline size_t str_rfindc(const str_t str1, char c)
{
    const char *result = memrchr(str1.const_str, c, str1.length);

    return result ? (size_t)(result - str1.const_str) : str_npos;
}

/**
 * Check whether a string starts with a given substring
 *
 * @param[in]       str1    the string to check into
 * @param[in]       str2    the substring
 * @return                  true if @p str1 starts with @p str2, false otherwise
 */
static inline bool str_startswith(const str_t str1, const str_t str2)
{
    return str1.length >= str2.length && memcmp(str1.const_str, str2.const_str, str2.length) == 0;
}

/**
 * Check whether a string ends with a given substring
 *
 * @param[in]       str1    the string to check into
 * @param[in]       str2    the substring
 * @return                  true if @p str1 ends with @p str2, false otherwise
 */
static inline bool str_endswith(const str_t str1, const str_t str2)
{
    return str1.length >= str2.length &&
           memcmp(str1.const_str + str1.length - str2.length, str2.const_str, str2.length) == 0;
}

#endif /* !CEEDS_STR_H */

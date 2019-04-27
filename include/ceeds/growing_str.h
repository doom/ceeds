/*
** Created by doom on 23/04/19.
*/

#ifndef CEEDS_GROWING_STR_H
#define CEEDS_GROWING_STR_H

#include <ceeds/core.h>
#include <ceeds/memory.h>

/**
 * Automatically growing strings, for string building and formatting
 *
 * These are always valid and NUL-terminated.
 */

typedef struct
{
    memory_allocator_handle_t allocator_handle;
    char *str;
    size_t length;
    size_t capacity;
} growing_str_t;

extern char _gs_empty_marker[1];

/**
 * Create a growing_str
 *
 * @param[in]       alloc_handle    the allocator handle to be used by the string
 * @param[in]       buffer          the buffer to use as the underlying array
 * @param[in]       length          the initial length
 * @param[in]       capacity        the initial capacity of the buffer
 *
 * @pre                             @p buffer must be capable of storing at least @p capacity elements
 */
#define gs_with_buffer(alloc_handle, buffer, length, capacity)              \
    (growing_str_t){alloc_handle, buffer, length, capacity}

/**
 * Create an empty growing_str
 *
 * @param[in]       alloc_handle    the allocator handle to be used by the vector
 */
#define gs_empty(alloc_handle)      gs_with_buffer(alloc_handle, _gs_empty_marker, 0, 1)

/**
 * Destroy a growing_str
 *
 * @param[in,out]   str             a pointer to the string to destroy
 */
static inline void gs_destroy(growing_str_t *str)
{
    if (str->str != _gs_empty_marker) {
        allocator_delete(str->allocator_handle, str->str);
    }
}

void _gs_reallocate(growing_str_t *str, size_t desired_capacity);

/**
 * Increase the capacity of a growing_str to be at least equal to a given amount
 *
 * @param[in,out]   str             the string whose capacity is to be increased
 * @param[in]       capacity        the new capacity
 */
static inline void gs_reserve(growing_str_t *str, size_t capacity)
{
    if (capacity > str->capacity) {
        _gs_reallocate(str, capacity);
    }
}

/**
 * Truncate a growing_str at a given position (i.e. set its length and NUL-terminate it)
 *
 * @param[in,out]   str             the string to truncate
 * @param[in]       at              the position at which to truncate
 */
static inline void gs_truncate(growing_str_t *str, size_t at)
{
    if (str->str != _gs_empty_marker) {
        str->str[at] = '\0';
        str->length = at;
    }
}

/**
 * Append a character to a growing_str
 *
 * @param[in,out]   str             the string to append to
 * @param[in]       add             the character to append
 */
static inline void gs_append_char(growing_str_t *str, char add)
{
    gs_reserve(str, str->length + 1 + 1);
    str->str[str->length] = add;
    gs_truncate(str, str->length + 1);
}

/**
 * Append multiple occurrences of a character to a growing_str
 *
 * @param[in,out]   str             the string to append to
 * @param[in]       n               the number of times the character should be appended
 * @param[in]       add             the character to append
 */
static inline void gs_append_n_char(growing_str_t *str, size_t n, char add)
{
    gs_reserve(str, str->length + n + 1);
    memset(str->str + str->length, add, n);
    gs_truncate(str, str->length + n);
}

/**
 * Append a buffer to a growing_str
 *
 * @param[in,out]   str             the string to append to
 * @param[in]       add             the buffer to append
 * @param[in]       len             the length of the buffer to append
 */
static inline void gs_append_buffer(growing_str_t *str, const char *add, size_t len)
{
    gs_reserve(str, str->length + len + 1);
    memcpy(str->str + str->length, add, len);
    gs_truncate(str, str->length + len);
}

/**
 * Append a growing_str to a growing_str
 *
 * @param[in,out]   str             the string to append to
 * @param[in]       add             the string to append
 */
static inline void gs_append(growing_str_t *str, const growing_str_t *add)
{
    gs_append_buffer(str, add->str, add->length);
}

/**
 * Append a C string to a growing_str
 *
 * @param[in,out]   str             the string to append to
 * @param[in]       add             the C string to add
 *
 * @pre                             @p add must be a valid C string, i.e. it must be NUL-terminated
 */
static inline void gs_append_c_string(growing_str_t *str, const char *add)
{
    gs_append_buffer(str, add, strlen(add));
}

/**
 * Append a formatted string to a growing_str
 *
 * @param[in,out]   str             the string to append to
 * @param[in]       fmt             the format string
 * @param[in]       ...             the arguments to use when formatting
 *
 * @pre                             @p fmt should be a valid format string for functions of the printf family
 * @pre                             the arguments in @p ... should match the format specified by @p fmt
 */
void gs_append_formatted(growing_str_t *str, const char *fmt, ...) _format_printf_(2, 3);

/**
 * Insert a character in a growing_str at a given position
 *
 * @param[in,out]   str             the string to append to
 * @param[in]       pos             the position at which to insert
 * @param[in]       add             the character to append
 *
 * @pre                             @p str must have a length of at least @p pos characters
 */
static inline void gs_insert_char(growing_str_t *str, size_t pos, char add)
{
    gs_reserve(str, str->length + 1 + 1);
    memmove(str->str + pos + 1, str->str + pos, str->length - pos);
    str->str[pos] = add;
    gs_truncate(str, str->length + 1);
}

/**
 * Insert multiple occurrences of a character in a growing_str at a given position
 *
 * @param[in,out]   str             the string to append to
 * @param[in]       pos             the position at which to insert
 * @param[in]       n               the number of times the character should be appended
 * @param[in]       add             the character to append
 *
 * @pre                             @p str must have a length of at least @p pos characters
 */
static inline void gs_insert_n_char(growing_str_t *str, size_t pos, size_t n, char add)
{
    gs_reserve(str, str->length + n + 1);
    memmove(str->str + pos + n, str->str + pos, str->length - pos);
    memset(str->str + pos, add, n);
    gs_truncate(str, str->length + n);
}

/**
 * Insert a buffer in a growing_str at a given position
 *
 * @param[in,out]   str             the string to append to
 * @param[in]       pos             the position at which to insert
 * @param[in]       add             the buffer to append
 * @param[in]       len             the length of the buffer to append
 *
 * @pre                             @p str must have a length of at least @p pos characters
 */
static inline void gs_insert_buffer(growing_str_t *str, size_t pos, const char *add, size_t len)
{
    gs_reserve(str, str->length + len + 1);
    memmove(str->str + pos + len, str->str + pos, str->length - pos);
    memcpy(str->str + pos, add, len);
    gs_truncate(str, str->length + len);
}

/**
 * Insert a growing_str in a growing_str at a given position
 *
 * @param[in,out]   str             the string to append to
 * @param[in]       pos             the position at which to insert
 * @param[in]       add             the string to append
 *
 * @pre                             @p str must have a length of at least @p pos characters
 */
static inline void gs_insert(growing_str_t *str, size_t pos, const growing_str_t *add)
{
    gs_insert_buffer(str, pos, add->str, add->length);
}

/**
 * Insert a C string in a growing_str at a given position
 *
 * @param[in,out]   str             the string to append to
 * @param[in]       pos             the position at which to insert
 * @param[in]       add             the C string to add
 *
 * @pre                             @p str must have a length of at least @p pos characters
 * @pre                             @p add must be a valid C string, i.e. it must be NUL-terminated
 */
static inline void gs_insert_c_string(growing_str_t *str, size_t pos, const char *add)
{
    gs_insert_buffer(str, pos, add, strlen(add));
}

/**
 * Insert a formatted string in a growing_str at a given position
 *
 * @param[in,out]   str             the string to append to
 * @param[in]       pos             the position at which to insert
 * @param[in]       fmt             the format string
 * @param[in]       ...             the arguments to use when formatting
 *
 * @pre                             @p str must have a length of at least @p pos characters
 * @pre                             @p fmt should be a valid format string for functions of the printf family
 * @pre                             the arguments in @p ... should match the format specified by @p fmt
 */
void gs_insert_formatted(growing_str_t *str, size_t pos, const char *fmt, ...) _format_printf_(3, 4);

#endif /* !CEEDS_GROWING_STR_H */

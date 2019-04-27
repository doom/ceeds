/*
** Created by doom on 23/04/19.
*/

#include <ceeds/growing_str.h>

char _gs_empty_marker[1] = "";

void _gs_reallocate(growing_str_t *str, size_t desired_capacity)
{
    desired_capacity = MAX(desired_capacity, str->capacity * 2);

    if (str->str == _gs_empty_marker) {
        str->str = allocator_new_array(str->allocator_handle, char, desired_capacity);
        str->str[0] = '\0';
    } else {
        str->str = (char *)allocator_resize_array(
            str->allocator_handle,
            str->str,
            char,
            str->capacity,
            desired_capacity
        );
    }
    str->capacity = desired_capacity;
}

static void gs_append_vformatted(growing_str_t *str, const char *fmt, va_list ap)
{
    va_list cpy;
    int result;
    size_t available = str->capacity - str->length;

    va_copy(cpy, ap);
    result = vsnprintf(str->str + str->length, available, fmt, cpy);
    va_end(cpy);

    if ((size_t)result < available) {
        gs_truncate(str, str->length + (size_t)result);
    } else {
        gs_reserve(str, str->capacity + (size_t)result);
        vsnprintf(str->str + str->length, (size_t)result + 1, fmt, ap);
        gs_truncate(str, str->length + (size_t)result);
    }
}

void gs_append_formatted(growing_str_t *str, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    gs_append_vformatted(str, fmt, ap);
    va_end(ap);
}

static void gs_insert_vformatted(growing_str_t *str, size_t pos, const char *fmt, va_list ap)
{
    va_list cpy;
    int result;
    char bk;

    va_copy(cpy, ap);
    result = vsnprintf(NULL, 0, fmt, cpy);
    va_end(cpy);

    gs_reserve(str, str->length + (size_t)result);
    memmove(str->str + pos + (size_t)result, str->str + pos, str->length - pos);
    /* Make a backup of the byte because vsnprintf will overwrite it with a NUL byte */
    bk = str->str[pos + (size_t)result];
    /* The "result + 1" is needed because vsnprintf includes the NUL byte in its parameters
     * but excludes it in its return value */
    vsnprintf(str->str + pos, (size_t)result + 1, fmt, ap);
    str->str[pos + (size_t)result] = bk;
    gs_truncate(str, str->length + (size_t)result);
}

void gs_insert_formatted(growing_str_t *str, size_t pos, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    gs_insert_vformatted(str, pos, fmt, ap);
    va_end(ap);
}

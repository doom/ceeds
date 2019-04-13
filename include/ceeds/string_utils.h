/*
** Created by doom on 12/04/19.
*/

#ifndef CEEDS_STRING_UTILS_H
#define CEEDS_STRING_UTILS_H

#include <ceeds/core.h>

/**
 * Skip characters accepted by isspace(3) until one is rejected
 *
 * @param[in]       str         the string to process
 * @return                      a pointer to the first non-space character found, or the end of the string
 */
char *skipspaces(const char *str);

/**
 * Skip characters accepted by isblank(3) until one is rejected
 *
 * @param[in]       str         the string to process
 * @return                      a pointer to the first non-blank character found, or the end of the string
 */
char *skipblanks(const char *str);

/**
 * Skip to the next word in the given string (i.e. skip over the current word, and then some spaces)
 *
 * @param[in]       str         the string to process
 * @return                      a pointer to the first character of the next word found, or the end of the string
 */
char *nextword(const char *str);

/**
 * Locate a memory area containing a given sequence of bytes, searching backwards
 *
 * @param[in]       haystack    a pointer to the beginning of the memory area to search into
 * @param[in]       hlen        the length of the memory area to search into
 * @param[in]       needle      a pointer to the beginning of the sequence of bytes to search
 * @param[in]       nlen        the length of the sequence of bytes to search
 * @return                      a pointer to the beginning of the matching sequence if found, NULL otherwise
 */
void *memrmem(const void *haystack, size_t hlen, const void *needle, size_t nlen);

/**
 * Copy bytes from a given memory area to another, adding an extra NUL byte at the end
 *
 * @param[out]      dest        a pointer to the beginning of the destination area
 * @param[in]       src         a pointer to the beginning of the source area
 * @param[in]       len         the number of bytes to copy
 * @return                      @p dest
 *
 * @pre                         the given areas must not overlap
 */
static inline void *memcpy_nulterm(void *dest, const void *src, size_t len)
{
    char *ptr = mempcpy(dest, src, len);
    *ptr = '\0';
    return dest;
}

/**
 * Copy bytes from a given memory area to another, adding an extra NUL byte at the end
 *
 * @param[out]      dest        a pointer to the beginning of the destination area
 * @param[in]       src         a pointer to the beginning of the source area
 * @param[in]       len         the number of bytes to copy
 * @return                      @p dest + @p len
 *
 * @pre                         the given areas must not overlap
 */
static inline void *mempcpy_nulterm(void *dest, const void *src, size_t len)
{
    char *ptr = mempcpy(dest, src, len);
    *ptr = '\0';
    return ptr;
}

#endif /* !CEEDS_STRING_UTILS_H */

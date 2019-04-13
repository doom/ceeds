/*
** Created by doom on 12/04/19.
*/

#include <ctype.h>
#include <ceeds/string_utils.h>

char *skipspaces(const char *str)
{
    while (isspace(*str)) {
        ++str;
    }
    return (char *)str;
}

char *skipblanks(const char *str)
{
    while (isblank(*str)) {
        ++str;
    }
    return (char *)str;
}

char *nextword(const char *str)
{
    while (*str && !isspace(*str)) {
        ++str;
    }
    while (isspace(*str)) {
        ++str;
    }
    return (char *)str;
}

void *memrmem(const void *haystack, size_t hlen, const void *needle, size_t nlen)
{
    if (hlen >= nlen) {
        const char *haystack_ptr = haystack;

        for (const char *cur = (const char *)haystack + hlen - nlen; cur >= haystack_ptr; --cur) {
            if (memcmp(cur, needle, nlen) == 0) {
                return (void *)cur;
            }
        }
    }
    return NULL;
}

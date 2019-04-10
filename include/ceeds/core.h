/*
** Created by doom on 08/04/19.
*/

#ifndef CEEDS_CORE_H
#define CEEDS_CORE_H

#include <assert.h>
#include <limits.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define likely(x)                   (__builtin_expect(!!(x), 1))
#define unlikely(x)                 (__builtin_expect((x), 0))

#define _always_inline_             __attribute__((always_inline)) inline
#define _unused_                    __attribute__((unused))
#define _pure_                      __attribute__((pure))
#define _noreturn_                  __attribute__((noreturn))
#define _aligned_(x)                __attribute__((aligned(x)))
#define _cleanup_(f)                __attribute__((cleanup(f)))
#define _constructor_               __attribute__((constructor))
#define _destructor_                __attribute__((destructor))
#define _format_printf_(ifmt, iarg) __attribute__((format(printf, ifmt, iarg)))

#define MIN(a, b)                                                           \
    ({                                                                      \
        typeof(a) __a = (a);                                                \
        typeof(b) __b = (b);                                                \
                                                                            \
        __a < __b ? __a : __b;                                              \
    })

#define MAX(a, b)                                                           \
    ({                                                                      \
        typeof(a) __a = (a);                                                \
        typeof(b) __b = (b);                                                \
                                                                            \
        __a < __b ? __b : __a;                                              \
    })

#define CMP(a, b)                                                           \
    ({                                                                      \
        typeof(a) __a = (a);                                                \
        typeof(b) __b = (b);                                                \
                                                                            \
        (__b < __a) - (__a < __b);                                          \
    })

#define SWAP(a, b)                                                          \
    ({                                                                      \
        typeof(*(a)) __c = *(a);                                            \
                                                                            \
        *(a) = *(b);                                                        \
        *(b) = __c;                                                         \
    })

#define array_length(arr)           (sizeof(arr) / sizeof((arr)[0]))

#define try(x, neq, ret)                                                    \
    ({                                                                      \
        typeof(x) __x = (x);                                                \
                                                                            \
        if unlikely(__x  == ret) {                                          \
            return (ret);                                                   \
        }                                                                   \
        __x;                                                                \
    })

#define try_null(x)                 try(x, NULL, NULL)
#define try_neg(x)                  try(x, -1, -1)
#define try_negnull(x)              try(n, -1, NULL)
#define try_nullneg(x)              try(n, NULL, -1)

/**
 * Get the struct object containing a given pointer-to-member
 *
 * @param[in]       ptr         the pointer to the member of the object to retrieve
 * @param           T           the type of the element to get
 * @param           f           the name of the field in the @p T type to which @p ptr points to
 */
#define container_of(ptr, T, f)     ((T *)((char *)ptr - offsetof(T, f)))

#endif /* !CEEDS_CORE_H */

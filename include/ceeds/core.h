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

#endif /* !CEEDS_CORE_H */

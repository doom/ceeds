/*
** Created by doom on 08/04/19.
*/

#ifndef CEEDS_UNIT_TESTS_H
#define CEEDS_UNIT_TESTS_H

#include <stdio.h>
#include <string.h>

#ifndef ut_printf
#define ut_printf(x, ...)           printf(x, ##__VA_ARGS__)
#endif

#define GREEN                       "\033[32m"
#define RED                         "\033[31m"
#define CYAN                        "\033[36m"
#define RESET                       "\033[0m"

#define ut_set_color(x)             ut_printf(x)

#ifndef ut_on_fail
#define ut_on_fail(x)
#endif

#define _ut_unused_                 __attribute__((unused))

#define ut_sizeof_array(x)          (sizeof(x) / sizeof((x)[0]))

#define _ut_STR2(x)                 #x
#define _ut_STR(x)                  _ut_STR2(x)

#ifdef __FILENAME__
#define _ut_FILE                    _ut_STR(__FILENAME__)
#else
#define _ut_FILE                    __FILE__
#endif

#define _ut_LINE                    _ut_STR(__LINE__)

typedef struct ut_test_s
{
    const char *name;
    void (*run)(struct ut_test_s *);
    unsigned int passed;
    unsigned int err;
} ut_test_t;

typedef struct
{
    const char *name;
    unsigned int nbtests;
    ut_test_t *const *tests;
} ut_group_t;

#define _ut_testimpl(name)          _testimpl_##name
#define _ut_testname(name)          _test_##name

#define ut_test(name)                                                       \
    static void _ut_testimpl(name)(ut_test_t *);                            \
                                                                            \
    static _ut_unused_ ut_test_t _ut_testname(name) = {                     \
        #name,                                                              \
        &_ut_testimpl(name),                                                \
        0, 0,                                                               \
    };                                                                      \
                                                                            \
    static void _ut_testimpl(name)(_ut_unused_ ut_test_t *_test)

#define ut_get_test(name)           (&_ut_testname(name))

#define ut_run_test(t)                                                      \
    (t)->passed = 0;                                                        \
    (t)->err = 0;                                                           \
    (t)->run(t);

#define _ut_test_failed(msg)                                                \
    _test->err += 1;                                                        \
    ut_set_color(RED);                                                      \
    ut_printf("\nfailure: ");                                               \
    ut_set_color(RESET);                                                    \
    ut_printf("%14s", _ut_FILE ":" _ut_LINE ": expected " msg "\n");        \
    return;

#define _ut_test_passed()                                                   \
    _test->passed += 1;

#define _ut_test_failed_if(cond, errmsg)                                    \
    if (cond) {                                                             \
        _ut_test_failed(errmsg);                                            \
    } else {                                                                \
        _ut_test_passed();                                                  \
    }

#define _ut_assert_rel(a, rel, b, err, stra, strb)                          \
    _ut_test_failed_if(!((a) rel (b)), stra " " #rel " " strb)

#define ut_assert(x)                                                        \
    _ut_test_failed_if(!(x), #x " to be true")

#define ut_assert_false(x)                                                  \
    _ut_test_failed_if(x, #x " to be false")

#define ut_assert_eq(a, b)                                                  \
    _ut_assert_rel(a, ==, b, " to be equal to ", #a, #b)

#define ut_assert_streq(a, b)                                               \
    _ut_assert_rel(strcmp(a, b), ==, 0, " to be equal to ", #a, #b)

#define ut_assert_memeq(a, b, sz)                                           \
    _ut_assert_rel(memcmp(a, b, sz), ==, 0, " to be equal to ", #a, #b)

#define ut_assert_ne(a, b)                                                  \
    _ut_assert_rel(a, !=, b, " to be different from ", #a, #b)

#define ut_assert_gt(a, b)                                                  \
    _ut_assert_rel(a, >, b, " to be greater than ", #a, #b)

#define ut_assert_ge(a, b)                                                  \
    _ut_assert_rel(a, >=, b, " to be greater or equal than ", #a, #b)

#define ut_assert_lt(a, b)                                                  \
    _ut_assert_rel(a, <, b, " to be less than ", #a, #b)

#define ut_assert_le(a, b)                                                  \
    _ut_assert_rel(a, <=, b, " to be lesser than or equal to ", #a, #b)

#define _ut_groupname(name)         _group_##name

#define ut_declare_group(name)      extern ut_group_t _ut_groupname(name)

#define ut_group(name, ...)                                                 \
    ut_test_t *const name##_tests_array[] = {__VA_ARGS__};                  \
                                                                            \
    const ut_group_t _ut_groupname(name) = {                                \
        #name,                                                              \
        ut_sizeof_array(name##_tests_array),                                \
        name##_tests_array,                                                 \
    }

#define ut_get_group(name)          (&_ut_groupname(name))

#define ut_run_group(g)                                                     \
    ({                                                                      \
        int _n = ut_printf("Running group: ");                              \
                                                                            \
        ut_set_color(CYAN);                                                 \
        _n += ut_printf("%s", (g)->name);                                   \
        ut_set_color(RESET);                                                \
        for (unsigned int _i = 0; _i < (g)->nbtests; ++_i) {                \
            ut_run_test((g)->tests[_i]);                                    \
            if ((g)->tests[_i]->err != 0) {                                 \
                ut_on_fail("Test failed\n");                                \
            }                                                               \
        }                                                                   \
        ut_printf("%*s", 80 - 4 - _n, "[");                                 \
        ut_set_color(GREEN);                                                \
        ut_printf("OK");                                                    \
        ut_set_color(RESET);                                                \
        ut_printf("]\n");                                                   \
    })

#endif /* !CEEDS_UNIT_TESTS_H */

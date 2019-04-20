/*
** Created by doom on 18/04/19.
*/

#include "unit_tests.h"
#include <ceeds/str.h>

ut_test(str_from_literal)
{
    str_t s = str_from_literal("some string");
    ut_assert_eq(s.length, 11);
    ut_assert_memeq("some string", s.const_str, s.length);
    str_destroy(&s);

    str_t s2 = str_empty();
    ut_assert_eq(s2.length, 0);
    ut_assert_memeq("", s2.const_str, s2.length);
    str_destroy(&s2);
}

ut_test(str_cmp)
{
    ut_assert_eq(str_cmp(str_empty(), str_empty()), 0);
    ut_assert_eq(str_cmp(str_with_length("ABCD", 3), str_with_length("ABCZZ", 3)), 0);
    ut_assert_gt(str_cmp(str_from_literal("ABD"), str_from_literal("ABC")), 0);
    ut_assert_lt(str_cmp(str_from_literal("Hello"), str_from_literal("Holla")), 0);
    ut_assert_lt(str_cmp(str_from_literal(""), str_from_literal("a")), 0);
}

ut_test(str_equal)
{
    ut_assert(str_equal(str_empty(), str_empty()));
    ut_assert(str_equal(str_empty(), str_empty()));
    ut_assert(str_equal(str_with_length("ABCD", 3), str_with_length("ABCZZ", 3)));
    ut_assert_false(str_equal(str_from_literal("Hello"), str_from_literal("Hola")));
    ut_assert_false(str_equal(str_from_literal(""), str_from_literal("a")));
}

ut_test(str_find)
{
    str_t a;

    a = str_from_literal("Because that's what people do when they grief, they go to Egypt");
    ut_assert_eq(str_find(a, str_from_literal("what people")), 15u);
    ut_assert_eq(str_find(a, str_from_literal("Because ")), 0u);
    ut_assert_eq(str_find(a, a), 0u);
    ut_assert_eq(str_find(a, str_from_literal("Egypt")), 58u);
    ut_assert_eq(str_find(a, str_from_literal("Egypta")), str_npos);
    ut_assert_eq(str_find(a, str_from_literal("zwyrx")), str_npos);
}

ut_test(str_findc)
{
    str_t a;

    a = str_from_literal("Because that's what people do when they grief, they go to Egypt");
    ut_assert_eq(str_findc(a, 'B'), 0);
    ut_assert_eq(str_findc(a, 't'), 8u);
    ut_assert_eq(str_findc(a, 'z'), str_npos);
    ut_assert_eq(str_findc(a, '\0'), str_npos);
}

ut_test(str_rfind)
{
    str_t a;

    a = str_from_literal("Because that's what people do when they grief, they go to Egypt");
    ut_assert_eq(str_rfind(a, str_from_literal("what people")), 15u);
    ut_assert_eq(str_rfind(a, str_from_literal("Because ")), 0u);
    ut_assert_eq(str_rfind(a, a), 0u);
    ut_assert_eq(str_rfind(a, str_from_literal("Egypt")), 58u);
    ut_assert_eq(str_rfind(a, str_from_literal("Egypta")), str_npos);
    ut_assert_eq(str_rfind(a, str_from_literal("zwyrx")), str_npos);
    ut_assert_eq(str_rfind(a, str_from_literal("o ")), 56u);
}

ut_test(str_printf_format)
{
    char buf[32];
    str_t a = str_from_literal("test string");

    snprintf(buf, sizeof(buf), "%" PRIstr "\n", str_printf_format(a));
    ut_assert_streq("test string\n", buf);
}

ut_group(str,
         ut_get_test(str_from_literal),
         ut_get_test(str_cmp),
         ut_get_test(str_equal),
         ut_get_test(str_find),
         ut_get_test(str_findc),
         ut_get_test(str_rfind),
         ut_get_test(str_printf_format),
);

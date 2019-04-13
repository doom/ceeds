/*
** Created by doom on 12/04/19.
*/

#include "unit_tests.h"
#include <ceeds/string_utils.h>

ut_test(skipspaces)
{
    ut_assert_streq(skipspaces("    lala lala"), "lala lala");
    ut_assert_streq(skipspaces("lala lala"), "lala lala");
    ut_assert_streq(skipspaces("    "), "");
}

ut_test(skipblanks)
{
    ut_assert_streq(skipblanks("    lala lala"), "lala lala");
    ut_assert_streq(skipblanks("  \n  lala lala"), "\n  lala lala");
    ut_assert_streq(skipblanks("lala lala"), "lala lala");
    ut_assert_streq(skipblanks("    "), "");
}

ut_test(nextword)
{
    ut_assert_streq(nextword("lala lala"), "lala");
    ut_assert_streq(nextword(" lala"), "lala");
    ut_assert_streq(nextword("lala   "), "");
}

ut_test(memrmem)
{
    const char *string = "My door has a wand\nThat belongs on your spell.";
    const char *ptr;

    ptr = memrmem(string, 46, "el", 2);
    ut_assert_eq(ptr - string, 42);

    ptr = memrmem(string, 46, ".", 1);
    ut_assert_eq(ptr - string, 45);

    ptr = memrmem(string, 46, "My door", 7);
    ut_assert_eq(ptr - string, 0);

    ptr = memrmem(string, 46, ".", 1);
    ut_assert_eq(ptr - string, 45);

    ptr = memrmem(string, 2, "door", 4);
    ut_assert_eq(ptr, NULL);

    ptr = memrmem(string, 46, "123", 3);
    ut_assert_eq(ptr, NULL);
}

ut_group(string_utils,
         ut_get_test(skipspaces),
         ut_get_test(skipblanks),
         ut_get_test(nextword),
         ut_get_test(memrmem),
);

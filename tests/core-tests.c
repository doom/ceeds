/*
** Created by doom on 08/04/19.
*/

#include "unit_tests.h"
#include <ceeds/core.h>

ut_test(expectations)
{
    ut_assert(likely(true));
    ut_assert_false(likely(false));
    ut_assert(unlikely(true));
    ut_assert_false(unlikely(false));
}

static int counter = 0;

static int counting_id(int i)
{
    ++counter;
    return i;
}

ut_test(max)
{
    ut_assert_eq(MAX(1, 2), 2);
    ut_assert_eq(MAX(2, 2), 2);

    counter = 0;
    ut_assert_eq(MAX(counting_id(1), 2), 2);
    ut_assert_eq(counter, 1);

    counter = 0;
    ut_assert_eq(MAX(2, counting_id(1)), 2);
    ut_assert_eq(counter, 1);
}

ut_test(min)
{
    ut_assert_eq(MIN(1, 2), 1);
    ut_assert_eq(MIN(2, 2), 2);

    counter = 0;
    ut_assert_eq(MIN(counting_id(1), 0), 0);
    ut_assert_eq(counter, 1);

    counter = 0;
    ut_assert_eq(MIN(0, counting_id(1)), 0);
    ut_assert_eq(counter, 1);
}

ut_test(cmp)
{
    ut_assert_eq(CMP(1, 2), -1);
    ut_assert_eq(CMP(2, 1), 1);
    ut_assert_eq(CMP(1, 1), 0);
    ut_assert_eq(CMP(INT_MIN, 1), -1);
    ut_assert_eq(CMP(1, INT_MIN), 1);
    ut_assert_eq(CMP(INT_MIN, INT_MIN), 0);
}

ut_test(swap)
{
    int a = 0;
    int b = 1;

    SWAP(&a, &b);
    ut_assert_eq(a, 1);
    ut_assert_eq(b, 0);
}

ut_test(array_length)
{
    int arr[6];

    ut_assert_eq(array_length(arr), 6);
}

ut_group(core,
         ut_get_test(expectations),
         ut_get_test(max),
         ut_get_test(min),
         ut_get_test(cmp),
         ut_get_test(swap),
         ut_get_test(array_length)
);

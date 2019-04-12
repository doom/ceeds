/*
** Created by doom on 11/04/19.
*/

#include "unit_tests.h"
#include <ceeds/ascii_set.h>

ut_test(initialization)
{
    ascii_set_t set;

    ascii_set_clear(&set);
    for (size_t i = 0; i < array_length(set.buffer); ++i) {
        ut_assert_eq(set.buffer[i], 0);
    }

    ascii_set_t set2;
    ascii_set_init(&set2, "az19_", 6);
    for (size_t i = 0; i < 256; ++i) {
        if (strchr("az19_", (unsigned char)i)) {
            ut_assert(is_bit_set(set2.buffer, i));
            ut_assert(ascii_set_contains(&set2, (char)i));
        } else {
            ut_assert_false(is_bit_set(set2.buffer, i));
            ut_assert_false(ascii_set_contains(&set2, (char)i));
        }
    }
}

ut_test(union)
{
    ascii_set_t set1;
    ascii_set_t set2;
    ascii_set_t union_set;

    ascii_set_init(&set1, "abcdefghijklmnopqrstuvwxyz", 26);
    ascii_set_init(&set2, "0123456789", 10);
    ascii_set_union(&union_set, &set1, &set2);

    for (size_t i = 0; i < 256; ++i) {
        ut_assert_eq(
            ascii_set_contains(&union_set, (char)i),
            ascii_set_contains(&set1, (char)i) || ascii_set_contains(&set2, (char)i)
        );
    }
}

ut_test(intersection)
{
    ascii_set_t set1;
    ascii_set_t set2;
    ascii_set_t intersection_set;

    ascii_set_init(&set1, "abcdefghijklmnopqrstuvwxyz", 26);
    ascii_set_init(&set2, "acegikmoqsuwy1_43", 17);
    ascii_set_intersection(&intersection_set, &set1, &set2);

    for (size_t i = 0; i < 256; ++i) {
        ut_assert_eq(
            ascii_set_contains(&intersection_set, (char)i),
            ascii_set_contains(&set1, (char)i) && ascii_set_contains(&set2, (char)i)
        );
    }
}

ut_test(difference)
{
    ascii_set_t set1;
    ascii_set_t set2;
    ascii_set_t difference_set;

    ascii_set_init(&set1, "abcdefghijklmnopqrstuvwxyz", 26);
    ascii_set_init(&set2, "acegikmoqsuwy1_43", 17);
    ascii_set_difference(&difference_set, &set1, &set2);

    for (size_t i = 0; i < 256; ++i) {
        ut_assert_eq(
            ascii_set_contains(&difference_set, (char)i),
            ascii_set_contains(&set1, (char)i) && !ascii_set_contains(&set2, (char)i)
        );
    }
}

ut_test(complement)
{
    ascii_set_t set1;
    ascii_set_t complement_set;

    ascii_set_init(&set1, "abcdefghijklmnopqrstuvwxyz", 26);
    ascii_set_complement(&complement_set, &set1);

    for (size_t i = 0; i < 256; ++i) {
        ut_assert_eq(
            ascii_set_contains(&complement_set, (char)i),
            !ascii_set_contains(&set1, (char)i)
        );
    }
}

ut_group(ascii_set,
         ut_get_test(initialization),
         ut_get_test(union),
         ut_get_test(intersection),
         ut_get_test(difference),
         ut_get_test(complement),
);

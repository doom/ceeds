/*
** Created by doom on 23/04/19.
*/

#include "unit_tests.h"
#include <ceeds/growing_str.h>

ut_test(initialization)
{
    growing_str_t gs = gs_empty(heap_allocator_handle());
    ut_assert_eq(gs.capacity, 1);
    ut_assert_eq(gs.length, 0);
    ut_assert_streq(gs.str, "");
    gs_destroy(&gs);

    char buffer[10] = {0};
    growing_str_t gs2 = gs_with_buffer(static_allocator_handle(), buffer, 0, 10);
    ut_assert_eq(gs2.capacity, 10);
    ut_assert_eq(gs2.length, 0);
    ut_assert_streq(gs2.str, "");
    gs_destroy(&gs2);
}

ut_test(reserve)
{
    growing_str_t gs = gs_empty(heap_allocator_handle());

    gs_reserve(&gs, 50);
    ut_assert_ge(gs.capacity, 50);
    ut_assert_eq(gs.length, 0);
    ut_assert_streq(gs.str, "");

    gs_reserve(&gs, 100);
    ut_assert_ge(gs.capacity, 100);
    ut_assert_eq(gs.length, 0);
    ut_assert_streq(gs.str, "");

    gs_destroy(&gs);
}

ut_test(append)
{
    growing_str_t gs = gs_empty(heap_allocator_handle());

    gs_append_c_string(&gs, "abcdefghijklmnopqrstuvwxyz");
    ut_assert_ge(gs.capacity, 26 + 1);
    ut_assert_eq(gs.length, 26);
    ut_assert_streq(gs.str, "abcdefghijklmnopqrstuvwxyz");

    gs_append_buffer(&gs, "0123456789abc", 10);
    ut_assert_ge(gs.capacity, 26 + 10 + 1);
    ut_assert_eq(gs.length, 26 + 10);
    ut_assert_streq(gs.str, "abcdefghijklmnopqrstuvwxyz0123456789");

    gs_append_formatted(&gs, "%d:%d", 123, 456);
    ut_assert_ge(gs.capacity, 26 + 10 + 7 + 1);
    ut_assert_eq(gs.length, 26 + 10 + 7);
    ut_assert_streq(gs.str, "abcdefghijklmnopqrstuvwxyz0123456789123:456");

    gs_destroy(&gs);
}

ut_test(insert)
{
    growing_str_t gs1 = gs_empty(heap_allocator_handle());

    gs_append_c_string(&gs1, "the brown fox jumps over the dog");
    gs_insert_buffer(&gs1, 4, "quick ", 6);
    ut_assert_ge(gs1.capacity, 38 + 1);
    ut_assert_eq(gs1.length, 38);
    ut_assert_streq(gs1.str, "the quick brown fox jumps over the dog");

    gs_insert_formatted(&gs1, 35, "l%xzy%c", 0xa, ' ');
    ut_assert_ge(gs1.capacity, 38 + 5 + 1);
    ut_assert_eq(gs1.length, 38 + 5);
    ut_assert_streq(gs1.str, "the quick brown fox jumps over the lazy dog");

    gs_destroy(&gs1);
}

ut_group(growing_str,
         ut_get_test(initialization),
         ut_get_test(reserve),
         ut_get_test(append),
         ut_get_test(insert),
);

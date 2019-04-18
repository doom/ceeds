/*
** Created by doom on 12/04/19.
*/

#include "unit_tests.h"
#include <ceeds/memory.h>

struct alignment_test_struct
{
    unsigned long x;
    unsigned int y;
} _aligned_(16);

struct allocator_aware_buf_test
{
    memory_allocator_handle_t handle;
    char *buf;
};

static void aabt_destroy(struct allocator_aware_buf_test *aabt)
{
    allocator_delete(aabt->handle, aabt->buf);
}

ut_test(heap_allocator)
{
    int *ptr = new(int);
    ut_assert_ne(ptr, NULL);
    *ptr = 2;
    ut_assert_eq(*ptr, 2);
    delete(ptr);

    struct alignment_test_struct *plol = new(struct alignment_test_struct);
    ut_assert(is_aligned_ptr(plol, alignof(struct alignment_test_struct)));
    plol->x = 1234567890;
    plol->y = 123456;
    delete(plol);

    struct allocator_aware_buf_test aabt = {.buf = new_array(char, 10), .handle = heap_allocator_handle()};
    aabt_destroy(&aabt);

    int *ptr2 = aligned_new(int, 16);
    ut_assert(is_aligned_ptr(ptr2, 16));
    delete(ptr2);
}

ut_test(static_allocator)
{
    struct allocator_aware_buf_test aabt = {.buf = "a string", .handle = static_allocator_handle()};

    aabt_destroy(&aabt);
}

ut_group(memory,
         ut_get_test(heap_allocator),
         ut_get_test(static_allocator),
);

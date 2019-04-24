/*
** Created by doom on 20/04/19.
*/

#include "unit_tests.h"
#include <ceeds/vector.h>

MAKE_VECTOR_TYPE(int, int);

ut_test(initialization)
{
    vector_t(int) vec = vector_empty(heap_allocator_handle());
    vector_t(int) vec2;

    ut_assert_eq(vec.allocator_handle, heap_allocator_handle());
    ut_assert_eq(vec.size, 0);
    ut_assert_eq(vec.capacity, 0);
    vector_destroy(&vec);

    vector_init_with_capacity(&vec2, heap_allocator_handle(), 10);
    ut_assert_eq(vec2.allocator_handle, heap_allocator_handle());
    ut_assert_ne(vec2.data, NULL);
    ut_assert_eq(vec2.size, 0);
    ut_assert_ge(vec2.capacity, 10);
    vector_destroy(&vec2);
}

ut_test(reserve)
{
    vector_t(int) vec;
    vector_init_empty(&vec, heap_allocator_handle());

    vector_reserve(&vec, 10);
    ut_assert_ne(vec.data, NULL);
    ut_assert_eq(vec.size, 0);
    ut_assert_ge(vec.capacity, 10);

    vector_reserve(&vec, 20);
    ut_assert_ne(vec.data, NULL);
    ut_assert_eq(vec.size, 0);
    ut_assert_ge(vec.capacity, 20);
    vector_destroy(&vec);
}

ut_test(push_back)
{
    vector_t(int) vec = vector_empty(heap_allocator_handle());

    for (int i = 0; i < 10; ++i) {
        ut_assert_eq(vec.size, (size_t)i);
        vector_push_back(&vec, i);
    }
    ut_assert_eq(vec.size, 10);

    for (int i = 0; i < 10; ++i) {
        ut_assert_eq(vec.data[i], i);
    }

    vector_destroy(&vec);
}

ut_test(pop_back)
{
    vector_t(int) vec;

    vector_init_empty(&vec, heap_allocator_handle());

    for (int i = 0; i < 10; ++i) {
        vector_push_back(&vec, i);
    }
    for (int i = 9; i >= 0; --i) {
        ut_assert_eq(vec.data[vec.size - 1], i);
        vector_pop_back(&vec);
    }

    vector_destroy(&vec);
}

ut_test(insert)
{
    vector_t(int) vec;

    vector_init_empty(&vec, heap_allocator_handle());

    for (int i = 1; i < 20; i += 2) {
        vector_push_back(&vec, i);
    }

    for (int i = 0; i < 20; i += 2) {
        vector_insert(&vec, (size_t)i, i);
    }

    ut_assert_eq(vec.size, 20);

    for (int i = 0; i < 20; ++i) {
        ut_assert_eq(vec.data[i], i);
    }

    vector_destroy(&vec);
}

ut_test(erase)
{
    vector_t(int) vec;

    vector_init_empty(&vec, heap_allocator_handle());

    for (int i = 0; i < 10; ++i) {
        vector_push_back(&vec, i);
        vector_push_back(&vec, i * 10);
    }

    for (size_t i = 1; i < 11; i += 1) {
        vector_erase(&vec, i);
    }

    ut_assert_eq(vec.size, 10);

    for (int i = 0; i < 10; ++i) {
        ut_assert_eq(vec.data[i], i);
    }

    vector_destroy(&vec);
}

ut_group(vector,
         ut_get_test(initialization),
         ut_get_test(reserve),
         ut_get_test(push_back),
         ut_get_test(pop_back),
         ut_get_test(insert),
         ut_get_test(erase),
);

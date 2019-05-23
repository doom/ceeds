/*
** Created by doom on 22/05/19.
*/

#include "unit_tests.h"
#include <time.h>
#include <ceeds/binary_heap.h>

MAKE_BINARY_HEAP_TYPE(int, int);

static bool is_max_heap(const int *data, size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        if (i * 2 + 1 < n) {
            if (data[i] < data[i * 2 + 1]) {
                return false;
            }
        }
        if (i * 2 + 2 < n) {
            if (data[i] < data[i * 2 + 2]) {
                return false;
            }
        }
    }
    return true;
}

static int max(const int *data, size_t n)
{
    int m = data[0];

    for (size_t i = 1; i < n; ++i) {
        m = MAX(m, data[i]);
    }
    return m;
}

ut_test(random_elements)
{
    binary_heap_t(int) bh = bheap_empty(heap_allocator_handle());

    srand((unsigned int)time(NULL));

    for (int i = 0; i < 100; ++i) {
        ut_assert(is_max_heap(bheap_data(&bh), bheap_size(&bh)));
        bheap_push(&bh, rand(), CMP);
    }

    while (bheap_size(&bh) > 0) {
        int first = bheap_first(&bh);
        int m = max(bheap_data(&bh), bheap_size(&bh));

        bheap_pop(&bh, CMP);
        ut_assert_eq(first, m);
        ut_assert(is_max_heap(bheap_data(&bh), bheap_size(&bh)));
    }

    bheap_destroy(&bh);
}

ut_group(binary_heap,
         ut_get_test(random_elements),
);

/*
** Created by doom on 25/05/19.
*/

#include "unit_tests.h"
#include <ceeds/hash_map.h>

#define hash_int(s)      fnv_one64((const char *)&s, sizeof(s))

MAKE_HASH_MAP_TYPE(test, int, int, hash_int, CMP);

ut_test(initialization)
{
    hash_map_t(test) hm = hash_map_empty(heap_allocator_handle());

    ut_assert_eq(hm.size, 0);

    hash_map_destroy(test, &hm);
}

ut_test(insert1000)
{
    hash_map_t(test) hm = hash_map_empty(heap_allocator_handle());

    for (int i = 0; i < 1000; ++i) {
        hash_map_insert(test, &hm, i * 2 + 3, (-i) * 2 + 3);
    }

    ut_assert_eq(hash_map_size(&hm), 1000);
    ut_assert_ge(hash_map_capacity(&hm), 1000);

    hash_map_destroy(test, &hm);
}

ut_test(insert_find)
{
    hash_map_t(test) hm = hash_map_empty(heap_allocator_handle());
    size_t pos;

    hash_map_insert(test, &hm, 1, 2);
    pos = hash_map_find(test, &hm, 1);
    ut_assert_ne(pos, hash_map_npos);
    ut_assert_eq(hm.values[pos], 2);

    hash_map_insert(test, &hm, 321, 642);
    pos = hash_map_find(test, &hm, 1);
    ut_assert_ne(pos, hash_map_npos);
    ut_assert_eq(hm.values[pos], 2);
    pos = hash_map_find(test, &hm, 321);
    ut_assert_ne(pos, hash_map_npos);
    ut_assert_eq(hm.values[pos], 642);

    pos = hash_map_find(test, &hm, 456);
    ut_assert_eq(pos, hash_map_npos);

    hash_map_destroy(test, &hm);
}

ut_test(erase)
{
    hash_map_t(test) hm = hash_map_empty(heap_allocator_handle());
    size_t pos;

    hash_map_insert(test, &hm, 23, 47);
    pos = hash_map_find(test, &hm, 23);
    ut_assert_ne(pos, hash_map_npos);
    ut_assert_eq(hm.values[pos], 47);

    hash_map_erase(test, &hm, 23);
    pos = hash_map_find(test, &hm, 23);
    ut_assert_eq(pos, hash_map_npos);

    hash_map_destroy(test, &hm);
}

ut_group(hash_map,
         ut_get_test(initialization),
         ut_get_test(insert1000),
         ut_get_test(insert_find),
         ut_get_test(erase)
);

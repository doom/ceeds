/*
** Created by doom on 09/04/19.
*/

#include "unit_tests.h"
#include <ceeds/list.h>

ut_test(initialization)
{
    list_t list;

    list_init(&list);
    ut_assert(list_is_empty(&list));
}

struct list_test_struct
{
    list_node_t node;
    size_t value;
};

ut_test(push_back)
{
    list_t list;
    struct list_test_struct test[4];

    list_init(&list);
    for (size_t i = 0; i < array_length(test); ++i) {
        test[i].value = i;
        list_push_back(&list, &test[i].node);
    }
    ut_assert_false(list_is_empty(&list));

    size_t idx = 0;
    list_for_each(&list, cur_node) {
        struct list_test_struct *cur = list_element(cur_node, struct list_test_struct, node);
        ut_assert_eq(cur->value, idx);
        ++idx;
    }
    ut_assert_eq(idx, array_length(test));

    list_for_each_r(&list, cur_node) {
        --idx;
        struct list_test_struct *cur = list_element(cur_node, struct list_test_struct, node);
        ut_assert_eq(cur->value, idx);
    }
    ut_assert_eq(idx, 0);
}

ut_test(push_front)
{
    list_t list;
    struct list_test_struct test[4];

    list_init(&list);
    for (size_t i = 0; i < array_length(test); ++i) {
        test[i].value = i;
        list_push_front(&list, &test[i].node);
    }
    ut_assert_false(list_is_empty(&list));

    size_t idx = array_length(test);
    list_for_each(&list, cur_node) {
        --idx;
        struct list_test_struct *cur = list_element(cur_node, struct list_test_struct, node);
        ut_assert_eq(cur->value, idx);
    }
    ut_assert_eq(idx, 0);

    list_for_each_r(&list, cur_node) {
        struct list_test_struct *cur = list_element(cur_node, struct list_test_struct, node);
        ut_assert_eq(cur->value, idx);
        ++idx;
    }
    ut_assert_eq(idx, array_length(test));
}

ut_test(pop_back)
{
    list_t list;
    struct list_test_struct test[4];

    list_init(&list);
    for (size_t i = 0; i < array_length(test); ++i) {
        test[i].value = i;
        list_push_back(&list, &test[i].node);
    }
    ut_assert_false(list_is_empty(&list));

    size_t idx = array_length(test);
    while (!list_is_empty(&list)) {
        --idx;
        struct list_test_struct *cur = list_element(list.tail, struct list_test_struct, node);
        ut_assert_eq(cur->value, idx);
        list_pop_back(&list);
    }
    ut_assert_eq(idx, 0);
}

ut_test(pop_front)
{
    list_t list;
    struct list_test_struct test[4];

    list_init(&list);
    for (size_t i = 0; i < array_length(test); ++i) {
        test[i].value = i;
        list_push_front(&list, &test[i].node);
    }
    ut_assert_false(list_is_empty(&list));

    size_t idx = array_length(test);
    while (!list_is_empty(&list)) {
        --idx;
        struct list_test_struct *cur = list_element(list.head, struct list_test_struct, node);
        ut_assert_eq(cur->value, idx);
        list_pop_front(&list);
    }
    ut_assert_eq(idx, 0);
}

ut_test(for_each_element)
{
    list_t list;
    struct list_test_struct test[5];

    list_init(&list);
    list_for_each_element(&list, cur_elem, struct list_test_struct, node) {
        ut_assert(false);
    }

    for (size_t i = 0; i < array_length(test); ++i) {
        test[i].value = i;
        list_push_back(&list, &test[i].node);
    }
    ut_assert_false(list_is_empty(&list));

    size_t idx = 0;
    list_for_each_element(&list, cur_elem, struct list_test_struct, node) {
        ut_assert_eq(cur_elem->value, idx);
        ++idx;
    }

    list_for_each_element_r(&list, cur_elem, struct list_test_struct, node) {
        --idx;
        ut_assert_eq(cur_elem->value, idx);
    }
    ut_assert_eq(idx, 0);
}

ut_group(list,
         ut_get_test(initialization),
         ut_get_test(push_back),
         ut_get_test(push_front),
         ut_get_test(pop_back),
         ut_get_test(pop_front),
         ut_get_test(for_each_element),
);

/*
** Created by doom on 09/04/19.
*/

#ifndef CEEDS_LIST_H
#define CEEDS_LIST_H

#include <ceeds/core.h>

/**
 * Doubly-linked lists with head and tail pointers
 */

typedef struct list_node
{
    struct list_node *next;
    struct list_node *prev;
} list_node_t;

typedef struct list
{
    union
    {
        struct list_node guard_node;
        struct
        {
            struct list_node *head;
            struct list_node *tail;
        };
    };
} list_t;

/**
 * Initialize a list
 *
 * @param[out]      list        the list to initialize
 */
static inline void list_init(list_t *list)
{
    list->guard_node.next = &list->guard_node;
    list->guard_node.prev = &list->guard_node;
}

/**
 * Check whether a list is empty
 *
 * @param[in]       list        the list to check
 * @return                      true if the list is empty, false otherwise
 */
static _always_inline_ bool list_is_empty(const list_t *list)
{
    return list->guard_node.next == &list->guard_node;
}

/**
 * Insert a node between two given nodes
 *
 * @param[out]      as_prev     the node to use as "prev" node
 * @param[out]      as_next     the node to use as "next" node
 * @param[out]      node        the node to insert
 *
 * @pre                         @p as_prev and @p as_next should respectively be each other's prev/next
 */
static inline void list_node_insert_between(list_node_t *as_prev, list_node_t *as_next, list_node_t *node)
{
    as_prev->next = node;
    node->prev = as_prev;
    node->next = as_next;
    as_next->prev = node;
}

/**
 * Add an element at the end of a list
 *
 * @param[in,out]   list        the list to insert into
 * @param[out]      node        the node to insert
 */
static inline void list_push_back(list_t *list, list_node_t *node)
{
    list_node_insert_between(list->tail, &list->guard_node, node);
}

/**
 * Add an element at the beginning of a list
 *
 * @param[in,out]   list        the list to insert into
 * @param[out]      node        the node to insert
 */
static inline void list_push_front(list_t *list, list_node_t *node)
{
    list_node_insert_between(&list->guard_node, list->head, node);
}

static inline void list_node_remove(list_node_t *node)
{
    node->next->prev = node->prev;
    node->prev->next = node->next;
}

/**
 * Remove the last element of a list
 *
 * @param[in,out]   list        the list to pop from
 *
 * @pre                         @p list must not be empty
 */
static inline void list_pop_back(list_t *list)
{
    assert(!list_is_empty(list));

    list_node_remove(list->tail);
}

/**
 * Remove the first element of a list
 *
 * @param[in,out]   list        the list to pop from
 *
 * @pre                         @p list must not be empty
 */
static inline void list_pop_front(list_t *list)
{
    assert(!list_is_empty(list));

    list_node_remove(list->head);
}

/**
 * Fix up a node's potentially broken links
 * This is useful when the address of a node has changed (for example, on reallocation),
 * in order to fix the other nodes' links to it.
 *
 * @param[in,out]   node        the node to fix up
 */
static inline void list_node_fix_up(list_node_t *node)
{
    node->next->prev = node;
    node->prev->next = node;
}

/**
 * Iterate through a list node by node
 *
 * @param[in]       list        the list to iterate through
 * @param           node_name   the name under which the current node should be used
 */
#define list_for_each(list, node_name)                                      \
    for (list_node_t *node_name = (list)->head;                             \
         node_name != (list)->head->prev;                                   \
         node_name = node_name->next)

/**
 * Iterate through a list node by node, in the reverse order
 *
 * @param[in]       list        the list to iterate through
 * @param           node_name   the name under which the current node should be used
 */
#define list_for_each_r(list, node_name)                                    \
    for (list_node_t *node_name = (list)->tail;                             \
         node_name != (list)->tail->next;                                   \
         node_name = node_name->prev)

/**
 * Get the element associated with a node
 *
 * @param[in]       nodep       the node to get the element from
 * @param           T           the type of the element to get
 * @param           f           the name of the node field in the @p T type
 */
#define list_element(nodep, T, f)            container_of(nodep, T, f)

/**
 * Iterate through a list element by element
 *
 * @param[in]       list        the list to iterate through
 * @param           element     the name under which the current element should be used
 */
#define list_for_each_element(list, element, T, f)                          \
    for (T *element = list_element((list)->head, T, f);                     \
         &element->node != (list)->head->prev;                              \
         element = list_element(element->node.next, T, f))

/**
 * Iterate through a list element by element, in the reverse order
 *
 * @param[in]       list        the list to iterate through
 * @param           element     the name under which the current element should be used
 */
#define list_for_each_element_r(list, element, T, f)                        \
    for (T *element = list_element((list)->tail, T, f);                     \
         &element->node != (list)->tail->next;                              \
         element = list_element(element->node.prev, T, f))

#endif /* !CEEDS_LIST_H */

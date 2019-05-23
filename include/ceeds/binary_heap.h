/*
** Created by doom on 27/04/19.
*/

#ifndef CEEDS_BINARY_HEAP_H
#define CEEDS_BINARY_HEAP_H

#include <ceeds/core.h>
#include <ceeds/vector.h>

/**
 * Max-Heaps
 */

#define binary_heap_t(n)            binary_heap_##n##_t

/**
 * Create a binary heap type
 *
 * @param           n               the name of the binary heap type to create
 * @param           T               the type of the elements to store
 */
#define MAKE_BINARY_HEAP_TYPE(n, T)                                         \
    MAKE_VECTOR_TYPE(heap_underlying_##n, T);                               \
                                                                            \
    typedef vector_t(heap_underlying_##n) binary_heap_t(n)

/**
 * Create an empty binary heap
 *
 * @param[in]       alloc_handle    the allocator handle to be used by the binary heap
 */
#define bheap_empty(alloc_handle)   vector_empty(alloc_handle)

/**
 * Destroy a binary heap
 *
 * @param[in,out]   heap_ptr        a pointer to the binary heap to destroy
 */
#define bheap_destroy(heap_ptr)     vector_destroy(heap_ptr)

/**
 * Get the size of a binary heap (i.e. the number of elements in the binary heap)
 *
 * @param[in]       heap_ptr        a pointer to the binary heap
 */
#define bheap_size(heap_ptr)        vector_size(heap_ptr)

/**
 * Get the capacity of a binary heap (i.e. the number of elements it currently has memory allocated for)
 *
 * @param[in]       heap_ptr        a pointer to the binary heap
 */
#define bheap_capacity(heap_ptr)    vector_capacity(heap_ptr)

/**
 * Get the data of a binary heap (i.e. a pointer to the underlying array)
 *
 * @param[in]       heap_ptr        a pointer to the binary heap
 */
#define bheap_data(heap_ptr)        vector_data(heap_ptr)

/**
 * Increase the capacity of a binary heap to be at least equal to a given amount
 *
 * @param[in,out]   heap_ptr        the binary heap whose capacity is to be increased
 * @param[in]       new_capacity    the new capacity
 */
#define bheap_reserve(heap_ptr, new_capacity)   vector_reserve(heap_ptr, new_capacity)

#define _bheap_push_up(heap_ptr, idx, cmp)                                  \
    do {                                                                    \
        typeof(heap_ptr) __htr_ptr = (heap_ptr);                            \
        size_t __i = (idx);                                                 \
                                                                            \
        while (                                                             \
            __i > 0 &&                                                      \
            cmp(__htr_ptr->data[__i], __htr_ptr->data[(__i - 1) / 2]) > 0   \
        ) {                                                                 \
            SWAP(&__htr_ptr->data[(__i - 1) / 2], &__htr_ptr->data[__i]);   \
            __i = (__i - 1) / 2;                                            \
        }                                                                   \
    } while (0)

#define _bheap_push_down(heap_ptr, idx, cmp)                                \
    do {                                                                    \
        typeof(heap_ptr) __htr_ptr = (heap_ptr);                            \
        size_t __i = (idx);                                                 \
                                                                            \
        while (1) {                                                         \
            size_t __left = __i * 2 + 1;                                    \
            size_t __right = __i * 2 + 2;                                   \
            size_t __max = __i;                                             \
                                                                            \
            if (                                                            \
                __left < __htr_ptr->size &&                                 \
                cmp(__htr_ptr->data[__max], __htr_ptr->data[__left]) < 0    \
            ) {                                                             \
                __max = __left;                                             \
            }                                                               \
            if (                                                            \
                __right < __htr_ptr->size &&                                \
                cmp(__htr_ptr->data[__max], __htr_ptr->data[__right]) < 0   \
            ) {                                                             \
                __max = __right;                                            \
            }                                                               \
            if (__max == __i) {                                             \
                break;                                                      \
            }                                                               \
            SWAP(&__htr_ptr->data[__i], &__htr_ptr->data[__max]);           \
            __i = __max;                                                    \
        }                                                                   \
    } while (0)

/**
 * Insert an element into a binary heap
 *
 * @param[in,out]   heap_ptr        a pointer to the binary heap to insert into
 * @param[in]       e               the element to insert
 * @param[in]       cmp             the comparator used to order elements in the heap
 *
 * @pre                             @p cmp takes two parameters A and B, and returns a value R, with
 *                                  R < 0 if A < B
 *                                  R == 0 if A == B
 *                                  R > 0 if A > B
 */
#define bheap_push(heap_ptr, e, cmp)                                        \
    do {                                                                    \
        typeof(heap_ptr) __heap_ptr = (heap_ptr);                           \
                                                                            \
        vector_push_back(__heap_ptr, e);                                    \
        _bheap_push_up(__heap_ptr, __heap_ptr->size - 1, cmp);              \
    } while (0)

/**
 * Remove the first element of a binary heap
 *
 * @param[in,out]   heap_ptr        a pointer to the binary heap to insert into
 * @param[in]       cmp             the comparator used to order elements in the heap
 *
 * @pre                             @p cmp takes two parameters A and B, and returns a value R, with
 *                                  R < 0 if A < B
 *                                  R == 0 if A == B
 *                                  R > 0 if A > B
 * @pre                             @p heap_ptr must have at least one element
 */
#define bheap_pop(heap_ptr, cmp)                                            \
    do {                                                                    \
        typeof(heap_ptr) __heap_ptr = (heap_ptr);                           \
                                                                            \
        SWAP(&__heap_ptr->data[0], &__heap_ptr->data[__heap_ptr->size - 1]);\
        vector_pop_back(__heap_ptr);                                        \
        _bheap_push_down(__heap_ptr, 0, cmp);                               \
    } while (0)

/**
 * Get the value of the first element of a binary heap
 *
 * @param[in,out]   heap_ptr        a pointer to the binary heap
 *
 * @pre                             @p heap_ptr must have at least one element
 */
#define bheap_first(heap_ptr)       ((heap_ptr)->data[0])

/**
 * Update an element in the heap
 *
 * @param[in,out]   heap_ptr        a pointer to the binary heap to update
 * @param[in]       idx             the index of the element to update
 * @param[in]       e               the new value of the element
 * @param[in]       cmp             the comparator used to order elements in the heap
 *
 * @pre                             @p cmp takes two parameters A and B, and returns a value R, with
 *                                  R < 0 if A < B
 *                                  R == 0 if A == B
 *                                  R > 0 if A > B
 * @pre                             @p heap_ptr must have at least @p idx elements
 */
#define bheap_update(heap_ptr, idx, e, cmp)                                 \
    do {                                                                    \
        typeof(heap_ptr) __heap_ptr = (heap_ptr);                           \
        size_t __idx = (idx);                                               \
        typeof(e) __e = (e);                                                \
                                                                            \
        if (cmp(__e, __heap_ptr->data[__idx]) > 0) {                        \
            __heap_ptr->data[__idx] = __e;                                  \
            _bheap_push_up(__heap_ptr, __idx, cmp);                         \
        } else {                                                            \
            __heap_ptr->data[__idx] = __e;                                  \
            _bheap_push_down(__heap_ptr, __idx, cmp);                       \
        }                                                                   \
    } while (0)

#endif /* !CEEDS_BINARY_HEAP_H */

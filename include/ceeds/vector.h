/*
** Created by doom on 20/04/19.
*/

#ifndef CEEDS_VECTOR_H
#define CEEDS_VECTOR_H

#include <ceeds/core.h>
#include <ceeds/memory.h>

/**
 * Vectors (dynamically-growing arrays)
 */

#define vector_t(n)                 vector_##n##_t

/**
 * Create a vector type
 *
 * @param           n               the name of the vector type to create
 * @param           T               the type of the elements to store
 */
#define MAKE_VECTOR_TYPE(n, T)                                              \
    typedef struct vector_t(n) {                                            \
        memory_allocator_handle_t allocator_handle;                         \
        T *data;                                                            \
        size_t size;                                                        \
        size_t capacity;                                                    \
    } vector_t(n)

/**
 * Create a vector with an existing buffer
 *
 * @param[in]       alloc_handle    the allocator handle to be used by the vector
 * @param[in]       buffer          the buffer to use as the underlying array
 * @param[in]       size            the initial size
 * @param[in]       capacity        the initial capacity of the buffer
 *
 * @pre                             @p buffer must be capable of storing at least @p capacity elements
 * @pre                             @p size must be less than or equal to @p capacity
 */
#define vector_with_buffer(alloc_handle, buffer, size, capacity)            \
    {alloc_handle, buffer, size, capacity}

/**
 * Create an empty vector
 *
 * @param[in]       alloc_handle    the allocator handle to be used by the vector
 */
#define vector_empty(alloc_handle)  vector_with_buffer(alloc_handle, NULL, 0, 0)

/**
 * Initialize a vector with a pre-allocated buffer, a size and a capacity
 *
 * @param[in,out]   vec_ptr         a pointer to the vector to initialize
 * @param[in]       alloc_handle    the allocator handle to be used by the vector
 * @param[in]       buffer          the buffer to use as the underlying array
 * @param[in]       sz              the initial size
 * @param[in]       cap             the initial capacity of the buffer
 *
 * @pre                             @p buffer must be capable of storing at least @p cap elements
 * @pre                             @p sz must be less than or equal to @p cap
 */
#define vector_init_with_buffer(vec_ptr, alloc_handle, buffer, sz, cap)     \
    do {                                                                    \
        typeof(vec_ptr) __vec_ptr = (vec_ptr);                              \
                                                                            \
        __vec_ptr->allocator_handle = (alloc_handle);                       \
        __vec_ptr->data = (buffer);                                         \
        __vec_ptr->size = (sz);                                             \
        __vec_ptr->capacity = (cap);                                        \
    } while (0)

/**
 * Initialize a vector with a given capacity
 *
 * @param[in,out]   vec_ptr         a pointer to the vector to initialize
 * @param[in]       alloc_handle    the allocator handle to be used by the vector
 * @param[in]       capacity        the initial capacity to allocate for
 */
#define vector_init_with_capacity(vec_ptr, alloc_handle, capacity)          \
    do {                                                                    \
        typeof(alloc_handle) __alloc_handle = (alloc_handle);               \
        size_t __cap = (size_t)(capacity);                                  \
                                                                            \
        vector_init_with_buffer(                                            \
            vec_ptr,                                                        \
            __alloc_handle,                                                 \
            allocator_new_array(                                            \
                __alloc_handle,                                             \
                typeof(*(vec_ptr)->data),                                   \
                __cap                                                       \
            ),                                                              \
            0,                                                              \
            __cap                                                           \
        );                                                                  \
    } while (0)

/**
 * Initialize a vector as empty
 *
 * @param[in,out]   vec_ptr         a pointer to the vector to initialize
 * @param[in]       alloc_handle    the allocator handle to be used by the vector
 */
#define vector_init_empty(vec_ptr, alloc_handle)                            \
    vector_init_with_buffer(vec_ptr, alloc_handle, NULL, 0, 0)

/**
 * Destroy a vector
 *
 * @param[in,out]   vec_ptr         a pointer to the vector to destroy
 */
#define vector_destroy(vec_ptr)                                             \
    allocator_delete((vec_ptr)->allocator_handle, (vec_ptr)->data)

/**
 * Get the size of a vector (i.e. the number of elements in the vector)
 *
 * @param[in]       vec_ptr         a pointer to the vector
 */
#define vector_size(vec_ptr)        ((vec_ptr)->size)

/**
 * Get the capacity of a vector (i.e. the number of elements it currently has memory allocated for)
 *
 * @param[in]       vec_ptr         a pointer to the vector
 */
#define vector_capacity(vec_ptr)    ((vec_ptr)->capacity)

/**
 * Get the data of a vector (i.e. a pointer to the underlying array)
 *
 * @param[in]       vec_ptr         a pointer to the vector
 */
#define vector_data(vec_ptr)        ((vec_ptr)->data)

/**
 * Increase the capacity of a vector to be at least equal to a given amount
 *
 * @param[in,out]   vec_ptr         the vector whose capacity is to be increased
 * @param[in]       new_capacity    the new capacity
 */
#define vector_reserve(vec_ptr, new_capacity)                               \
    do {                                                                    \
        typeof(vec_ptr) __vtg_ptr = (vec_ptr);                              \
        size_t __new_capacity = (new_capacity);                             \
                                                                            \
        if (__vtg_ptr->capacity < __new_capacity) {                         \
            __new_capacity = MAX(__new_capacity, __vtg_ptr->capacity * 2);  \
            __vtg_ptr->data = allocator_resize_array(                       \
                __vtg_ptr->allocator_handle,                                \
                __vtg_ptr->data,                                            \
                typeof(*__vtg_ptr->data),                                   \
                __vtg_ptr->capacity,                                        \
                __new_capacity                                              \
            );                                                              \
            __vtg_ptr->capacity = __new_capacity;                           \
        }                                                                   \
    } while (0)

/**
 * Add an element at the end of a vector
 *
 * @param[in,out]   vec_ptr         a pointer to the vector to append into
 * @param[in]       e               the element to append
 */
#define vector_push_back(vec_ptr, e)                                        \
    do {                                                                    \
        typeof(vec_ptr) __vec_ptr = (vec_ptr);                              \
                                                                            \
        vector_reserve(__vec_ptr, __vec_ptr->size + 1);                     \
        __vec_ptr->data[__vec_ptr->size++] = e;                             \
    } while (0)

/**
 * Remove the last element of a vector
 *
 * @param[in,out]   vec_ptr         a pointer to the vector to remove from
 *
 * @pre                             @p vec_ptr must have at least one element
 */
#define vector_pop_back(vec_ptr)                                            \
    do {                                                                    \
        typeof(vec_ptr) __vec_ptr = (vec_ptr);                              \
                                                                            \
        --__vec_ptr->size;                                                  \
    } while (0)

/**
 * Insert an element into a vector at a given position
 *
 * @param[in,out]   vec_ptr         a pointer to the vector to insert into
 * @param[in]       pos             the position at which to insert
 * @param[in]       e               the element to insert
 *
 * @pre                             @p vec_ptr must have at least @p pos elements
 */
#define vector_insert(vec_ptr, pos, e)                                      \
    do {                                                                    \
        typeof(vec_ptr) __vec_ptr = (vec_ptr);                              \
        size_t __pos = (pos);                                               \
                                                                            \
        vector_reserve(__vec_ptr, __vec_ptr->size + 1);                     \
        memmove(                                                            \
            __vec_ptr->data + __pos + 1,                                    \
            __vec_ptr->data + __pos,                                        \
            sizeof(*__vec_ptr->data) * (__vec_ptr->size - __pos)            \
        );                                                                  \
        ++__vec_ptr->size;                                                  \
        __vec_ptr->data[__pos] = e;                                         \
    } while (0)

/**
 * Remove an element at a given position from a vector
 *
 * @param[in,out]   vec_ptr         a pointer to the vector to remove from
 * @param[in]       pos             the position of the element to remove
 *
 * @pre                             @p vec_ptr must have at least @p pos + 1 elements
 */
#define vector_erase(vec_ptr, pos)                                          \
    do {                                                                    \
        typeof(vec_ptr) __vec_ptr = (vec_ptr);                              \
        size_t __pos = (pos);                                               \
                                                                            \
        memmove(                                                            \
            __vec_ptr->data + __pos,                                        \
            __vec_ptr->data + __pos + 1,                                    \
            sizeof(*__vec_ptr->data) * (__vec_ptr->size - __pos - 1)        \
        );                                                                  \
        --__vec_ptr->size;                                                  \
    } while (0)

#endif /* !CEEDS_VECTOR_H */

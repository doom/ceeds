/*
** Created by doom on 17/04/19.
*/

#ifndef CEEDS_MEMORY_ALLOCATOR_H
#define CEEDS_MEMORY_ALLOCATOR_H

#include <ceeds/core.h>

/**
 * Memory allocator abstraction, used to build allocator-aware containers
 */

/**
 * A handle to an allocator
 *
 * Allocator-aware objects should use such a handle in order to manage their memory without caring about
 * the underlying specific allocator.
 */
typedef struct memory_allocator *memory_allocator_handle_t;

/**
 * A memory allocator
 *
 * Such an object is used only by allocator implementers, and (under the hood) by allocation macros.
 */
struct memory_allocator
{
    /**
     * Allocate memory
     *
     * @param[in]           alloc       a pointer to the allocator handle used for this allocation
     * @param[in]           size        the amount of bytes to allocate
     * @param[in]           align       the minimum alignment required for the allocated memory
     * @return                          a pointer to the beginning of the newly allocated memory
     *
     * @pre                             @p align must be a power of 2
     */
    void *(*allocate)(memory_allocator_handle_t alloc, size_t size, size_t align);

    /**
     * Allocate memory, zeroing it
     *
     * @param[in]           alloc       a pointer to the allocator handle used for this allocation
     * @param[in]           size        the amount of bytes to allocate
     * @param[in]           align       the minimum alignment required for the allocated memory
     * @return                          a pointer to the beginning of the newly allocated memory
     *
     * @pre                             @p align must be a power of 2
     */
    void *(*zero_allocate)(memory_allocator_handle_t alloc, size_t size, size_t align);

    /**
     * Deallocate previously allocated memory
     *
     * @param[in]           alloc       a pointer to the allocator handle used for this allocation
     * @param[in]           ptr         a pointer to the allocated memory
     */
    void (*deallocate)(memory_allocator_handle_t alloc, void *ptr);

    /**
     * Resize a previously allocated memory block, keeping the data
     *
     * If @p ptr is NULL, this is equivalent to a regular allocation.
     * If @p old_size is 0, no data will be kept
     * If @p new_size is 0, this is equivalent to a regular deallocation
     *
     * @param[in]           alloc       a pointer to the allocator handle used for this allocation
     * @param[in]           ptr         a pointer to the previously allocated memory
     * @param[in]           old_size    the amount of bytes allocated for @p ptr
     * @param[in]           new_size    the new amount of bytes to allocated
     * @param[in]           new_align   the new minimum alignment required for the allocated memory
     * @return                          a pointer to the beginning of the newly allocated memory
     *
     * @pre                             @p new_align must be a power of 2
     */
    void *(*reallocate)(memory_allocator_handle_t alloc, void *ptr, size_t old_size, size_t new_size, size_t new_align);
};

/**
 * Allocate memory from a given allocator to hold an object of a given type, with a given alignment
 *
 * @param[in]               handle      a handle to the allocator to use
 * @param                   T           the type of object to allocate memory for
 * @param[in]               align       the minimum alignment required for the allocated memory
 * @return                              a pointer to the beginning of the newly allocated memory
 *
 * @pre                                 @p align must be a power of 2
 */
#define allocator_aligned_new(handle, T, align)             ((handle)->allocate((handle), sizeof(T), (align)))

/**
 * Allocate memory from a given allocator to hold an object of a given type, with a given alignment, zeroing it
 *
 * @param[in]               handle      a handle to the allocator to use
 * @param                   T           the type of object to allocate memory for
 * @param[in]               align       the minimum alignment required for the allocated memory
 * @return                              a pointer to the beginning of the newly allocated memory
 *
 * @pre                                 @p align must be a power of 2
 */
#define allocator_aligned_znew(handle, T, align)            ((handle)->zero_allocate((handle), sizeof(T), (align)))

/**
 * Allocate memory from a given allocator to hold an array of objects of a given type, with a given alignment
 *
 * @param[in]               handle      a handle to the allocator to use
 * @param                   T           the type of object to allocate memory for
 * @param[in]               n           the number of elements to allocate memory for
 * @param[in]               align       the minimum alignment required for the allocated memory
 * @return                              a pointer to the beginning of the newly allocated memory
 *
 * @pre                                 @p align must be a power of 2
 */
#define allocator_aligned_new_array(handle, T, n, align)    ((handle)->allocate((handle), sizeof(T) * n, (align)))

/**
 * Allocate memory from a given allocator to hold an array of objects of a given type, with a given alignment, zeroing it
 *
 * @param[in]               handle      a handle to the allocator to use
 * @param                   T           the type of object to allocate memory for
 * @param[in]               n           the number of elements to allocate memory for
 * @param[in]               align       the minimum alignment required for the allocated memory
 * @return                              a pointer to the beginning of the newly allocated memory
 *
 * @pre                                 @p align must be a power of 2
 */
#define allocator_aligned_znew_array(handle, T, n, align)   ((handle)->zero_allocate((handle), sizeof(T) * n, (align)))

/**
 * Allocate memory from a given allocator to hold an object of a given type
 *
 * @param[in]               handle      a handle to the allocator to use
 * @param                   T           the type of object to allocate memory for
 * @return                              a pointer to the beginning of the newly allocated memory
 */
#define allocator_new(handle, T)                            allocator_aligned_new(handle, T, alignof(T))

/**
 * Allocate memory from a given allocator to hold an object of a given type, zeroing it
 *
 * @param[in]               handle      a handle to the allocator to use
 * @param                   T           the type of object to allocate memory for
 * @return                              a pointer to the beginning of the newly allocated memory
 */
#define allocator_znew(handle, T)                           allocator_aligned_znew(handle, T, alignof(T))

/**
 * Allocate memory from a given allocator to hold an array of objects of a given type
 *
 * @param[in]               handle      a handle to the allocator to use
 * @param                   T           the type of object to allocate memory for
 * @param[in]               n           the number of elements to allocate memory for
 * @return                              a pointer to the beginning of the newly allocated memory
 */
#define allocator_new_array(handle, T, n)                   allocator_aligned_new_array(handle, T, n, alignof(T))

/**
 * Allocate memory from a given allocator to hold an array of objects of a given type, zeroing it
 *
 * @param[in]               handle      a handle to the allocator to use
 * @param                   T           the type of object to allocate memory for
 * @param[in]               n           the number of elements to allocate memory for
 * @return                              a pointer to the beginning of the newly allocated memory
 */
#define allocator_znew_array(handle, T, n)                  allocator_aligned_znew_array(handle, T, n, alignof(T))

/**
 * Deallocate memory previously allocated from a given allocator
 *
 * @param[in]               handle      a handle to the allocator to use
 * @param[in]               ptr         a pointer to the memory to deallocate
 */
#define allocator_delete(handle, ptr)                       ((handle)->deallocate((handle), ptr))

#endif /* !CEEDS_MEMORY_ALLOCATOR_H */

/*
** Created by doom on 12/04/19.
*/

#ifndef CEEDS_MEMORY_H
#define CEEDS_MEMORY_H

#include <ceeds/memory_allocator.h>

/**
 * Basic allocators using the model defined in the memory_allocator.h header
 */

/**
 * The heap allocator is merely a wrapper around malloc/calloc/free/realloc.
 */
extern struct memory_allocator heap_allocator;

/**
 * Get a handle to the heap allocator
 */
#define heap_allocator_handle()         (&heap_allocator)

/**
 * Allocate memory from the heap to hold an object of a given type
 *
 * @param                   T           the type of object to allocate memory for
 * @return                              a pointer to the beginning of the newly allocated memory
 */
#define new(T)                          allocator_new(heap_allocator_handle(), T)

/**
 * Allocate memory from the heap to hold an object of a given type, zeroing it
 *
 * @param                   T           the type of object to allocate memory for
 * @return                              a pointer to the beginning of the newly allocated memory
 */
#define znew(T)                         allocator_znew(heap_allocator_handle(), T)

/**
 * Allocate memory from the heap to hold an array of objects of a given type
 *
 * @param                   T           the type of object to allocate memory for
 * @param[in]               n           the number of elements to allocate memory for
 * @return                              a pointer to the beginning of the newly allocated memory
 */
#define new_array(T, n)                 allocator_new_array(heap_allocator_handle(), T, n)

/**
 * Allocate memory from the heap to hold an array of objects of a given type, zeroing it
 *
 * @param                   T           the type of object to allocate memory for
 * @param[in]               n           the number of elements to allocate memory for
 * @return                              a pointer to the beginning of the newly allocated memory
 */
#define znew_array(T, n)                allocator_znew_array(heap_allocator_handle(), T, n)

/**
 * Allocate memory from the heap to hold an object of a given type, with a given alignment
 *
 * @param                   T           the type of object to allocate memory for
 * @param[in]               align       the minimum alignment required for the allocated memory
 * @return                              a pointer to the beginning of the newly allocated memory
 */
#define aligned_new(T, align)           allocator_aligned_new(heap_allocator_handle(), T, align)

/**
 * Allocate memory from the heap to hold an object of a given type, with a given alignment, zeroing it
 *
 * @param                   T           the type of object to allocate memory for
 * @param[in]               align       the minimum alignment required for the allocated memory
 * @return                              a pointer to the beginning of the newly allocated memory
 */
#define aligned_znew(T, align)          allocator_aligned_znew(heap_allocator_handle(), T, align)

/**
 * Allocate memory from the heap to hold an array of objects of a given type, with a given alignment
 *
 * @param                   T           the type of object to allocate memory for
 * @param[in]               n           the number of elements to allocate memory for
 * @param[in]               align       the minimum alignment required for the allocated memory
 * @return                              a pointer to the beginning of the newly allocated memory
 */
#define aligned_new_array(T, n, align)  allocator_aligned_new_array(heap_allocator_handle(), T, n, align)

/**
 * Allocate memory from the heap to hold an array of objects of a given type, with a given alignment, zeroing it
 *
 * @param                   T           the type of object to allocate memory for
 * @param[in]               n           the number of elements to allocate memory for
 * @param[in]               align       the minimum alignment required for the allocated memory
 * @return                              a pointer to the beginning of the newly allocated memory
 */
#define aligned_znew_array(T, n, align) allocator_aligned_znew_array(heap_allocator_handle(), T, n, align)

/**
 * Deallocate memory previously allocated from the heap
 *
 * @param[in]               ptr         a pointer to the memory to deallocate
 */
#define delete(ptr)                     allocator_delete(heap_allocator_handle(), ptr)

/**
 * The static allocator is a placeholder that should be used for objects whose memory does
 * not have to be managed. That includes static storage, but more generally any storage that
 * will outlive the associated allocator handle.
 *
 * Therefore this allocator cannot (and should not) be used in a attempt to allocate or
 * reallocate memory, and its de-allocating function is a no-op.
 */
extern struct memory_allocator static_allocator;

/**
 * Get a handle to the static allocator
 */
#define static_allocator_handle()       (&static_allocator)

#endif /* !CEEDS_MEMORY_H */

/*
** Created by doom on 12/04/19.
*/

#include <ceeds/memory.h>

static __attribute_noinline__ _noreturn_ void handle_allocation_failure(void)
{
    exit(1);
}

#define MALLOC_ALIGN    (alignof(max_align_t))

static void *heap_allocate(_unused_ memory_allocator_handle_t alloc, size_t size, size_t align)
{
    void *ptr;

    if (align <= MALLOC_ALIGN) {
        ptr = malloc(size);
    } else {
        ptr = aligned_alloc(align, size);
    }
    if unlikely(ptr == NULL) {
        handle_allocation_failure();
    }
    return ptr;
}

static void *heap_zero_allocate(_unused_ memory_allocator_handle_t alloc, size_t size, size_t align)
{
    if (align <= MALLOC_ALIGN) {
        void *ptr = calloc(1, size);

        if unlikely(ptr == NULL) {
            handle_allocation_failure();
        }
        return ptr;
    } else {
        void *ptr = aligned_alloc(align, size);

        if unlikely(ptr == NULL) {
            handle_allocation_failure();
        }
        return memset(ptr, 0, size);
    }
}

static void heap_deallocate(_unused_ memory_allocator_handle_t alloc, void *ptr)
{
    free(ptr);
}

static void *heap_reallocate(
    memory_allocator_handle_t alloc,
    void *ptr,
    size_t old_size,
    size_t new_size,
    size_t new_align
)
{
    if (new_size == 0) {
        free(ptr);
        return NULL;
    }

    if (ptr == NULL) {
        return heap_allocate(alloc, new_size, new_align);
    }

    if (new_align <= MALLOC_ALIGN) {
        ptr = realloc(ptr, new_size);

        if unlikely(ptr == NULL) {
            handle_allocation_failure();
        }
        return ptr;
    } else {
        void *new_ptr = heap_allocate(alloc, new_size, new_align);

        if (old_size > 0) {
            memcpy(new_ptr, ptr, MIN(old_size, new_size));
        }
        return new_ptr;
    }
}

struct memory_allocator heap_allocator = {
    .allocate = &heap_allocate,
    .zero_allocate = &heap_zero_allocate,
    .deallocate = &heap_deallocate,
    .reallocate = &heap_reallocate,
};

static void *static_allocate(_unused_ memory_allocator_handle_t alloc, _unused_ size_t size, _unused_ size_t align)
{
    assert(false);
}

static void *static_zero_allocate(_unused_ memory_allocator_handle_t alloc, _unused_ size_t size, _unused_ size_t align)
{
    assert(false);
}

static void static_deallocate(_unused_ memory_allocator_handle_t alloc, _unused_ void *ptr)
{
}

static void *static_reallocate(
    _unused_ memory_allocator_handle_t alloc,
    _unused_ void *ptr,
    _unused_ size_t old_size,
    _unused_ size_t new_size,
    _unused_ size_t new_align
)
{
    assert(false);
}

struct memory_allocator static_allocator = {
    .allocate = &static_allocate,
    .zero_allocate = &static_zero_allocate,
    .deallocate = &static_deallocate,
    .reallocate = &static_reallocate,
};

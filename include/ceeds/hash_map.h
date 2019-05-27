/*
** Created by doom on 24/05/19.
*/

#ifndef CEEDS_HASH_MAP_H
#define CEEDS_HASH_MAP_H

#include <ceeds/core.h>
#include <ceeds/memory.h>
#include <ceeds/hash_utils.h>
#include <ceeds/bitmanip.h>

/**
 * Hash maps
 */

#define hash_map_t(n)               hash_map_##n##_t

#define hash_map_empty_with_buffers(alloc_handle, keys, values, hashes, capacity)   \
    {alloc_handle, keys, values, memset(hashes, 0, capacity), 0, capacity}

#define hash_map_empty(alloc_handle)                                                \
    {alloc_handle, NULL, NULL, NULL, 0, 0}

#define hash_map_npos               ((size_t)-1)

/**
 * Destroy a hash map
 *
 * @param           n               the name of the hash map type
 * @param[in,out]   hm_ptr          a pointer to the hash map to destroy
 */
#define hash_map_destroy(n, hm_ptr)                                                 \
    _hash_map_destroy_##n(hm_ptr)

/**
 * Get the size of a hash map (i.e. the number of elements in the hash map)
 *
 * @param[in]       hm_ptr          a pointer to the hash map
 */
#define hash_map_size(hm_ptr)       ((hm_ptr)->size)

/**
 * Get the capacity of a hash map (i.e. the number of elements it currently has memory allocated for)
 *
 * @param[in]       hm_ptr          a pointer to the hash map
 */
#define hash_map_capacity(hm_ptr)   ((hm_ptr)->capacity)

/**
 * Increase the capacity of a hash map to be at least equal to a given amount
 *
 * @param           n               the name of the hash map type
 * @param[in,out]   hm_ptr          the hash map whose capacity is to be increased
 * @param[in]       new_capacity    the new capacity
 */
#define hash_map_reserve(n, hm_ptr, new_capacity)                                   \
    _hash_map_reserve_##n(hm_ptr, new_capacity)

/**
 * Insert an element into a hash map
 *
 * @param           n               the name of the hash map type
 * @param[in,out]   hm_ptr          a pointer to the hash map to insert into
 * @param[in]       key             the key to insert
 * @param[in]       value           the value to insert
 */
#define hash_map_insert(n, hm_ptr, key, value)                                      \
    _hash_map_insert_##n(hm_ptr, key, value)

/**
 * Insert an element into a hash map, when already knowing its hashed value
 *
 * @param           n               the name of the hash map type
 * @param[in,out]   hm_ptr          a pointer to the hash map to insert into
 * @param[in]       hash            the hash of the key to insert
 * @param[in]       key             the key to insert
 * @param[in]       value           the value to insert
 */
#define hash_map_insert_with_hash(n, hm_ptr, hash, key, value)                      \
    _hash_map_insert_with_hash_##n(hm_ptr, hash, key, value)

/**
 * Find the position of an element in a hash map
 *
 * @param           n               the name of the hash map type
 * @param[in,out]   hm_ptr          a pointer to the hash map to search into
 * @param[in]       key             the key to search for
 * @return                          the position of the element if found, hash_map_npos otherwise
 */
#define hash_map_find(n, hm_ptr, key)                                               \
    _hash_map_find_##n(hm_ptr, key)

/**
 * Erase the element at a given position in a hash map
 *
 * @param           n               the name of the hash map type
 * @param[in,out]   hm_ptr          a pointer to the hash map to erase from
 * @param[in]       pos             the position of the element to remove
 *
 * @pre                             @p pos must have been obtained through a call to hash_map_find_pos
 */
#define hash_map_erase_pos(n, hm_ptr, pos)                                          \
    _hash_map_erase_pos_##n(hm_ptr, pos)

/**
 * Erase the element with a given key in a hash map
 *
 * @param           n               the name of the hash map type
 * @param[in,out]   hm_ptr          a pointer to the hash map to erase from
 * @param[in]       key             the key to search for
 */
#define hash_map_erase(n, hm_ptr, key)                                              \
    _hash_map_erase_##n(hm_ptr, key)

/**
 * Create a hash map type
 *
 * @param           n               the name of the hash map type to create
 * @param           KeyT            the type of the keys to store
 * @param           ValueT          the type of the values to store
 * @param           key_hash        a function or function-like macro to hash @p KeyT objects
 * @param           key_cmp         a function or function-like macro to compare @p KeyT objects
 *
 * @pre                             @p cmp takes two parameters A and B, and returns a value R, with
 *                                  R == 0 if A == B
 *                                  R != 0 if A != B
 */
#define MAKE_HASH_MAP_TYPE(n, KeyT, ValueT, key_hash, key_cmp)                      \
    typedef struct {                                                                \
        memory_allocator_handle_t alloc;                                            \
        KeyT *keys;                                                                 \
        ValueT *values;                                                             \
        hash_value_t *hashes;                                                       \
        size_t size;                                                                \
        size_t capacity;                                                            \
    } hash_map_t(n);                                                                \
                                                                                    \
    static inline void _hash_map_destroy_##n(hash_map_t(n) *hm_ptr)                 \
    {                                                                               \
        allocator_delete(hm_ptr->alloc, hm_ptr->keys);                              \
        allocator_delete(hm_ptr->alloc, hm_ptr->values);                            \
        allocator_delete(hm_ptr->alloc, hm_ptr->hashes);                            \
    }                                                                               \
                                                                                    \
    static inline size_t _hash_map_find_ll_##n(                                     \
        const hash_map_t(n) *hm_ptr,                                                \
        hash_value_t hash,                                                          \
        KeyT const key                                                              \
    )                                                                               \
    {                                                                               \
        if (likely(hm_ptr->size)) {                                                 \
            size_t cur_slot = hash % hm_ptr->capacity;                              \
            size_t cur_dist = 0;                                                    \
                                                                                    \
            for (;;) {                                                              \
                if (                                                                \
                    _hash_map_is_empty_slot(hm_ptr->hashes[cur_slot]) ||            \
                    cur_dist > _hash_map_distance_to_ideal(hm_ptr, cur_slot)        \
                ) {                                                                 \
                    return hash_map_npos;                                           \
                } else if (                                                         \
                    hm_ptr->hashes[cur_slot] == hash &&                             \
                    key_cmp(key, hm_ptr->keys[cur_slot]) == 0                       \
                ) {                                                                 \
                    return cur_slot;                                                \
                }                                                                   \
                cur_slot = (cur_slot + 1) % hm_ptr->capacity;                       \
                cur_dist += 1;                                                      \
            }                                                                       \
        }                                                                           \
        return hash_map_npos;                                                       \
    }                                                                               \
                                                                                    \
    static inline size_t _hash_map_find_with_hash_##n(                              \
        const hash_map_t(n) *hm_ptr,                                                \
        hash_value_t hash,                                                          \
        KeyT const key                                                              \
    )                                                                               \
    {                                                                               \
        return _hash_map_find_ll_##n(hm_ptr, _hash_map_fix_hash(hash), key);        \
    }                                                                               \
                                                                                    \
    static inline size_t _hash_map_find_##n(const hash_map_t(n) *hm_ptr, KeyT const key)  \
    {                                                                               \
        hash_value_t hash = key_hash(key);                                          \
        size_t slot = _hash_map_find_with_hash_##n(hm_ptr, hash, key);              \
                                                                                    \
        return slot;                                                                \
    }                                                                               \
                                                                                    \
    static inline size_t _hash_map_insert_ll_##n(                                   \
        hash_map_t(n) *hm_ptr,                                                      \
        hash_value_t hash,                                                          \
        KeyT key,                                                                   \
        ValueT value                                                                \
    )                                                                               \
    {                                                                               \
        size_t cur_slot = hash % hm_ptr->capacity;                                  \
        size_t cur_dist = 0;                                                        \
                                                                                    \
        for (;;) {                                                                  \
            size_t other_dist;                                                      \
                                                                                    \
            if (_hash_map_is_empty_slot(hm_ptr->hashes[cur_slot])) {                \
                _hash_map_put(hm_ptr, cur_slot, hash, key, value);                  \
                return cur_slot;                                                    \
            }                                                                       \
            other_dist = _hash_map_distance_to_ideal(hm_ptr, cur_slot);             \
            if (other_dist < cur_dist) {                                            \
                if (_hash_map_is_tombstone(hm_ptr->hashes[cur_slot])) {             \
                    _hash_map_put(hm_ptr, cur_slot, hash, key, value);              \
                    return cur_slot;                                                \
                }                                                                   \
                SWAP(&hash, &hm_ptr->hashes[cur_slot]);                             \
                SWAP(&key, &hm_ptr->keys[cur_slot]);                                \
                SWAP(&value, &hm_ptr->values[cur_slot]);                            \
                cur_dist = other_dist;                                              \
            }                                                                       \
            cur_slot = (cur_slot + 1) % hm_ptr->capacity;                           \
            cur_dist += 1;                                                          \
        }                                                                           \
    }                                                                               \
                                                                                    \
    static inline void _hash_map_grow_##n(                                          \
        hash_map_t(n) *hm_ptr,                                                      \
        size_t new_cap                                                              \
    )                                                                               \
    {                                                                               \
        hash_value_t *old_hashes = hm_ptr->hashes;                                  \
        KeyT *old_keys = hm_ptr->keys;                                              \
        ValueT *old_values = hm_ptr->values;                                        \
        size_t old_capacity = hm_ptr->capacity;                                     \
                                                                                    \
        hm_ptr->hashes = allocator_znew_array(                                      \
            hm_ptr->alloc, hash_value_t, new_cap                                    \
        );                                                                          \
        hm_ptr->keys = allocator_new_array(hm_ptr->alloc, KeyT, new_cap);           \
        hm_ptr->values = allocator_new_array(hm_ptr->alloc, ValueT, new_cap);       \
        hm_ptr->capacity = new_cap;                                                 \
        hm_ptr->size = 0;                                                           \
        for (size_t i = 0; i < old_capacity; ++i) {                                 \
            if (                                                                    \
                !_hash_map_is_empty_slot(old_hashes[i]) &&                          \
                !_hash_map_is_tombstone(old_hashes[i])                              \
            ) {                                                                     \
                _hash_map_insert_ll_##n(                                            \
                    hm_ptr,                                                         \
                    old_hashes[i],                                                  \
                    old_keys[i],                                                    \
                    old_values[i]                                                   \
                );                                                                  \
            }                                                                       \
        }                                                                           \
        allocator_delete(hm_ptr->alloc, old_hashes);                                \
        allocator_delete(hm_ptr->alloc, old_keys);                                  \
        allocator_delete(hm_ptr->alloc, old_values);                                \
    }                                                                               \
                                                                                    \
    static inline void _hash_map_reserve_##n(                                       \
        hash_map_t(n) *hm_ptr,                                                      \
        size_t new_cap                                                              \
    )                                                                               \
    {                                                                               \
        if (hm_ptr->capacity * 90 / 100 < new_cap) {                                \
            _hash_map_grow_##n(hm_ptr, MAX(new_cap, 2 * hm_ptr->capacity));         \
        }                                                                           \
    }                                                                               \
                                                                                    \
                                                                                    \
    static inline size_t _hash_map_insert_with_hash_##n(                            \
        hash_map_t(n) *hm_ptr,                                                      \
        hash_value_t hash,                                                          \
        KeyT key,                                                                   \
        ValueT value                                                                \
    )                                                                               \
    {                                                                               \
        hash_map_reserve(n, hm_ptr, hm_ptr->size + 1);                              \
        hash = _hash_map_fix_hash(hash);                                            \
        return _hash_map_insert_ll_##n(hm_ptr, hash, key, value);                   \
    }                                                                               \
                                                                                    \
    static inline size_t _hash_map_insert_##n(                                      \
        hash_map_t(n) *hm_ptr,                                                      \
        KeyT key,                                                                   \
        ValueT value                                                                \
    )                                                                               \
    {                                                                               \
        hash_value_t hash = key_hash(key);                                          \
                                                                                    \
        return hash_map_insert_with_hash(n, hm_ptr, hash, key, value);              \
    }                                                                               \
                                                                                    \
    static inline void _hash_map_erase_pos_##n(hash_map_t(n) *hm_ptr, size_t pos)   \
    {                                                                               \
        hm_ptr->hashes[pos] |= bitmasknth_type(                                     \
            bitsizeof(hash_value_t) - 1,                                            \
            hash_value_t                                                            \
        );                                                                          \
        --hm_ptr->size;                                                             \
    }                                                                               \
                                                                                    \
    static inline void _hash_map_erase_##n(hash_map_t(n) *hm_ptr, KeyT const key)   \
    {                                                                               \
        size_t slot = hash_map_find(n, hm_ptr, key);                                \
                                                                                    \
        if (slot != hash_map_npos) {                                                \
            hash_map_erase_pos(n, hm_ptr, slot);                                    \
        }                                                                           \
    }                                                                               \
                                                                                    \
    struct _allow_semi_colon_##n { int unused; }

static _always_inline_ hash_value_t _hash_map_fix_hash(hash_value_t hash)
{
    hash &= ~bitmasknth_type(bitsizeof(hash_value_t) - 1, hash_value_t);
    hash |= hash == 0;
    return hash;
}

#define _hash_map_is_empty_slot(hash)                                               \
    (hash == 0)

#define _hash_map_is_tombstone(hash)                                                \
    (hash >> (bitsizeof(hash_value_t) - 1) != 0)

#define _hash_map_distance_to_ideal(hm_ptr, slot)                                   \
    (((hm_ptr)->capacity + slot - (hm_ptr)->hashes[slot] % (hm_ptr)->capacity) % (hm_ptr)->capacity)

#define _hash_map_put(hm_ptr, slot, h, k, v)                                        \
    do {                                                                            \
        hm_ptr->hashes[slot] = h;                                                   \
        hm_ptr->keys[slot] = k;                                                     \
        hm_ptr->values[slot] = v;                                                   \
        hm_ptr->size += 1;                                                          \
    } while (0)

#endif /* !CEEDS_HASH_MAP_H */

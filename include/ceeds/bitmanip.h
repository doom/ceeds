/*
** Created by doom on 11/04/19.
*/

#ifndef CEEDS_BITMANIP_H
#define CEEDS_BITMANIP_H

#include <ceeds/core.h>

/**
 * Get the size in bits of a given operand
 *
 * @param           x       the operand to get the size of
 * @return                  the size of the operand in bits
 */
#define bitsizeof(x)                (CHAR_BIT * sizeof(x))

/**
 * Get the size in bytes required to store a given number of bits
 *
 * @param[in]       n       the number of bits
 * @return                  the number of bytes
 *
 * @pre                     @p n must be greater than or equal than 0
 */
#define size_for_bits(n)            ((size_t)(((n) + CHAR_BIT - 1) / CHAR_BIT))

#define bitmasknth_type(nth, T)     (((T)1) << ((nth) & (bitsizeof(T) - 1)))

#define _ceeds_internal_apply_on_nth_bit(T, buf, nth, op)                   \
    ({                                                                      \
        typeof(nth) __nth = (nth);                                          \
                                                                            \
        (buf)[__nth / bitsizeof(T)] op bitmasknth_type(__nth, T);           \
    })

/**
 * Test whether a given bit is set in a bit buffer, returning a boolean
 *
 * @param[in]       buf     an array acting as a bit buffer
 * @param[in]       nth     the number of the bit to test
 * @return                  true if the bit is set, false otherwise
 *
 * @pre                     @p nth must be greater than or equal than 0
 * @pre                     @p nth must be less than the number of bits in @p buf
 */
#define is_bit_set(buf, nth)        (_ceeds_internal_apply_on_nth_bit(typeof((buf)[0]), buf, nth, &) != 0)

/**
 * Set a given bit in a bit buffer
 *
 * @param[in,out]   buf     an array acting as a bit buffer
 * @param[in]       nth     the number of the bit to set
 *
 * @pre                     @p nth must be greater than or equal than 0
 * @pre                     @p nth must be less than the number of bits in @p buf
 */
#define set_bit(buf, nth)           _ceeds_internal_apply_on_nth_bit(typeof((buf)[0]), buf, nth, |=)

/**
 * Flip a given bit in a bit buffer
 *
 * @param[in,out]   buf     an array acting as a bit buffer
 * @param[in]       nth     the number of the bit to flip
 *
 * @pre                     @p nth must be greater than or equal than 0
 * @pre                     @p nth must be less than the number of bits in @p buf
 */
#define flip_bit(buf, nth)          _ceeds_internal_apply_on_nth_bit(typeof((buf)[0]), buf, nth, ^=)

/**
 * Unset a given bit in a bit buffer
 *
 * @param[in,out]   buf     an array acting as a bit buffer
 * @param[in]       nth     the number of the bit to unset
 *
 * @pre                     @p nth must be greater than or equal than 0
 * @pre                     @p nth must be less than the number of bits in @p buf
 */
#define unset_bit(buf, nth)         _ceeds_internal_apply_on_nth_bit(typeof((buf)[0]), buf, nth, &= ~)

#endif /* !CEEDS_BITMANIP_H */

/*
** Created by doom on 11/04/19.
*/

#include "unit_tests.h"
#include <ceeds/bitmanip.h>

ut_test(bitsizeof)
{
    ut_assert_eq(bitsizeof(uint8_t), 8);
    ut_assert_eq(bitsizeof(uint16_t), 16);
    ut_assert_eq(bitsizeof(uint32_t), 32);
    ut_assert_eq(bitsizeof(uint64_t), 64);
}

ut_test(size_for_bits)
{
    ut_assert_eq(size_for_bits(0), 0);
    ut_assert_eq(size_for_bits(32), sizeof(uint32_t));
    ut_assert_eq(size_for_bits(CHAR_BIT - 1), sizeof(char));
#if CHAR_BIT == 8
    ut_assert_eq(size_for_bits(31), 32 / 8);
    ut_assert_eq(size_for_bits(33), 40 / 8);
#endif
}

ut_test(test_set_unset_flip)
{
    uint8_t bit_buf[size_for_bits(32)] = {0};

    ut_assert_eq(bitsizeof(bit_buf), 32);

    for (size_t i = 0; i < bitsizeof(bit_buf); ++i) {
        ut_assert_false(is_bit_set(bit_buf, i));
        set_bit(bit_buf, i);
        for (size_t j = 0; j < bitsizeof(bit_buf); ++j) {
            if (i == j) {
                ut_assert(is_bit_set(bit_buf, j));
            } else {
                ut_assert_false(is_bit_set(bit_buf, j));
            }
        }
        unset_bit(bit_buf, i);
        ut_assert_false(is_bit_set(bit_buf, i));
        flip_bit(bit_buf, i);
        ut_assert(is_bit_set(bit_buf, i));
        flip_bit(bit_buf, i);
        ut_assert_false(is_bit_set(bit_buf, i));
    }
}

ut_group(bitmanip,
         ut_get_test(bitsizeof),
         ut_get_test(size_for_bits),
         ut_get_test(test_set_unset_flip)
);

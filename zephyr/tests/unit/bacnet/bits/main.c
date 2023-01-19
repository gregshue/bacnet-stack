/*
 * Copyright (c) 2023 Legrand North America, LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */


#include <zephyr/ztest.h>

#undef BIT
#undef _BV
#undef BIT_SET
#undef BIT_CLEAR
#undef BIT_FLIP
#undef BIT_CHECK
#undef BITMASK_SET
#undef BITMASK_CLEAR
#undef BITMASK_FLIP
#undef BITMASK_CHECK

#include "bacnet/bits.h"


/* NOTE: These tests must be compatible with C90, so they do not
 * verify support for ULL.
 */

ZTEST(bacnet_bits, test_BIT)
{
    unsigned int bitpos = sizeof(unsigned long) * 8;

    do {
	bitpos -= 1;
        zassert_true(BIT(bitpos) == (1UL << bitpos), NULL);
    } while (bitpos > 0);
}

ZTEST(bacnet_bits, test__BV)
{
    unsigned int bitpos = sizeof(unsigned long) * 8;

    do {
	bitpos -= 1;
        zassert_equal(BIT(bitpos), (1UL << bitpos), NULL);
    } while (bitpos > 0);
}

ZTEST(bacnet_bits, test_BIT_SET)
{
    unsigned int bitpos = sizeof(unsigned long) * 8;

    do {
        unsigned long a = 0UL;
	bitpos -= 1;
	BIT_SET(a, bitpos);
        zassert_equal(a, (1UL << bitpos), NULL);
    } while (bitpos > 0);
}

ZTEST(bacnet_bits, test_BIT_CLEAR)
{
    unsigned int bitpos = sizeof(unsigned long) * 8;

    do {
        unsigned long a = ~0UL;
	bitpos -= 1;
	BIT_CLEAR(a, bitpos);
        zassert_equal(~(a), (1UL << bitpos), NULL);
    } while (bitpos > 0);
}

ZTEST(bacnet_bits, test_BIT_FLIP)
{
    unsigned int bitpos = sizeof(unsigned long) * 8;

    do {
        unsigned long a = ~0UL;
	bitpos -= 1;
	BIT_FLIP(a, bitpos);
        zassert_equal(~(a), (1UL << bitpos), NULL);
	BIT_FLIP(a, bitpos);
        zassert_equal(a, ~0UL, NULL);

	a = 0UL;
	BIT_FLIP(a, bitpos);
        zassert_equal(a, (1UL << bitpos), NULL);
	BIT_FLIP(a, bitpos);
        zassert_equal(a, 0UL, NULL);
	
    } while (bitpos > 0);
}

ZTEST(bacnet_bits, test_BIT_CHECK)
{
    unsigned int bitpos = sizeof(unsigned long) * 8;

    do {
        unsigned long a = ~0UL;
	bitpos -= 1;
	zassert_true(BIT_CHECK(a, bitpos), NULL);

	a = 0UL;
	zassert_false(BIT_CHECK(a, bitpos), NULL);
	
    } while (bitpos > 0);
}

ZTEST(bacnet_bits, test_BITMASK_SET)
{
    unsigned int bitpos = sizeof(unsigned long) * 8;

    do {
        unsigned long a = 0UL;
	bitpos -= 1;
	BITMASK_SET(a, (1UL << bitpos));
        zassert_equal(a, (1UL << bitpos), NULL);
    } while (bitpos > 0);
}

ZTEST(bacnet_bits, test_BITMASK_CLEAR)
{
    unsigned int bitpos = sizeof(unsigned long) * 8;

    do {
        unsigned long a = ~0UL;
	bitpos -= 1;
	BITMASK_CLEAR(a, (1UL << bitpos));
        zassert_equal(~(a), (1UL << bitpos), NULL);
    } while (bitpos > 0);
}

ZTEST(bacnet_bits, test_BITMASK_FLIP)
{
    unsigned int bitpos = sizeof(unsigned long) * 8;

    do {
        unsigned long a = ~0UL;
	bitpos -= 1;
	BITMASK_FLIP(a, (1UL << bitpos));
        zassert_equal(~(a), (1UL << bitpos), NULL);
	BITMASK_FLIP(a, (1UL << bitpos));
        zassert_equal(a, ~0UL, NULL);

	a = 0UL;
	BITMASK_FLIP(a, (1UL << bitpos));
        zassert_equal(a, (1UL << bitpos), NULL);
	BITMASK_FLIP(a, (1UL << bitpos));
        zassert_equal(a, 0UL, NULL);
	
    } while (bitpos > 0);
}

ZTEST(bacnet_bits, test_BITMASK_CHECK)
{
    unsigned int bitpos = sizeof(unsigned long) * 8;

    do {
        unsigned long a = ~0UL;
	bitpos -= 1;
	zassert_true(BITMASK_CHECK(a, (1UL << bitpos)), NULL);

	a = 0UL;
	zassert_false(BITMASK_CHECK(a, (1UL << bitpos)), NULL);
	
    } while (bitpos > 0);
}

ZTEST_SUITE(bacnet_bits, NULL, NULL, NULL, NULL, NULL);

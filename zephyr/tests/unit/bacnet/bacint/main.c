/*
 * Copyright (c) 2023 Legrand North America, LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "bacnet/bacint.h"
#include "bacnet/bacint.c"

#include <zephyr/ztest.h>
#include <string.h>


ZTEST(bacnet_bacint, testBACnetUnsigned16)
{
	uint8_t apdu[32] = { 0 };
	uint8_t expected[32] = { 0 };
	uint16_t value = 0;
        uint16_t test_value = 0;
	int len = 0;
	int i = 0;

	/* Test invalid parameter values */
	test_value = 0;
	memset(expected, 0xFF, sizeof(expected));
	memset(apdu, 0xFF, sizeof(apdu));
	len = encode_unsigned16(NULL, test_value);
	zassert_equal(len, 0, NULL);
	zassert_mem_equal(apdu, expected, sizeof(apdu), NULL);

	value = 0xFFFFU & ~test_value;
	len = decode_unsigned16(NULL, &value);
	zassert_equal(len, 0, NULL);
	zassert_equal(value, 0xFFFFU & ~test_value, NULL);
	
	len = decode_unsigned16(apdu, NULL);
	zassert_equal(len, 0, NULL);
	zassert_mem_equal(apdu, expected, sizeof(apdu), NULL);

	test_value = 0xFFFFU;
	memset(expected, 0, sizeof(expected));
	memset(apdu, 0, sizeof(apdu));
	len = encode_unsigned16(NULL, test_value);
	zassert_equal(len, 0, NULL);
	zassert_mem_equal(apdu, expected, sizeof(apdu), NULL);

	value = 0xFFFFU & ~test_value;
	len = decode_unsigned16(NULL, &value);
	zassert_equal(len, 0, NULL);
	zassert_equal(value, 0xFFFFU & ~test_value, NULL);

	len = decode_unsigned16(apdu, NULL);
	zassert_equal(len, 0, NULL);
	zassert_mem_equal(apdu, expected, sizeof(apdu), NULL);

	// Check for aligned write handling w/ min, max values
	test_value = 0;
	memset(expected, 0xFF, sizeof(expected));
	for (i = 0; i < sizeof(uint16_t); ++i) {
	    expected[0 + i] = 0xFFU & (test_value >> (8 * (sizeof(uint16_t) - 1 - i)));
	}
	memset(apdu, 0xFF, sizeof(apdu));
	len = encode_unsigned16(&apdu[0], test_value);
	zassert_equal(len, 2, NULL);
	zassert_mem_equal(apdu, expected, sizeof(apdu), NULL);

	value = ~test_value;
	len = decode_unsigned16(&apdu[0], &value);
	zassert_equal(len, 2, NULL);
	zassert_equal(value, test_value, NULL);
	zassert_mem_equal(apdu, expected, sizeof(apdu), NULL);

	test_value = 0xFFFFU;
	memset(expected, 0, sizeof(expected));
	for (i = 0; i < sizeof(uint16_t); ++i) {
	    expected[0 + i] = 0xFFU & (test_value >> (8 * (sizeof(uint16_t) - 1 - i)));
	}
	memset(apdu, 0, sizeof(apdu)/sizeof(apdu[0]));
	len = encode_unsigned16(&apdu[0], test_value);
	zassert_equal(len, 2, NULL);
	zassert_mem_equal(apdu, expected, sizeof(apdu), NULL);

	value = ~test_value;
	len = decode_unsigned16(&apdu[0], &value);
	zassert_equal(len, 2, NULL);
	zassert_equal(value, test_value, NULL);
	zassert_mem_equal(apdu, expected, sizeof(apdu), NULL);

	// Check for misaligned write handling w/ min, max values
	test_value = 0;
	memset(expected, 0xFF, sizeof(expected));
	for (i = 0; i < sizeof(uint16_t); ++i) {
	    expected[1 + i] = 0xFFU & (test_value >> (8 * (sizeof(uint16_t) - 1 - i)));
	}
	memset(apdu, 0xFF, sizeof(apdu)/sizeof(apdu[0]));
	len = encode_unsigned16(&apdu[1], test_value);
	zassert_equal(len, 2, NULL);
	zassert_mem_equal(apdu, expected, sizeof(apdu), NULL);

	value = ~test_value;
	len = decode_unsigned16(&apdu[1], &value);
	zassert_equal(len, 2, NULL);
	zassert_equal(value, test_value, NULL);
	zassert_mem_equal(apdu, expected, sizeof(apdu), NULL);

	test_value = 0xFFFFU;
	memset(expected, 0, sizeof(expected));
	for (i = 0; i < sizeof(uint16_t); ++i) {
	    expected[1 + i] = 0xFFU & (test_value >> (8 * (sizeof(uint16_t) - 1 - i)));
	}
	memset(apdu, 0, sizeof(apdu)/sizeof(apdu[0]));
	len = encode_unsigned16(&apdu[1], test_value);
	zassert_equal(len, 2, NULL);
	zassert_mem_equal(apdu, expected, sizeof(apdu), NULL);

	value = ~test_value;
	len = decode_unsigned16(&apdu[1], &value);
	zassert_equal(len, 2, NULL);
	zassert_equal(value, test_value, NULL);
	zassert_mem_equal(apdu, expected, sizeof(apdu), NULL);

	// Check for endianness with arbitrary values
	test_value = 0x1234U;
	memset(expected, 0xFF, sizeof(expected));
	for (i = 0; i < sizeof(uint16_t); ++i) {
	    expected[0 + i] = 0xFFU & (test_value >> (8 * (sizeof(uint16_t) - 1 - i)));
	}
	memset(apdu, 0xFF, sizeof(apdu));
	len = encode_unsigned16(&apdu[0], test_value);
	zassert_equal(len, 2, NULL);
	zassert_mem_equal(apdu, expected, sizeof(apdu), NULL);

	value = ~test_value;
	len = decode_unsigned16(&apdu[0], &value);
	zassert_equal(len, 2, NULL);
	zassert_equal(value, test_value, NULL);
	zassert_mem_equal(apdu, expected, sizeof(apdu), NULL);

	test_value = 0x4321U;
	memset(expected, 0, sizeof(expected));
	for (i = 0; i < sizeof(uint16_t); ++i) {
	    expected[1 + i] = 0xFFU & (test_value >> (8 * (sizeof(uint16_t) - 1 - i)));
	}
	memset(apdu, 0, sizeof(apdu));
	len = encode_unsigned16(&apdu[1], test_value);
	zassert_equal(len, 2, NULL);
	zassert_mem_equal(apdu, expected, sizeof(apdu), NULL);

	value = ~test_value;
	len = decode_unsigned16(&apdu[1], &value);
	zassert_equal(len, 2, NULL);
	zassert_equal(value, test_value, NULL);
	zassert_mem_equal(apdu, expected, sizeof(apdu), NULL);
}

ZTEST_SUITE(bacnet_bacint, NULL, NULL, NULL, NULL, NULL);

/*
 * Copyright (c) 2023 Legrand North America, LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "bacnet/bacdcode.c"

#include <zephyr/ztest.h>
#include <zephyr/fff.h>
#include <string.h> // for memset(), memcpy
#include <limits.h> // for INT_MAX, INT_MIN

#include "mocks/bacint.h"
#include "mocks/bacstr.h"
#include "mocks/bacreal.h"

#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif

DEFINE_FFF_GLOBALS;


static const uint8_t type_extended_value = 0x05U;
static const uint8_t type_opening_tag = 0x06U;
static const uint8_t type_closing_tag = 0x07U;
static const uint8_t type_context_specific = 0x08U;

static const uint8_t type_extended_value_as_u16 = 254;
static const uint8_t type_extended_value_as_u32 = 255;

//static const uint8_t bacnet_application_tag_NULL = 0;
static const uint8_t bacnet_application_tag_boolean = 1;
//static const uint8_t bacnet_application_tag_unsigned_int = 2;
//static const uint8_t bacnet_application_tag_signed_int = 3;
//static const uint8_t bacnet_application_tag_real = 4;
//static const uint8_t bacnet_application_tag_double = 5;
//static const uint8_t bacnet_application_tag_octet_string = 6;
//static const uint8_t bacnet_application_tag_character_string = 7;
//static const uint8_t bacnet_application_tag_bit_string = 8;
//static const uint8_t bacnet_application_tag_enumerated = 9;
//static const uint8_t bacnet_application_tag_date = 10;
//static const uint8_t bacnet_application_tag_time = 11;
//static const uint8_t bacnet_application_tag_object_id = 12;
//static const uint8_t bacnet_application_tag_reserve1 = 13;
//static const uint8_t bacnet_application_tag_reserve2 = 14;
//static const uint8_t bacnet_application_tag_reserve3 = 15;

/*
 * For FAKEs:
 *     Implement only the behavior necessary to programmatically
 *     exercise the API (including reporting all error conditions
 *     and side-effects).
 */
static int bacnet_unsigned_length_custom_fake(BACNET_UNSIGNED_INTEGER value)
{
    int len = 0;

    do
    {
        len += 1;
        value >>= 8;
    }
    while (value != 0ULL);

    return len;
}

static int bacnet_signed_length_custom_fake(int32_t value)
{
    int len = 0;

    if (-128 <= value && value <= 127)
    {
        len = 1;
    }
    else if (-32768 <= value && value <= 32767)
    {
        len = 2;
    }
    else if (-8388608 <= value && value <= 8388607)
    {
        len = 3;
    }
    else
    {
        len = 4;
    }

    return len;
}

static int decode_unsigned16_custom_fake(uint8_t * apdu, uint16_t *value)
{
    if ((apdu != NULL) && (value != NULL))
    {
        *value = (((uint16_t)apdu[0]) << 8) |
                 (((uint16_t)apdu[1]) << 0);
    }

    return decode_unsigned16_fake.return_val;
}

static int decode_unsigned32_custom_fake(uint8_t * apdu, uint32_t *value)
{
    if ((apdu != NULL) && (value != NULL))
    {
        *value = (((uint32_t)apdu[0]) <<24) |
                 (((uint32_t)apdu[1]) <<16) |
                 (((uint32_t)apdu[2]) << 8) |
                 (((uint32_t)apdu[3]) << 0);
    }

    return decode_unsigned32_fake.return_val;
}


static int encode_unsigned16_custom_fake(uint8_t * apdu, uint16_t value)
{
    int i;

    for(i = 0; (apdu) && (i <= 1); ++i)
    {
        apdu[i] = (uint8_t) (value >> (8 * (1 - i)));
    }

    return encode_unsigned16_fake.return_val;
}

static int encode_unsigned24_custom_fake(uint8_t * apdu, uint32_t value)
{
    int i;

    for(i = 0; (apdu) && (i <= 2); ++i)
    {
        apdu[i] = (uint8_t) (value >> (8 * (2 - i)));
    }

    return encode_unsigned24_fake.return_val;
}

static int encode_unsigned32_custom_fake(uint8_t * apdu, uint32_t value)
{
    int i;

    for(i = 0; (apdu) && (i <= 3); ++i)
    {
        apdu[i] = (uint8_t) (value >> (8 * (3 - i)));
    }

    return encode_unsigned32_fake.return_val;
}

static int encode_unsigned40_custom_fake(uint8_t * apdu, uint64_t value)
{
    int i;

    for(i = 0; (apdu) && (i <= 4); ++i)
    {
        apdu[i] = (uint8_t) (value >> (8 * (4 - i)));
    }

    return encode_unsigned40_fake.return_val;
}

static int encode_unsigned48_custom_fake(uint8_t * apdu, uint64_t value)
{
    int i;

    for(i = 0; (apdu) && (i <= 5); ++i)
    {
        apdu[i] = (uint8_t) (value >> (8 * (5 - i)));
    }

    return encode_unsigned48_fake.return_val;
}

static int encode_unsigned56_custom_fake(uint8_t * apdu, uint64_t value)
{
    int i;

    for(i = 0; (apdu) && (i <= 6); ++i)
    {
        apdu[i] = (uint8_t) (value >> (8 * (6 - i)));
    }

    return encode_unsigned56_fake.return_val;
}

static int encode_unsigned64_custom_fake(uint8_t * apdu, uint64_t value)
{
    int i;

    for(i = 0; (apdu) && (i <= 7); ++i)
    {
        apdu[i] = (uint8_t) (value >> (8 * (7 - i)));
    }

    return encode_unsigned64_fake.return_val;
}

static int encode_signed8_custom_fake(uint8_t * apdu, int8_t value)
{
    apdu[0] = (uint8_t) value;

    return encode_signed8_fake.return_val;
}

static int encode_signed16_custom_fake(uint8_t * apdu, int16_t value)
{
    int i;

    for(i = 0; (apdu) && (i <= 1); ++i)
    {
        apdu[i] = (uint8_t) (value >> (8 * (1 - i)));
    }

    return encode_signed16_fake.return_val;
}

static int encode_signed24_custom_fake(uint8_t * apdu, int32_t value)
{
    int i;

    for(i = 0; (apdu) && (i <= 2); ++i)
    {
        apdu[i] = (uint8_t) (value >> (8 * (2 - i)));
    }

    return encode_signed24_fake.return_val;
}

static int encode_signed32_custom_fake(uint8_t * apdu, int32_t value)
{
    int i;

    for(i = 0; (apdu) && (i <= 3); ++i)
    {
        apdu[i] = (uint8_t) (value >> (8 * (3 - i)));
    }

    return encode_signed32_fake.return_val;
}


/*
 * Unit Tests (mocking calls outside of bacdcode.c
 */


static void test_decode_max_segs(void)
{
    zassert_equal(decode_max_segs(0x00U),  0, NULL);
    zassert_equal(decode_max_segs(0x0FU),  0, NULL);
    zassert_equal(decode_max_segs(0x1EU),  2, NULL);
    zassert_equal(decode_max_segs(0x11U),  2, NULL);
    zassert_equal(decode_max_segs(0x2DU),  4, NULL);
    zassert_equal(decode_max_segs(0x22U),  4, NULL);
    zassert_equal(decode_max_segs(0x3CU),  8, NULL);
    zassert_equal(decode_max_segs(0x33U),  8, NULL);
    zassert_equal(decode_max_segs(0x4BU), 16, NULL);
    zassert_equal(decode_max_segs(0x44U), 16, NULL);
    zassert_equal(decode_max_segs(0x5AU), 32, NULL);
    zassert_equal(decode_max_segs(0x55U), 32, NULL);
    zassert_equal(decode_max_segs(0x69U), 64, NULL);
    zassert_equal(decode_max_segs(0x66U), 64, NULL);
    zassert_equal(decode_max_segs(0x78U), 65, NULL);
    zassert_equal(decode_max_segs(0x77U), 65, NULL);
    zassert_equal(decode_max_segs(0x87U),  0, NULL);
    zassert_equal(decode_max_segs(0x96U),  0, NULL);
    zassert_equal(decode_max_segs(0xA5U),  0, NULL);
    zassert_equal(decode_max_segs(0xB4U),  0, NULL);
    zassert_equal(decode_max_segs(0xC3U),  0, NULL);
    zassert_equal(decode_max_segs(0xD2U),  0, NULL);
    zassert_equal(decode_max_segs(0xE1U),  0, NULL);
    zassert_equal(decode_max_segs(0xF0U),  0, NULL);
    zassert_equal(decode_max_segs(0xFFU),  0, NULL);
}

static void test_decode_max_apdu(void)
{
    zassert_equal(decode_max_apdu(0x00U),   50, NULL);
    zassert_equal(decode_max_apdu(0xF0U),   50, NULL);
    zassert_equal(decode_max_apdu(0x11U),  128, NULL);
    zassert_equal(decode_max_apdu(0xE1U),  128, NULL);
    zassert_equal(decode_max_apdu(0x22U),  206, NULL);
    zassert_equal(decode_max_apdu(0xD2U),  206, NULL);
    zassert_equal(decode_max_apdu(0x33U),  480, NULL);
    zassert_equal(decode_max_apdu(0xC3U),  480, NULL);
    zassert_equal(decode_max_apdu(0x44U), 1024, NULL);
    zassert_equal(decode_max_apdu(0xB4U), 1024, NULL);
    zassert_equal(decode_max_apdu(0x55U), 1476, NULL);
    zassert_equal(decode_max_apdu(0xA5U), 1476, NULL);
    zassert_equal(decode_max_apdu(0x96U),    0, NULL);
    zassert_equal(decode_max_apdu(0x87U),    0, NULL);
    zassert_equal(decode_max_apdu(0x78U),    0, NULL);
    zassert_equal(decode_max_apdu(0x69U),    0, NULL);
    zassert_equal(decode_max_apdu(0x5AU),    0, NULL);
    zassert_equal(decode_max_apdu(0x4BU),    0, NULL);
    zassert_equal(decode_max_apdu(0x3CU),    0, NULL);
    zassert_equal(decode_max_apdu(0x2DU),    0, NULL);
    zassert_equal(decode_max_apdu(0x1EU),    0, NULL);
    zassert_equal(decode_max_apdu(0x0FU),    0, NULL);
    zassert_equal(decode_max_apdu(0xFFU),    0, NULL);
}

static void test_encode_max_segs_max_apdu(void)
{
    zassert_equal(encode_max_segs_max_apdu(INT_MIN, 0), 0x00, NULL);
    zassert_equal(encode_max_segs_max_apdu(     -1, 0), 0x00, NULL);
    zassert_equal(encode_max_segs_max_apdu(      0, 0), 0x00, NULL);
    zassert_equal(encode_max_segs_max_apdu(      1, 0), 0x00, NULL);
    zassert_equal(encode_max_segs_max_apdu(      2, 0), 0x10, NULL);
    zassert_equal(encode_max_segs_max_apdu(      3, 0), 0x10, NULL);
    zassert_equal(encode_max_segs_max_apdu(      4, 0), 0x20, NULL);
    zassert_equal(encode_max_segs_max_apdu(      7, 0), 0x20, NULL);
    zassert_equal(encode_max_segs_max_apdu(      8, 0), 0x30, NULL);
    zassert_equal(encode_max_segs_max_apdu(     15, 0), 0x30, NULL);
    zassert_equal(encode_max_segs_max_apdu(     16, 0), 0x40, NULL);
    zassert_equal(encode_max_segs_max_apdu(     31, 0), 0x40, NULL);
    zassert_equal(encode_max_segs_max_apdu(     32, 0), 0x50, NULL);
    zassert_equal(encode_max_segs_max_apdu(     63, 0), 0x50, NULL);
    zassert_equal(encode_max_segs_max_apdu(     65, 0), 0x70, NULL);
    zassert_equal(encode_max_segs_max_apdu(INT_MAX, 0), 0x70, NULL);

    zassert_equal(encode_max_segs_max_apdu(0, INT_MIN), 0x00, NULL);
    zassert_equal(encode_max_segs_max_apdu(0,      -1), 0x00, NULL);
    zassert_equal(encode_max_segs_max_apdu(0,       0), 0x00, NULL);
    zassert_equal(encode_max_segs_max_apdu(0,       1), 0x00, NULL);
    zassert_equal(encode_max_segs_max_apdu(0,      50), 0x00, NULL);
    zassert_equal(encode_max_segs_max_apdu(0,      51), 0x01, NULL);
    zassert_equal(encode_max_segs_max_apdu(0,     128), 0x01, NULL);
    zassert_equal(encode_max_segs_max_apdu(0,     129), 0x02, NULL);
    zassert_equal(encode_max_segs_max_apdu(0,     206), 0x02, NULL);
    zassert_equal(encode_max_segs_max_apdu(0,     207), 0x03, NULL);
    zassert_equal(encode_max_segs_max_apdu(0,     480), 0x03, NULL);
    zassert_equal(encode_max_segs_max_apdu(0,     481), 0x04, NULL);
    zassert_equal(encode_max_segs_max_apdu(0,    1024), 0x04, NULL);
    zassert_equal(encode_max_segs_max_apdu(0,    1025), 0x05, NULL);
    zassert_equal(encode_max_segs_max_apdu(0,    1476), 0x05, NULL);
    zassert_equal(encode_max_segs_max_apdu(0,    1477), 0x00, NULL);
    zassert_equal(encode_max_segs_max_apdu(0, INT_MAX), 0x00, NULL);

    zassert_equal(encode_max_segs_max_apdu(5,     300), 0x23, NULL);
}

static void test_encode_tag(void)
{
    struct test_point
    {
        uint8_t buf_fill;
        uint8_t tag_number;
        bool context_specific;
        uint32_t len_value_type;
        int expected_len;
        uint8_t expected_encoding;
    };

    const struct test_point test_points[] =
    {
        { /* Verify context_specific bit not set */
            .buf_fill = 0xFFU,
            .tag_number = 0,
            .context_specific = false,
            .len_value_type = 0,
            .expected_len = 1,
            .expected_encoding = 0x00U,
        },
        { /* Verify context_specific bit is set */
            .buf_fill = 0xEEU,
            .tag_number = 0,
            .context_specific = true,
            .len_value_type = 0,
            .expected_len = 1,
            .expected_encoding = 0x08U,
        },
        {
            .buf_fill = 0xA5U,
            .tag_number = 14,
            .context_specific = true,
            .len_value_type = 0,
            .expected_len = 1,
            .expected_encoding = (14 << 4) | 0x08U,
        },
        {
            .buf_fill = 0x5AU,
            .tag_number = 15,
            .context_specific = true,
            .len_value_type = 0,
            .expected_len = 2,
            .expected_encoding = (0xFU << 4) | 0x08U,
        },
        {
            .buf_fill = 0x4BU,
            .tag_number = 0xFFU,
            .context_specific = false,
            .len_value_type = 0,
            .expected_len = 2,
            .expected_encoding = (0xFU << 4),
        },
        {
            .buf_fill = 0xB9U,
            .tag_number = 0,
            .context_specific = false,
            .len_value_type = 4,
            .expected_len = 1,
            .expected_encoding = (0U << 4) | 4,
        },
        {
            .buf_fill = 0xB7U,
            .tag_number = 0,
            .context_specific = false,
            .len_value_type = 5,
            .expected_len = 2,
            .expected_encoding = (0U << 4) | 5,
        },
        {
            .buf_fill = 0xB7U,
            .tag_number = 0xC3,
            .context_specific = true,
            .len_value_type = 253,
            .expected_len = 3,
            .expected_encoding = (0xFU << 4) | 0x08 | 5,
        },
        {
            .buf_fill = 0xB6U,
            .tag_number = 0x3C,
            .context_specific = false,
            .len_value_type = 254,
            .expected_len = 5,
            .expected_encoding = (0xFU << 4) | 5,
        },
        {
            .buf_fill = 0xB5U,
            .tag_number = 0x3C,
            .context_specific = true,
            .len_value_type = 65535,
            .expected_len = 5,
            .expected_encoding = (0xFU << 4) | 0x08 | 5,
        },
        {
            .buf_fill = 0x5AU,
            .tag_number = 0x3,
            .context_specific = false,
            .len_value_type = 65536,
            .expected_len = 6,
            .expected_encoding = (0x3U << 4) | 5,
        },
        {
            .buf_fill = 0xB3U,
            .tag_number = 0x3C,
            .context_specific = true,
            .len_value_type = 65536,
            .expected_len = 7,
            .expected_encoding = (0xFU << 4) | 0x08 | 5,
        },
        {
            .buf_fill = 0xB3U,
            .tag_number = 0x3C,
            .context_specific = true,
            .len_value_type = (uint32_t)(~0UL),
            .expected_len = 7,
            .expected_encoding = (0xFU << 4) | 0x08 | 5,
        },
    };
    int i = 0;


    for (i = 0; i < sizeof(test_points)/sizeof(test_points[0]); ++i)
    {
        const struct test_point * const tp = &test_points[i];
        const int base_offset = 2;
        int offset = base_offset;
        uint8_t buf[16] = { 0 };

        FFF_RESET_HISTORY();
        BACINT_FFF_FAKES_LIST(RESET_FAKE);

        encode_unsigned16_fake.return_val = 2;
        encode_unsigned32_fake.return_val = 4;

        /* NOTE: When the parameter 'apdu' is NULL it returns the number
         * of bytes that the encoding would have consumed.
         */
        zassert_equal(encode_tag(NULL,
                                 tp->tag_number,
                                 tp->context_specific,
                                 tp->len_value_type),
                      tp->expected_len, NULL);

        memset(buf, tp->buf_fill, sizeof(buf));
        zassert_equal(encode_tag(&buf[offset],
                                 tp->tag_number,
                                 tp->context_specific,
                                 tp->len_value_type),
                      tp->expected_len, NULL);

        zassert_equal(buf[offset++], tp->expected_encoding, NULL);
        if ((tp->expected_encoding >> 4) == 0xFU)
        {
            zassert_equal(buf[offset++], tp->tag_number, NULL);
        }
        if ((tp->expected_encoding & 0x07U) == 5)
        {
            if (tp->len_value_type <= 4)
            {
                zassert_equal(0x7U & buf[base_offset],
                              tp->len_value_type, NULL);
            }
            else
            {
                zassert_equal(0x7U & buf[base_offset],
                              5, NULL);
                if (tp->len_value_type <= 253)
                {
                    zassert_equal(buf[offset++],
                                  tp->len_value_type, NULL);
                }
                else if (tp->len_value_type <= 65535)
                {
                    /* Verify the call was made as expected */
                    zassert_equal(encode_unsigned16_fake.call_count, 2, NULL);
                    zassert_equal(encode_unsigned32_fake.call_count, 0, NULL);
                    zassert_equal(encode_unsigned16_fake.arg0_val,
                                  &buf[offset + 1], NULL);
                    zassert_equal(encode_unsigned16_fake.arg1_val,
                                  tp->len_value_type, NULL);

                    zassert_equal(buf[offset], 254, NULL);
                    offset += encode_unsigned16_fake.return_val;
                }
                else
                {
                    /* Verify the call was made as expected */
                    zassert_equal(encode_unsigned16_fake.call_count, 0, NULL);
                    zassert_equal(encode_unsigned32_fake.call_count, 2, NULL);
                    zassert_equal(encode_unsigned32_fake.arg0_val,
                                  &buf[offset + 1], NULL);
                    zassert_equal(encode_unsigned32_fake.arg1_val,
                                  tp->len_value_type, NULL);

                    zassert_equal(buf[offset], 255, NULL);
                    offset += encode_unsigned32_fake.return_val;
                }
            }
        }

        /* Check for not writing memory off the end */
        zassert_equal(buf[1], tp->buf_fill, NULL);
        zassert_equal(buf[1 + tp->expected_len + 1], tp->buf_fill, NULL);
    }


    /* Verify size of encode_unsigned16 is used */
    encode_unsigned16_fake.return_val = 2 + 5;
    zassert_equal(encode_tag(NULL, 0x3CU, false, 254),
                  3 + encode_unsigned16_fake.return_val, NULL);

    /* Verify size of encode_unsigned32 is used */
    encode_unsigned32_fake.return_val = 4 + 9;
    zassert_equal(encode_tag(NULL, 0x3CU, true, (uint32_t)(~0UL)),
                  3 + encode_unsigned32_fake.return_val, NULL);
}

static void test_encode_opening_tag__embedded(void)
{
    const uint8_t test_tag_numbers[] = { 0, 1, 13, 14 };
    int i = 0;


    for (i = 0; i < sizeof(test_tag_numbers)/sizeof(test_tag_numbers[0]); ++i)
    {
        uint8_t buf[16] = { 0 };
        uint8_t buf_fill = 0;
        const uint8_t tag_number = test_tag_numbers[i];
        int len = 0;


        len = encode_opening_tag(NULL, tag_number);
        zassert_equal(len, 1, NULL);

        memset(buf, buf_fill=0xFFU, sizeof(buf));
        len = encode_opening_tag(&buf[2], tag_number);
        zassert_equal(len, 1, NULL);
        zassert_equal(buf[2],
                      ((tag_number << 4) | type_context_specific |
                        type_opening_tag), NULL);
        zassert_equal(buf[1], buf_fill, NULL);
        zassert_equal(buf[3], buf_fill, NULL);
    }
}

static void test_encode_opening_tag__extended(void)
{
    const uint8_t test_tag_numbers[] = { 15, 16, 0xFEU, 0xFFU };
    int i = 0;


    for (i = 0; i < sizeof(test_tag_numbers)/sizeof(test_tag_numbers[0]); ++i)
    {
        uint8_t buf[16] = { 0 };
        uint8_t buf_fill = 0;
        const uint8_t tag_number = test_tag_numbers[i];
        int len = 0;


        len = encode_opening_tag(NULL, tag_number);
        zassert_equal(len, 2, NULL);

        memset(buf, buf_fill=0xFFU, sizeof(buf));
        len = encode_opening_tag(&buf[2], tag_number);
        zassert_equal(len, 2, NULL);
        zassert_equal(buf[2],
                      0xF0U | type_context_specific | type_opening_tag,
                      NULL);
        zassert_equal(buf[3], tag_number, NULL);
        zassert_equal(buf[1], buf_fill, NULL);
        zassert_equal(buf[4], buf_fill, NULL);
    }
}

static void test_encode_closing_tag__embedded(void)
{
    const uint8_t test_tag_numbers[] = { 0, 1, 13, 14 };
    int i = 0;


    for (i = 0; i < sizeof(test_tag_numbers)/sizeof(test_tag_numbers[0]); ++i)
    {
        uint8_t buf[16] = { 0 };
        uint8_t buf_fill = 0;
        const uint8_t tag_number = test_tag_numbers[i];
        int len = 0;


        len = encode_closing_tag(NULL, tag_number);
        zassert_equal(len, 1, NULL);

        memset(buf, buf_fill=0xFFU, sizeof(buf));
        len = encode_closing_tag(&buf[2], tag_number);
        zassert_equal(len, 1, NULL);
        zassert_equal(buf[2],
                      ((tag_number << 4) |
                        type_context_specific |
                        type_closing_tag), NULL);
        zassert_equal(buf[1], buf_fill, NULL);
        zassert_equal(buf[3], buf_fill, NULL);
    }
}

static void test_encode_closing_tag__extended(void)
{
    const uint8_t test_tag_numbers[] = { 15, 16, 0xFEU, 0xFFU };
    int i = 0;


    for (i = 0; i < sizeof(test_tag_numbers)/sizeof(test_tag_numbers[0]); ++i)
    {
        uint8_t buf[16] = { 0 };
        uint8_t buf_fill = 0;
        const uint8_t tag_number = test_tag_numbers[i];
        int len = 0;


        len = encode_closing_tag(NULL, tag_number);
        zassert_equal(len, 2, NULL);

        memset(buf, buf_fill=0xFFU, sizeof(buf));
        len = encode_closing_tag(&buf[2], tag_number);
        zassert_equal(len, 2, NULL);
        zassert_equal(buf[2],
                      (0xF0U | type_context_specific | type_closing_tag),
                      NULL);
        zassert_equal(buf[3], tag_number, NULL);
        zassert_equal(buf[1], buf_fill, NULL);
        zassert_equal(buf[4], buf_fill, NULL);
    }
}

static void test_decode_tag_number(void)
{
    struct test_point {
        int expected_len;
        uint8_t expected_tag_number;
        uint8_t apdu[2];
        uint8_t apdu_len;
        uint8_t apdu_offset;
        uint8_t buf_fill;
    };

    const struct test_point test_points[] =
    {
        {
            .expected_len = 1,
            .expected_tag_number = 0,
            .apdu =
                {
                  (0 << 4) | type_context_specific | type_opening_tag,
                },
            .apdu_len = 1,
            .apdu_offset = 2,
            .buf_fill = 0xDCU,
        },
        {
            .expected_len = 1,
            .expected_tag_number = 14,
            .apdu =
                {
                  (14 << 4) | type_context_specific | type_closing_tag,
                },
            .apdu_len = 1,
            .apdu_offset = 1,
            .buf_fill = 0xCDU,
        },
        {
            .expected_len = 2,
            .expected_tag_number = 15,
            .apdu =
                {
                  (0xFU << 4) | type_context_specific | type_closing_tag,
                  15,
                },
            .apdu_len = 2,
            .apdu_offset = 2,
            .buf_fill = 0xABU,
        },
        {
            .expected_len = 2,
            .expected_tag_number = 0xFFU,
            .apdu =
                {
                  (0xFU << 4) | type_context_specific | type_closing_tag,
                  0xFFU,
                },
            .apdu_len = 2,
            .apdu_offset = 1,
            .buf_fill = 0xBAU,
        },
    };
    uint8_t tag_number = 0;
    int len = 0;
    int i = 0;


    for (i = 0; i < sizeof(test_points)/sizeof(test_points[0]); ++i)
    {
        const struct test_point * const tp = &test_points[i];
        const int base_offset = tp->apdu_offset;
        uint8_t buf[16] = { 0 };


        memset(buf, tp->buf_fill, sizeof(buf));
        memcpy(&buf[base_offset], tp->apdu,
                MIN(sizeof(tp->apdu), tp->apdu_len));

        len = decode_tag_number(&buf[base_offset], NULL);
        zassert_equal(len, tp->expected_len, NULL);

        zassert_equal(buf[base_offset - 1], tp->buf_fill, NULL);
        zassert_equal(buf[base_offset + tp->apdu_len], tp->buf_fill, NULL);


        tag_number = (uint8_t)~tp->expected_tag_number;

        len = decode_tag_number(&buf[base_offset], &tag_number);
        zassert_equal(len, tp->expected_len, NULL);
        zassert_equal(tag_number, tp->expected_tag_number, NULL);

        zassert_equal(buf[base_offset - 1], tp->buf_fill, NULL);
        zassert_equal(buf[base_offset + tp->apdu_len], tp->buf_fill, NULL);
    }

#if 0
    /*
     * Verify when NULL apdu provided
     */
    len = decode_tag_number(NULL, NULL);
    zassert_equal(len, 0, NULL);

    tag_number = 39;
    len = decode_tag_number(NULL, &tag_number);
    zassert_equal(len, 0, NULL);
    zassert_equal(tag_number, 39, NULL);

    tag_number = 0xA5;
    len = decode_tag_number(NULL, &tag_number);
    zassert_equal(len, 0, NULL);
    zassert_equal(tag_number, 0xA5, NULL);
#endif

}

static void test_bacnet_tag_number_decode(void)
{
    struct test_point {
        int expected_len;
        uint8_t expected_tag_number;
        uint8_t apdu[2];
        uint8_t apdu_len;
        uint8_t apdu_offset;
        uint8_t buf_fill;
    };

    const struct test_point test_points[] =
    {
        {
            .expected_len = 1,
            .expected_tag_number = 0,
            .apdu =
                {
                  (0 << 4) | type_context_specific | type_closing_tag,
                },
            .apdu_len = 1,
            .apdu_offset = 2,
            .buf_fill = 0xDCU,
        },
        {
            .expected_len = 1,
            .expected_tag_number = 14,
            .apdu =
                {
                  (14 << 4) | type_context_specific | type_closing_tag,
                },
            .apdu_len = 1,
            .apdu_offset = 1,
            .buf_fill = 0xE7,
        },
        {
            .expected_len = 2,
            .expected_tag_number = 15,
            .apdu =
                {
                  (0xFU << 4) | type_opening_tag,
                  15,
                },
            .apdu_len = 2,
            .apdu_offset = 1,
            .buf_fill = 0x83,
        },
        {
            .expected_len = 2,
            .expected_tag_number = 0xFFU,
            .apdu =
                {
                  (0xFU << 4),
                  0xFFU,
                },
            .apdu_len = 2,
            .apdu_offset = 2,
            .buf_fill = 0x38,
        },
    };
    const uint8_t test_tag_number = 39; /* For trailing tests */
    uint8_t tag_number = 0;
    int len = 0;
    int i = 0;


    for (i = 0; i < sizeof(test_points)/sizeof(test_points[0]); ++i)
    {
        const struct test_point * const tp = &test_points[i];
        const int base_offset = tp->apdu_offset;
        uint8_t buf[16] = { 0 };


        memset(buf, tp->buf_fill, sizeof(buf));
        memcpy(&buf[base_offset], tp->apdu,
               MIN(sizeof(tp->apdu), tp->apdu_len));

        len = bacnet_tag_number_decode(&buf[base_offset],
                                       tp->apdu_len + 1, NULL);
        zassert_equal(len, tp->expected_len, NULL);

        zassert_equal(buf[base_offset - 1], tp->buf_fill, NULL);
        zassert_equal(buf[base_offset + tp->apdu_len], tp->buf_fill, NULL);


        tag_number = (uint8_t)~tp->expected_tag_number;

        len = bacnet_tag_number_decode(&buf[base_offset],
                                       tp->apdu_len, &tag_number);
        zassert_equal(len, tp->expected_len, NULL);
        zassert_equal(tag_number, tp->expected_tag_number, NULL);

        zassert_equal(buf[base_offset - 1], tp->buf_fill, NULL);
        zassert_equal(buf[base_offset + tp->apdu_len], tp->buf_fill, NULL);


        tag_number = (uint8_t)~tp->expected_tag_number;

        len = bacnet_tag_number_decode(&buf[base_offset],
                                       tp->apdu_len - 1, &tag_number);
        zassert_equal(len, 0, NULL);
        zassert_equal(tag_number, (uint8_t)~tp->expected_tag_number, NULL);

        zassert_equal(buf[base_offset - 1], tp->buf_fill, NULL);
        zassert_equal(buf[base_offset + tp->apdu_len], tp->buf_fill, NULL);
    }


    /*
     * Verify when NULL apdu provided
     */
    len = bacnet_tag_number_decode(NULL, 0, NULL);
    zassert_equal(len, 0, NULL);

    len = bacnet_tag_number_decode(NULL, 1, NULL);
    zassert_equal(len, 0, NULL);

    tag_number = test_tag_number;
    len = bacnet_tag_number_decode(NULL, 1, &tag_number);
    zassert_equal(len, 0, NULL);
    zassert_equal(tag_number, test_tag_number, NULL);

    tag_number = test_tag_number + 1;
    len = bacnet_tag_number_decode(NULL, 1, &tag_number);
    zassert_equal(len, 0, NULL);
    zassert_equal(tag_number, test_tag_number + 1, NULL);

    /*
     * Verify when zero (0) apdu_len_max provided
     */
    len = bacnet_tag_number_decode((uint8_t*)&test_tag_number, 0, NULL);
    zassert_equal(len, 0, NULL);

    tag_number = test_tag_number;
    len = bacnet_tag_number_decode((uint8_t*)&test_tag_number, 0, &tag_number);
    zassert_equal(len, 0, NULL);
    zassert_equal(tag_number, test_tag_number, NULL);

    tag_number = test_tag_number + 1;
    len = bacnet_tag_number_decode((uint8_t*)&test_tag_number, 0, &tag_number);
    zassert_equal(len, 0, NULL);
    zassert_equal(tag_number, test_tag_number + 1, NULL);
}

static void test_decode_is_opening_tag(void)
{
    uint8_t buf[4] = { 0 };


    zassert_false(decode_is_opening_tag(NULL), NULL);

    memset(buf, 0xFDU, sizeof(buf));
    buf[2] = 0;
    zassert_false(decode_is_opening_tag(&buf[2]), NULL);
    buf[1] = 0x5U;
    zassert_false(decode_is_opening_tag(&buf[1]), NULL);
    buf[2] = 0x6U;
    zassert_true(decode_is_opening_tag(&buf[2]), NULL);
    buf[2] = 0xF6U;
    zassert_true(decode_is_opening_tag(&buf[2]), NULL);
    buf[1] = 0xF7U;
    zassert_false(decode_is_opening_tag(&buf[1]), NULL);
    buf[1] = 0xFFU;
    zassert_false(decode_is_opening_tag(&buf[1]), NULL);
}

static void test_decode_is_closing_tag(void)
{
    uint8_t buf[4] = { 0 };


    zassert_false(decode_is_closing_tag(NULL), NULL);

    memset(buf, 0xFDU, sizeof(buf));
    buf[2] = 0;
    zassert_false(decode_is_closing_tag(&buf[2]), NULL);
    buf[1] = 0x6U;
    zassert_false(decode_is_closing_tag(&buf[1]), NULL);
    buf[2] = 0x7U;
    zassert_true(decode_is_closing_tag(&buf[2]), NULL);
    buf[2] = 0xF7U;
    zassert_true(decode_is_closing_tag(&buf[2]), NULL);
    buf[1] = 0xF8U;
    zassert_false(decode_is_closing_tag(&buf[1]), NULL);
    buf[1] = 0xFFU;
    zassert_true(decode_is_closing_tag(&buf[1]), NULL);
}

static void test_decode_tag_number_and_value__NULL_apdu(void)
{
    uint8_t buf[16] = { 0 };
    uint8_t test_tag_number = 0;
    uint8_t tag_number = 0;
    uint32_t test_value = 0;
    uint32_t value = 0;
    int len = 0;


    /*
     * Verify invalid parameters
     */
    memset(buf, 0xFDU, sizeof(buf));
    test_tag_number = 0;
    test_value = 0;
    tag_number = test_tag_number;

    len = decode_tag_number_and_value(NULL, NULL, NULL);
    zassert_equal(len, 0, NULL);

    test_value = 12;
    value = test_value;
    len = decode_tag_number_and_value(NULL, NULL, &value);
    zassert_equal(len, 0, NULL);
    zassert_equal(value, test_value, NULL);

    test_tag_number = 0x57U;
    tag_number = test_tag_number;
    len = decode_tag_number_and_value(NULL, &tag_number, NULL);
    zassert_equal(len, 0, NULL);
    zassert_equal(tag_number, test_tag_number, NULL);

    test_tag_number = 0xF9U;
    tag_number = test_tag_number;
    test_value = 0xC8U;
    value = test_value;
    len = decode_tag_number_and_value(NULL, &tag_number, &value);
    zassert_equal(len, 0, NULL);
    zassert_equal(tag_number, test_tag_number, NULL);
    zassert_equal(value, test_value, NULL);
}

static void test_decode_tag_number_and_value__special_tags(void)
{
    const uint8_t special_tags[] =
    {
        type_opening_tag,
        type_closing_tag
    };
    int i = 0;


    /*
     * Verify opening_tag (reads as value 0)
     */
    for (i = 0; i < sizeof(special_tags)/sizeof(special_tags[0]); i++)
    {
        uint8_t buf[16] = { 0 };
        uint8_t buf_fill = 0;
        const uint8_t test_tag_number = 13;
        uint8_t tag_number = 0;
        const uint32_t test_value = 0;  /* special tags require value 0 */
        uint32_t value = 0;
        int len = 0;

        memset(buf, buf_fill=0xDCU, sizeof(buf));
        buf[2] = (test_tag_number << 4) | special_tags[i];

        tag_number = ~test_tag_number;
        len = decode_tag_number_and_value(&buf[2], &tag_number, NULL);
        zassert_equal(len, 1, NULL);
        zassert_equal(tag_number, test_tag_number, NULL);

        tag_number = ~test_tag_number;
        value = ~test_value;
        len = decode_tag_number_and_value(&buf[2], &tag_number, &value);
        zassert_equal(len, 1, NULL);
        zassert_equal(tag_number, test_tag_number, NULL);
        zassert_equal(value, test_value, NULL);

        zassert_equal(buf[1], buf_fill, NULL);
        zassert_equal(buf[3], buf_fill, NULL);
    }
}

static void test_decode_tag_number_and_value__values_below5(void)
{
    int i = 0;


    for (i = 0; i < 5; ++i)
    {
        uint8_t buf[16] = { 0 };
        uint8_t buf_fill = 0;
        const uint8_t test_tag_number = 1;
        uint8_t tag_number = 0;
        const uint32_t test_value = i;
        uint32_t value = 0;
        int len = 0;

        memset(buf, buf_fill=0xDCU, sizeof(buf));
        buf[2] = (test_tag_number << 4) | (0x7U & test_value);

        tag_number = ~test_tag_number;
        len = decode_tag_number_and_value(&buf[2], &tag_number, NULL);
        zassert_equal(len, 1, NULL);
        zassert_equal(tag_number, test_tag_number, NULL);

        tag_number = ~test_tag_number;
        value = ~test_value;
        len = decode_tag_number_and_value(&buf[2], &tag_number, &value);
        zassert_equal(len, 1, NULL);
        zassert_equal(tag_number, test_tag_number, NULL);
        zassert_equal(value, test_value, NULL);

        zassert_equal(buf[1], buf_fill, NULL);
        zassert_equal(buf[3], buf_fill, NULL);
    }
}

static void test_decode_tag_number_and_value__uint8_extension(void)
{
    /* Note: values >253 require uint16 extension */
    const uint8_t test_points[] = { 0U, 4U, 5U, 252U, 253U };
    int i = 0;


    for (i = 0; i < sizeof(test_points)/sizeof(test_points[0]); ++i)
    {
        uint8_t buf[16] = { 0 };
        uint8_t buf_fill = 0;
        const uint8_t test_tag_number = 13;
        uint8_t tag_number = 0;
        const uint32_t test_value = test_points[i];
        uint32_t value = 0;
        int len = 0;


        memset(buf, buf_fill=0xDCU, sizeof(buf));
        buf[2] = (test_tag_number << 4) | type_extended_value;
        buf[3] = test_value;

        tag_number = ~test_tag_number;
        len = decode_tag_number_and_value(&buf[2], &tag_number, NULL);
        zassert_equal(len, 2, NULL);
        zassert_equal(tag_number, test_tag_number, NULL);

        tag_number = ~test_tag_number;
        value = ~test_value;
        len = decode_tag_number_and_value(&buf[2], &tag_number, &value);
        zassert_equal(len, 2, NULL);
        zassert_equal(tag_number, test_tag_number, NULL);
        zassert_equal(value, test_value, NULL);

        zassert_equal(buf[1], buf_fill, NULL);
        zassert_equal(buf[4], buf_fill, NULL);
    }
}

static void test_decode_tag_number_and_value__uint16_extension(void)
{
    /* Note: values >253 require uint16 extension */
    const uint16_t test_points[] =
    {
        0U, 4U, 5U, 252U, 253U,
        254U, 255U, 256U, 65534U, 65535U
    };
    int i = 0;

    for (i = 0; i < sizeof(test_points)/sizeof(test_points[0]); ++i)
    {
        uint8_t buf[16] = { 0 };
        uint8_t buf_fill = 0;
        const uint8_t test_tag_number = 13;
        uint8_t tag_number = 0;
        const uint32_t test_value = test_points[i];
        uint32_t value = 0;
        int len = 0;


        FFF_RESET_HISTORY();
        BACINT_FFF_FAKES_LIST(RESET_FAKE);

        memset(buf, buf_fill=0xDCU, sizeof(buf));
        buf[1] = (test_tag_number << 4) | type_extended_value;
        buf[2] = type_extended_value_as_u16;

        /* buf[3..4] delegated to decode_unsigned16() to process */
        buf[3] = (uint8_t)(test_value >> 8);
        buf[4] = (uint8_t)(test_value >> 0);

        decode_unsigned16_fake.custom_fake = decode_unsigned16_custom_fake;
        zassert_equal(decode_unsigned16_fake.call_count, 0, NULL);

        /* Verify code tracks return len, use non-real length with NULL ref */
        decode_unsigned16_fake.return_val = 2 + 13;

        tag_number = ~test_tag_number;
        len = decode_tag_number_and_value(&buf[1], &tag_number, NULL);
        zassert_equal(len, 4 + 13, NULL);
        zassert_equal(tag_number, test_tag_number, NULL);
        zassert_equal(decode_unsigned16_fake.call_count, 1, NULL);
        zassert_equal(decode_unsigned16_fake.arg0_val, &buf[3], NULL);
        zassert_not_equal(decode_unsigned16_fake.arg1_val, NULL, NULL);


        /* Verify code tracks return len use real length */
        decode_unsigned16_fake.return_val = 2;

        tag_number = ~test_tag_number;
        value = ~test_value;
        len = decode_tag_number_and_value(&buf[1], &tag_number, &value);
        zassert_equal(len, 4, NULL);
        zassert_equal(tag_number, test_tag_number, NULL);
        zassert_equal(decode_unsigned16_fake.call_count, 2, NULL);
        zassert_equal(decode_unsigned16_fake.arg0_val, &buf[3], NULL);
        zassert_not_equal(decode_unsigned16_fake.arg1_val, (void*)&value,
                          NULL);
        zassert_equal(value, test_value, NULL);

        zassert_equal(buf[0], buf_fill, NULL);
        zassert_equal(buf[5], buf_fill, NULL);
    }
}

static void test_decode_tag_number_and_value__uint32_extension(void)
{
    /* Note: values >253 require uint16 extension */
    const uint32_t test_points[] =
    {
        0U, 4U, 5U, 252U, 253U,
        254U, 255U, 256U, 65534U, 65535U,
        65536U, 65537U, 0xFFFFFFFEU, 0xFFFFFFFFU
    };
    int i = 0;


    for (i = 0; i < sizeof(test_points)/sizeof(test_points[0]); ++i)
    {
        uint8_t buf[16] = { 0 };
        uint8_t buf_fill = 0;
        const uint8_t test_tag_number = 13;
        uint8_t tag_number = 0;
        const uint32_t test_value = test_points[i];
        uint32_t value = 0;
        int len = 0;


        FFF_RESET_HISTORY();
        BACINT_FFF_FAKES_LIST(RESET_FAKE);

        memset(buf, buf_fill=0xDCU, sizeof(buf));
        buf[1] = (test_tag_number << 4) | type_extended_value;
        buf[2] = type_extended_value_as_u32;

        /* buf[3..6] delegated to decode_unsigned32() to process */
        buf[3] = (uint8_t)(test_value >>24);
        buf[4] = (uint8_t)(test_value >>16);
        buf[5] = (uint8_t)(test_value >> 8);
        buf[6] = (uint8_t)(test_value >> 0);

        decode_unsigned32_fake.custom_fake = decode_unsigned32_custom_fake;
        zassert_equal(decode_unsigned32_fake.call_count, 0, NULL);

        /* Verify code tracks return len, use non-real length with NULL ref */
        decode_unsigned32_fake.return_val = 4 + 13;

        tag_number = ~test_tag_number;
        len = decode_tag_number_and_value(&buf[1], &tag_number, NULL);
        zassert_equal(len, 6 + 13, NULL);
        zassert_equal(tag_number, test_tag_number, NULL);
        zassert_equal(decode_unsigned32_fake.call_count, 1, NULL);
        zassert_equal(decode_unsigned32_fake.arg0_val, &buf[3], NULL);
        zassert_not_equal(decode_unsigned32_fake.arg1_val, NULL, NULL);


        /* Verify code tracks return len use real length */
        decode_unsigned32_fake.return_val = 4;

        tag_number = ~test_tag_number;
        value = ~test_value;
        len = decode_tag_number_and_value(&buf[1], &tag_number, &value);
        zassert_equal(len, 6, NULL);
        zassert_equal(tag_number, test_tag_number, NULL);
        zassert_equal(decode_unsigned32_fake.call_count, 2, NULL);
        zassert_equal(decode_unsigned32_fake.arg0_val, &buf[3], NULL);
        zassert_not_equal(decode_unsigned32_fake.arg1_val,
                          (void*)&value, NULL);
        zassert_equal(value, test_value, NULL);

        zassert_equal(buf[0], buf_fill, NULL);
        zassert_equal(buf[7], buf_fill, NULL);
    }
}

static void test_bacnet_tag_number_and_value_decode__NULL_apdu(void)
{
    uint8_t test_tag_number = 0;
    uint8_t tag_number = 0;
    uint32_t test_value = 0;
    uint32_t value = 0;
    int len = 0;


    len = bacnet_tag_number_and_value_decode(NULL, 0, NULL, NULL);
    zassert_equal(len, 0, NULL);

    test_value = 12;  /* arbitrary */
    value = test_value;
    len = bacnet_tag_number_and_value_decode(NULL, 1, NULL, &value);
    zassert_equal(len, 0, NULL);
    zassert_equal(value, test_value, NULL);

    test_tag_number = 0x57U;  /* arbitrary */
    tag_number = test_tag_number;
    len = bacnet_tag_number_and_value_decode(NULL, 2, &tag_number, NULL);
    zassert_equal(len, 0, NULL);
    zassert_equal(tag_number, test_tag_number, NULL);

    test_tag_number = 0xF9U;  /* arbitrary */
    tag_number = test_tag_number;
    test_value = 0xC8U;  /* arbitrary */
    value = test_value;
    len = bacnet_tag_number_and_value_decode(NULL, 3, &tag_number, &value);
    zassert_equal(len, 0, NULL);
    zassert_equal(tag_number, test_tag_number, NULL);
    zassert_equal(value, test_value, NULL);
}

static void test_bacnet_tag_number_and_value_decode__apdu_len_zero(void)
{
    uint8_t buf[16] = { 0 };
    uint8_t buf_fill = 0;
    uint8_t test_tag_number = 0;
    uint8_t tag_number = 0;
    uint32_t test_value = 0;
    uint32_t value = 0;
    int len = 0;


    memset(buf, buf_fill=0xFDU, sizeof(buf));

    /*
     * Verify when tag_number and/or value ptrs are NULL
     */
    len = bacnet_tag_number_and_value_decode(&buf[2], 0, NULL, NULL);
    zassert_equal(len, 0, NULL);

    zassert_equal(buf[1], buf_fill, NULL);
    zassert_equal(buf[2], buf_fill, NULL);
    zassert_equal(buf[3], buf_fill, NULL);


    test_tag_number = 0x43;  /* arbitrary */
    tag_number = test_tag_number;
    memset(buf, buf_fill=0xFDU, sizeof(buf));
    len = bacnet_tag_number_and_value_decode(&buf[2], 0,
                                             &tag_number, NULL);
    zassert_equal(len, 0, NULL);
    zassert_equal(tag_number, test_tag_number, NULL);

    zassert_equal(buf[1], buf_fill, NULL);
    zassert_equal(buf[2], buf_fill, NULL);
    zassert_equal(buf[3], buf_fill, NULL);


    test_tag_number = 0x89;  /* arbitrary */
    tag_number = test_tag_number;
    value = test_value;
    memset(buf, buf_fill=0xFDU, sizeof(buf));

    len = bacnet_tag_number_and_value_decode(&buf[2], 0,
                                             &tag_number, &value);
    zassert_equal(len, 0, NULL);
    zassert_equal(tag_number, test_tag_number, NULL);
    zassert_equal(value, test_value, NULL);

    zassert_equal(buf[1], buf_fill, NULL);
    zassert_equal(buf[2], buf_fill, NULL);
    zassert_equal(buf[3], buf_fill, NULL);
}

static void test_bacnet_tag_number_and_value_decode__special_tags(void)
{
    const uint8_t special_tags[] =
    {
        type_opening_tag,
        type_closing_tag
    };
    int i = 0;


    /*
     * Verify opening_tag (reads as value 0)
     */
    for (i = 0; i < sizeof(special_tags)/sizeof(special_tags[0]); i++)
    {
        uint8_t buf[16] = { 0 };
        uint8_t buf_fill = 0;
        const uint8_t test_tag_number = 3;
        uint8_t tag_number = 0;
        const uint32_t test_value = 0;  /* special tags require value 0 */
        uint32_t value = 0;
        int len = 0;


        memset(buf, buf_fill=0xDCU, sizeof(buf));
        buf[2] = (test_tag_number << 4) | special_tags[i];

        tag_number = ~test_tag_number;
        len = bacnet_tag_number_and_value_decode(&buf[2], 1,
                                                 &tag_number, NULL);
        zassert_equal(len, 1, NULL);
        zassert_equal(tag_number, test_tag_number, NULL);

        tag_number = ~test_tag_number;
        value = ~test_value;
        len = bacnet_tag_number_and_value_decode(&buf[2], 1,
                                                 &tag_number, &value);
        zassert_equal(len, 1, NULL);
        zassert_equal(tag_number, test_tag_number, NULL);
        zassert_equal(value, test_value, NULL);

        zassert_equal(buf[1], buf_fill, NULL);
        zassert_equal(buf[3], buf_fill, NULL);
    }
}

static void test_bacnet_tag_number_and_value_decode__values_below5(void)
{
    int i = 0;


    for (i = 0; i < 5; ++i)
    {
        uint8_t buf[16] = { 0 };
        uint8_t buf_fill = 0;
        const uint8_t test_tag_number = 1;
        uint8_t tag_number = 0;
        const uint32_t test_value = i;
        uint32_t value = 0;
        int len = 0;


        memset(buf, buf_fill=0xDCU, sizeof(buf));
        buf[2] = (test_tag_number << 4) | (0x7U & test_value);

        tag_number = ~test_tag_number;
        len = bacnet_tag_number_and_value_decode(&buf[2], 1,
                                                 &tag_number, NULL);
        zassert_equal(len, 1, NULL);
        zassert_equal(tag_number, test_tag_number, NULL);

        tag_number = ~test_tag_number;
        value = ~test_value;
        len = bacnet_tag_number_and_value_decode(&buf[2], 1,
                                                 &tag_number, &value);
        zassert_equal(len, 1, NULL);
        zassert_equal(tag_number, test_tag_number, NULL);
        zassert_equal(value, test_value, NULL);

        zassert_equal(buf[1], buf_fill, NULL);
        zassert_equal(buf[3], buf_fill, NULL);
    }
}

static void test_bacnet_tag_number_and_value_decode__uint8_extension(void)
{
    /* Note: values >253 require uint16 extension */
    const uint8_t test_points[] = { 0U, 4U, 5U, 252U, 253U };
    int i = 0;


    for (i = 0; i < sizeof(test_points)/sizeof(test_points[0]); ++i)
    {
        uint8_t buf[16] = { 0 };
        uint8_t buf_fill = 0;
        const uint8_t test_tag_number = 13;
        uint8_t tag_number = 0;
        const uint32_t test_value = test_points[i];
        uint32_t value = 0;
        int len = 0;


        memset(buf, buf_fill=0xDCU, sizeof(buf));
        buf[2] = (test_tag_number << 4) | type_extended_value;
        buf[3] = test_value;

        /* NOTE: packet is truncated after tag number encoding */
        tag_number = (uint8_t)~test_tag_number;
        len = bacnet_tag_number_and_value_decode(&buf[2], 1,
                                                 &tag_number, NULL);
        zassert_equal(len, 0, NULL);
        zassert_equal(tag_number, test_tag_number, NULL);

        tag_number = ~test_tag_number;
        len = bacnet_tag_number_and_value_decode(&buf[2], 2,
                                                 &tag_number, NULL);
        zassert_equal(len, 2, NULL);
        zassert_equal(tag_number, test_tag_number, NULL);

        tag_number = ~test_tag_number;
        value = ~test_value;
        len = bacnet_tag_number_and_value_decode(&buf[2], 2,
                                                 &tag_number, &value);
        zassert_equal(len, 2, NULL);
        zassert_equal(tag_number, test_tag_number, NULL);
        zassert_equal(value, test_value, NULL);

        zassert_equal(buf[1], buf_fill, NULL);
        zassert_equal(buf[4], buf_fill, NULL);
    }
}

static void test_bacnet_tag_number_and_value_decode__uint16_extension(void)
{
    /* Note: values >253 require uint16 extension */
    const uint16_t test_points[] =
    {
        0U, 4U, 5U, 252U, 253U,
        254U, 255U, 256U, 65534U, 65535U
    };
    int i = 0;

    for (i = 0; i < sizeof(test_points)/sizeof(test_points[0]); ++i)
    {
        uint8_t buf[16] = { 0 };
        uint8_t buf_fill = 0;
        const uint8_t test_tag_number = 13;
        uint8_t tag_number = 0;
        const uint32_t test_value = test_points[i];
        uint32_t value = 0;
        int len = 0;


        FFF_RESET_HISTORY();
        BACINT_FFF_FAKES_LIST(RESET_FAKE);

        memset(buf, buf_fill=0xDCU, sizeof(buf));
        buf[1] = (test_tag_number << 4) | type_extended_value;
        buf[2] = type_extended_value_as_u16;

        /* buf[3..4] delegated to decode_unsigned16() to process */
        buf[3] = (uint8_t)(test_value >> 8);
        buf[4] = (uint8_t)(test_value >> 0);

        decode_unsigned16_fake.custom_fake = decode_unsigned16_custom_fake;
        zassert_equal(decode_unsigned16_fake.call_count, 0, NULL);

        /* Verify code tracks return len, use non-real length with NULL ref */
        decode_unsigned16_fake.return_val = 2 + 13;

        tag_number = ~test_tag_number;
        len = bacnet_tag_number_and_value_decode(&buf[1], 4 + 13,
                                                 &tag_number, NULL);
        zassert_equal(len, 4 + 13, NULL);
        zassert_equal(tag_number, test_tag_number, NULL);
        zassert_equal(decode_unsigned16_fake.call_count, 1, NULL);
        zassert_equal(decode_unsigned16_fake.arg0_val, &buf[3], NULL);
        zassert_not_equal(decode_unsigned16_fake.arg1_val, NULL, NULL);

        /* Verify code tracks return len use real length */
        decode_unsigned16_fake.return_val = 2;

        tag_number = ~test_tag_number;
        value = ~test_value;
        len = bacnet_tag_number_and_value_decode(&buf[1], 4,
                                                 &tag_number, &value);
        zassert_equal(len, 4, NULL);
        zassert_equal(tag_number, test_tag_number, NULL);
        zassert_equal(decode_unsigned16_fake.call_count, 2, NULL);
        zassert_equal(decode_unsigned16_fake.arg0_val, &buf[3], NULL);
        zassert_not_equal(decode_unsigned16_fake.arg1_val, (void*)&value,
                          NULL);
        zassert_equal(value, test_value, NULL);

        zassert_equal(buf[0], buf_fill, NULL);
        zassert_equal(buf[5], buf_fill, NULL);
    }
}

static void test_bacnet_tag_number_and_value_decode__uint32_extension(void)
{
    /* Note: values >253 require uint16 extension */
    const uint32_t test_points[] =
    {
        0U, 4U, 5U, 252U, 253U,
        254U, 255U, 256U, 65534U, 65535U,
        65536U, 65537U, 0xFFFFFFFEU, 0xFFFFFFFFU
    };
    int i = 0;

    for (i = 0; i < sizeof(test_points)/sizeof(test_points[0]); ++i)
    {
        uint8_t buf[16] = { 0 };
        uint8_t buf_fill = 0;
        const uint8_t test_tag_number = 13;
        uint8_t tag_number = 0;
        const uint32_t test_value = test_points[i];
        uint32_t value = 0;
        int len = 0;


        FFF_RESET_HISTORY();
        BACINT_FFF_FAKES_LIST(RESET_FAKE);

        memset(buf, buf_fill=0xDCU, sizeof(buf));
        buf[1] = (test_tag_number << 4) | type_extended_value;
        buf[2] = type_extended_value_as_u32;

        /* buf[3..6] delegated to decode_unsigned32() to process */
        buf[3] = (uint8_t)(test_value >>24);
        buf[4] = (uint8_t)(test_value >>16);
        buf[5] = (uint8_t)(test_value >> 8);
        buf[6] = (uint8_t)(test_value >> 0);

        decode_unsigned32_fake.custom_fake = decode_unsigned32_custom_fake;
        zassert_equal(decode_unsigned32_fake.call_count, 0, NULL);

        /* Verify error generated upon truncation, ignore returned values */
        decode_unsigned32_fake.return_val = 4 - 1;

        tag_number = ~test_tag_number;
        len = bacnet_tag_number_and_value_decode(&buf[1], 6 - 1,
                                                 &tag_number, NULL);
        zassert_equal(len, 0, NULL);
        zassert_equal(decode_unsigned32_fake.call_count, 0, NULL);

        /* Verify code tracks return length use extra data */
        decode_unsigned32_fake.return_val = 4 + 1;

        tag_number = ~test_tag_number;
        len = bacnet_tag_number_and_value_decode(&buf[1], 6 + 1,
                                                 &tag_number, NULL);
        zassert_equal(len, 6 + 1, NULL);
        zassert_equal(tag_number, test_tag_number, NULL);
        zassert_equal(decode_unsigned32_fake.call_count, 1, NULL);
        zassert_equal(decode_unsigned32_fake.arg0_val, &buf[3], NULL);
        zassert_not_equal(decode_unsigned32_fake.arg1_val, NULL, NULL);

        /* Verify code tracks return len use real length */
        decode_unsigned32_fake.return_val = 4;

        tag_number = ~test_tag_number;
        value = ~test_value;
        len = bacnet_tag_number_and_value_decode(&buf[1], 6,
                                                 &tag_number, &value);
        zassert_equal(len, 6, NULL);
        zassert_equal(tag_number, test_tag_number, NULL);
        zassert_equal(decode_unsigned32_fake.call_count, 2, NULL);
        zassert_equal(decode_unsigned32_fake.arg0_val, &buf[3], NULL);
        zassert_not_equal(decode_unsigned32_fake.arg1_val,
                          (void*)&value, NULL);
        zassert_equal(value, test_value, NULL);

        zassert_equal(buf[0], buf_fill, NULL);
        zassert_equal(buf[7], buf_fill, NULL);
    }
}

static void test_decode_is_context_open_close_tag_set(void)
{
    struct test_point {
        bool expected_is_context_tag;
        bool expected_is_opening_tag_number;
        bool expected_is_closing_tag_number;
        int expected_len;
        uint8_t tag_number;
        uint8_t apdu[2];
        uint8_t apdu_len;
        uint8_t apdu_offset;
        uint8_t buf_fill;
    };

    const struct test_point test_points[] =
    {
        { /* False - mismatched tag number */
            .expected_is_context_tag = false,
            .expected_is_opening_tag_number = false,
            .expected_is_closing_tag_number = false,
            .expected_len = 1,
            .tag_number = 1,
            .apdu =
                {
                  (0 << 4) | type_context_specific | type_opening_tag,
                },
            .apdu_len = 1,
            .apdu_offset = 2,
            .buf_fill = 0xDCU,
        },
        { /* False - not type_context_specific */
            .expected_is_context_tag = false,
            .expected_is_opening_tag_number = false,
            .expected_is_closing_tag_number = false,
            .expected_len = 1,
            .tag_number = 1,
            .apdu =
                {
                  (0 << 4) | type_opening_tag,
                },
            .apdu_len = 1,
            .apdu_offset = 2,
            .buf_fill = 0xDCU,
        },
        { /* False - requested tag requires 2-byte apdu */
            .expected_is_context_tag = false,
            .expected_is_opening_tag_number = false,
            .expected_is_closing_tag_number = false,
            .expected_len = 1,
            .tag_number = 16,
            .apdu =
                {
                  (0 << 4) | type_context_specific | type_opening_tag,
                },
            .apdu_len = 1,
            .apdu_offset = 2,
            .buf_fill = 0xDCU,
        },
        { /* True - 1 byte */
            .expected_is_context_tag = true,
            .expected_is_opening_tag_number = false,
            .expected_is_closing_tag_number = true,
            .expected_len = 1,
            .tag_number = 14,
            .apdu =
                {
                  (14 << 4) | type_context_specific | type_closing_tag,
                },
            .apdu_len = 1,
            .apdu_offset = 1,
            .buf_fill = 0xCDU,
        },
        { /* True - 2 byte */
            .expected_is_context_tag = true,
            .expected_is_opening_tag_number = true,
            .expected_is_closing_tag_number = false,
            .expected_len = 2,
            .tag_number = 15,
            .apdu =
                {
                  (0xFU << 4) | type_context_specific | type_opening_tag,
                  15,
                },
            .apdu_len = 2,
            .apdu_offset = 2,
            .buf_fill = 0xABU,
        },
        { /* False - requested tag requres 1-byte apdu */
            .expected_is_context_tag = false,
            .expected_is_opening_tag_number = false,
            .expected_is_closing_tag_number = false,
            .expected_len = 2,
            .tag_number = 12,
            .apdu =
                {
                  (0xFU << 4) | type_context_specific | type_closing_tag,
                  0xFFU,
                },
            .apdu_len = 2,
            .apdu_offset = 1,
            .buf_fill = 0xBAU,
        },
        { /* False - requested tag requres 1-byte apdu */
            .expected_is_context_tag = true,
            .expected_is_opening_tag_number = true,
            .expected_is_closing_tag_number = false,
            .expected_len = 1,
            .tag_number = 0,
            .apdu =
                {
                  (0 << 4) | type_context_specific | type_opening_tag,
                  (0 << 4) | type_context_specific | type_opening_tag,
                },
            .apdu_len = 2,
            .apdu_offset = 1,
            .buf_fill = 0xBAU,
        },
    };
    int i = 0;


    for (i = 0; i < sizeof(test_points)/sizeof(test_points[0]); ++i)
    {
        const struct test_point * const tp = &test_points[i];
        const int base_offset = tp->apdu_offset;
        int tag_length = 0;
        bool is_context_tag = false;
        uint8_t buf[16] = { 0 };


        memset(buf, tp->buf_fill, sizeof(buf));
        memcpy(&buf[base_offset], tp->apdu,
                MIN(sizeof(tp->apdu), tp->apdu_len));

        is_context_tag = decode_is_context_tag(&buf[base_offset],
                                               tp->tag_number);
        zassert_equal(is_context_tag, tp->expected_is_context_tag, NULL);

        zassert_equal(buf[base_offset - 1], tp->buf_fill, NULL);
        zassert_equal(buf[base_offset + tp->apdu_len], tp->buf_fill, NULL);

        tag_length = -1;
        is_context_tag = decode_is_context_tag_with_length(
                            &buf[base_offset],
                            tp->tag_number,
                            &tag_length);
        zassert_equal(is_context_tag, tp->expected_is_context_tag, NULL);
        zassert_equal(tag_length, tp->expected_len, NULL);

        zassert_equal(decode_is_opening_tag_number(&buf[base_offset],
                                                   tp->tag_number),
                      tp->expected_is_opening_tag_number, NULL);

        zassert_equal(decode_is_closing_tag_number(&buf[base_offset],
                                                   tp->tag_number),
                      tp->expected_is_closing_tag_number, NULL);

        zassert_equal(buf[base_offset - 1], tp->buf_fill, NULL);
        zassert_equal(buf[base_offset + tp->apdu_len], tp->buf_fill, NULL);
    }

    /*
     * Verify when NULL apdu provided
     */
    zassert_false(decode_is_context_tag(NULL, 0), NULL);
    zassert_false(decode_is_context_tag_with_length(NULL, 0, NULL), NULL);
    zassert_false(decode_is_opening_tag_number(NULL, 0), NULL);
    zassert_false(decode_is_closing_tag_number(NULL, 0), NULL);
}

static void test_encode_application_boolean(void)
{
    struct test_point {
        bool boolean_value;
        int expected_len;
        uint8_t expected_apdu[2];
        uint8_t apdu_offset;
        uint8_t buf_fill;
    };

    const struct test_point test_points[] =
    {
        {
            .boolean_value = false,
            .expected_len = 1,
            .expected_apdu =
                {
                  ((bacnet_application_tag_boolean << 4) |
                    0x00U),
                },
            .apdu_offset = 2,
            .buf_fill = 0xDCU,
        },
        {
            .boolean_value = true,
            .expected_len = 1,
            .expected_apdu =
                {
                  ((bacnet_application_tag_boolean << 4) |
                    0x01U),
                },
            .apdu_offset = 1,
            .buf_fill = 0xDCU,
        },
    };
    int i = 0;


    for (i = 0; i < sizeof(test_points)/sizeof(test_points[0]); ++i)
    {
        const struct test_point * const tp = &test_points[i];
        const int base_offset = tp->apdu_offset;
        int len = 0;
        uint8_t buf[8] = { 0 };


        memset(buf, tp->buf_fill, sizeof(buf));

        len = encode_application_boolean(&buf[base_offset], tp->boolean_value);
        zassert_equal(len, tp->expected_len, NULL);
        zassert_mem_equal(&buf[base_offset], tp->expected_apdu,
                          tp->expected_len, NULL);

        zassert_equal(buf[base_offset - 1], tp->buf_fill, NULL);
        zassert_equal(buf[base_offset + tp->expected_len], tp->buf_fill, NULL);
    }

    zassert_equal(encode_application_boolean(NULL, true), 0, NULL);
    zassert_equal(encode_application_boolean(NULL, false), 0, NULL);
}

static void test_encode_context_boolean(void)
{
    struct test_point {
        bool boolean_value;
        uint8_t tag_number;
        int expected_len;
        uint8_t expected_apdu[4];
        uint8_t apdu_offset;
        uint8_t buf_fill;
    };

    const struct test_point test_points[] =
    {
        {
            .boolean_value = false,
            .tag_number = 0,
            .expected_len = 2,
            .expected_apdu =
                {
                  ((0 << 4) |
                    type_context_specific |
                    0x01U),
                  0,
                },
            .apdu_offset = 2,
            .buf_fill = 0xDCU,
        },
        {
            .boolean_value = true,
            .tag_number = 13,
            .expected_len = 2,
            .expected_apdu =
                {
                  ((13 << 4) |
                    type_context_specific |
                    0x01U),
                  1,
                },
            .apdu_offset = 2,
            .buf_fill = 0xDCU,
        },
        {
            .boolean_value = true,
            .tag_number = 16,
            .expected_len = 3,
            .expected_apdu =
                {
                  ((0xFU << 4) |
                    type_context_specific |
                    0x01U),
                  16,
                  1

                },
            .apdu_offset = 2,
            .buf_fill = 0xDCU,
        },
    };
    int i = 0;


    for (i = 0; i < sizeof(test_points)/sizeof(test_points[0]); ++i)
    {
        const struct test_point * const tp = &test_points[i];
        const int base_offset = tp->apdu_offset;
        int len = 0;
        uint8_t buf[8] = { 0 };


        memset(buf, tp->buf_fill, sizeof(buf));

        /* Verify the expected length is reported with NULL apdu */
        len = encode_context_boolean(NULL,
                                     tp->tag_number, tp->boolean_value);
        zassert_equal(len, tp->expected_len, NULL);

        /* Verify the expected length is reported with real apdu */
        len = encode_context_boolean(&buf[base_offset],
                                     tp->tag_number, tp->boolean_value);
        zassert_equal(len, tp->expected_len, NULL);
        zassert_mem_equal(&buf[base_offset], tp->expected_apdu,
                          tp->expected_len, NULL);

        zassert_equal(buf[base_offset - 1], tp->buf_fill, NULL);
        zassert_equal(buf[base_offset + tp->expected_len], tp->buf_fill, NULL);
    }
}

static void test_decode_context_boolean(void)
{
    const uint8_t test_points[] = { 0, 1, 0xFE, 0xFF };
    int i = 0;


    for (i = 0; i < sizeof(test_points)/sizeof(test_points[0]); ++i)
    {
        uint8_t buf[4] = { 0 };
        uint8_t buf_fill = 0xFD;


        memset(buf, buf_fill, sizeof(buf));
        buf[1] = test_points[i];
        zassert_equal(decode_context_boolean(&buf[1]),
                      (test_points[i] != 0), NULL);
        zassert_equal(buf[0], buf_fill, NULL);
        zassert_equal(buf[2], buf_fill, NULL);
    }
}

static void test_decode_context_boolean2(void)
{
    struct test_point {
        bool expected_boolean_value;
        int expected_len;
        uint8_t tag_number;
        uint8_t apdu[4];
        uint8_t apdu_len;
        uint8_t apdu_offset;
        uint8_t buf_fill;
    };

    const struct test_point test_points[] =
    {
        {
            .expected_boolean_value = false,
            .expected_len = -1, /* BACNET_STATUS_ERROR */
            .tag_number = 0,
            .apdu =
                {
                  ((0 << 4) |
                    type_context_specific |
                    0x01U),
                  0,
                },
            .apdu_offset = 2,
            .buf_fill = 0xDCU,
        },
        {
            .expected_boolean_value = true,
            .expected_len = 2,
            .tag_number = 13,
            .apdu =
                {
                  ((13 << 4) |
                    type_context_specific |
                    0x01U),
                  1,
                },
            .apdu_offset = 2,
            .buf_fill = 0xDCU,
        },
        {
            .expected_boolean_value = false,
            .expected_len = -1,
            .tag_number = 16,
            .apdu =
                {
                  ((0xFU << 4) |
                    type_context_specific |
                    0x01U),
                  16,
                  1
                },
            .apdu_offset = 2,
            .buf_fill = 0xDCU,
        },
    };
    bool boolean_value = false;
    uint8_t buf[8] = { 0 };
    int i = 0;


    for (i = 0; i < sizeof(test_points)/sizeof(test_points[0]); ++i)
    {
        const struct test_point * const tp = &test_points[i];
        const int base_offset = tp->apdu_offset;
        int len = 0;


        memset(buf, tp->buf_fill, sizeof(buf));
        memcpy(&buf[base_offset], tp->apdu,
                MIN(sizeof(tp->apdu), tp->apdu_len));

        boolean_value = false;
        len = decode_context_boolean2(&buf[base_offset],
                                      tp->tag_number,
                                      &boolean_value);
        zassert_equal(len, tp->expected_len, NULL);
        zassert_equal(boolean_value, tp->expected_boolean_value, NULL);

        zassert_equal(buf[base_offset - 1], tp->buf_fill, NULL);
        zassert_equal(buf[base_offset + tp->expected_len], tp->buf_fill, NULL);
    }


    /* Verify error reported when NULL boolean_value reference provided */
    memset(buf, test_points[0].buf_fill, sizeof(buf));
    memcpy(buf, test_points[0].apdu,
                MIN(sizeof(test_points[0].apdu), test_points[0].apdu_len));
    zassert_equal(decode_context_boolean2(buf, test_points[0].tag_number, NULL),
                  -1, NULL);
    /* Verify error reported when NULL boolean_value reference provided */
    zassert_equal(decode_context_boolean2(NULL, 0, &boolean_value), -1, NULL);
}

static void test_decode_boolean(void)
{
    const uint32_t test_points[] = { 0, 1, 0xFE, 0xFF, UINT_MAX };
    int i = 0;


    for (i = 0; i < sizeof(test_points)/sizeof(test_points[0]); ++i)
    {
        zassert_equal(decode_boolean(test_points[i]),
                      (test_points[i] != 0), NULL);
    }
}

static void test_encode_application_null(void)
{
    const uint8_t encoded_application_null = 0x00U;
    const uint8_t buf_fill = 0xFFU;
    uint8_t buf[4] = { 0 };

    zassert_equal(encode_application_null(NULL), 1, NULL);

    memset(buf, buf_fill, sizeof(buf));
    zassert_equal(encode_application_null(&buf[1]), 1, NULL);
    zassert_equal(buf[1], encoded_application_null, NULL);
    zassert_equal(buf[0], buf_fill, NULL);
    zassert_equal(buf[2], buf_fill, NULL);

    /* Verify it can handle an odd address */
    memset(buf, buf_fill, sizeof(buf));
    zassert_equal(encode_application_null(&buf[2]), 1, NULL);
    zassert_equal(buf[2], encoded_application_null, NULL);
    zassert_equal(buf[1], buf_fill, NULL);
    zassert_equal(buf[3], buf_fill, NULL);
}

static void test_encode_context_null(void)
{
    struct test_point {
        int expected_len;
        uint8_t expected_apdu[4];
        uint8_t tag_number;
        uint8_t apdu_len;
        uint8_t apdu_offset;
        uint8_t buf_fill;
    };

    const struct test_point test_points[] =
    {
        {
            .expected_len = 1,
            .tag_number = 0,
            .expected_apdu =
                {
                  ((0 << 4) |
                    type_context_specific),
                  0,
                },
            .apdu_offset = 2,
            .buf_fill = 0xDCU,
        },
        {
            .expected_len = 1,
            .tag_number = 1,
            .expected_apdu =
                {
                  ((1 << 4) |
                    type_context_specific),
                },
            .apdu_offset = 2,
            .buf_fill = 0xDCU,
        },
        {
            .expected_len = 1,
            .tag_number = 4,
            .expected_apdu =
                {
                  ((4 << 4) |
                    type_context_specific),
                },
            .apdu_offset = 2,
            .buf_fill = 0xDCU,
        },
        {
            .expected_len = 1,
            .tag_number = 5,
            .expected_apdu =
                {
                  ((5 << 4) |
                    type_context_specific),
                },
            .apdu_offset = 2,
            .buf_fill = 0xDCU,
        },
        {
            .expected_len = 2,
            .tag_number = 0xFFU,
            .expected_apdu =
                {
                  ((0xFU << 4) |
                    type_context_specific),
                  0xFFU,
                },
            .apdu_offset = 1,
            .buf_fill = 0xDCU,
        },
    };
    uint8_t buf[8] = { 0 };
    int i = 0;


    for (i = 0; i < sizeof(test_points)/sizeof(test_points[0]); ++i)
    {
        const struct test_point * const tp = &test_points[i];
        const int base_offset = tp->apdu_offset;
        int len = 0;


        len = encode_context_null(NULL, tp->tag_number);
        zassert_equal(len, tp->expected_len, NULL);

        memset(buf, tp->buf_fill, sizeof(buf));

        len = encode_context_null(&buf[base_offset], tp->tag_number);
        zassert_equal(len, tp->expected_len, NULL);
        zassert_mem_equal(&buf[base_offset], tp->expected_apdu,
                          tp->expected_len, NULL);

        zassert_equal(buf[base_offset - 1], tp->buf_fill, NULL);
        zassert_equal(buf[base_offset + tp->expected_len], tp->buf_fill, NULL);
    }
}

static void test_byte_reverse_bits(void)
{
    struct test_point {
        uint8_t in_byte;
        uint8_t expected_out_byte;
    };
    const struct test_point test_points[] =
    {
        { .in_byte = 0x00, .expected_out_byte = 0x00, },
        { .in_byte = 0x01, .expected_out_byte = 0x80, },
        { .in_byte = 0x02, .expected_out_byte = 0x40, },
        { .in_byte = 0x04, .expected_out_byte = 0x20, },
        { .in_byte = 0x08, .expected_out_byte = 0x10, },
        { .in_byte = 0x10, .expected_out_byte = 0x08, },
        { .in_byte = 0x20, .expected_out_byte = 0x04, },
        { .in_byte = 0x40, .expected_out_byte = 0x02, },
        { .in_byte = 0x80, .expected_out_byte = 0x01, },
        { .in_byte = 0xB5, .expected_out_byte = 0xAD, },
        { .in_byte = 0xFF, .expected_out_byte = 0xFF, },
    };
    int i = 0;


    for (i = 0; i < sizeof(test_points)/sizeof(test_points[0]); ++i)
    {
        const struct test_point * const tp = &test_points[i];
        zassert_equal(byte_reverse_bits(tp->in_byte),
                      tp->expected_out_byte, NULL);
    }
}

/* REVISIT: Provide missing tests */

static void test_bacnet_unsigned_decode(void)
{
    struct test_point {
        BACNET_UNSIGNED_INTEGER expected_bacint_value;
        uint32_t expected_len_value;
        uint8_t apdu[16];
        uint16_t apdu_len;
        uint8_t apdu_offset;
        uint8_t buf_fill;
    };
    const struct test_point test_points[] =
    {
        {
            .expected_len_value = 0,
        },
    };
    BACNET_UNSIGNED_INTEGER bacint_value = { 0 };
    uint8_t buf[20] = { 0 };
    int len = 0;
    int i = 0;

    for (i = 0; i < sizeof(test_points)/sizeof(test_points[0]); ++i)
    {
        const struct test_point * const tp = &test_points[i];
        const int base_offset = tp->apdu_offset;


        FFF_RESET_HISTORY();
        BACINT_FFF_FAKES_LIST(RESET_FAKE);

        decode_unsigned16_fake.return_val = 2;
        decode_unsigned24_fake.return_val = 3;
        decode_unsigned32_fake.return_val = 4;
        decode_unsigned40_fake.return_val = 5;
        decode_unsigned48_fake.return_val = 6;
        decode_unsigned56_fake.return_val = 7;
        decode_unsigned64_fake.return_val = 8;


        memset(buf, tp->buf_fill, sizeof(buf));
        memcpy(&buf[base_offset], tp->apdu,
                MIN(sizeof(tp->apdu), tp->apdu_len));

        /*
         * Verify behavior with NULL bacint_value reference
         */
        len = bacnet_unsigned_decode(&buf[base_offset], tp->apdu_len,
                                     tp->expected_len_value, NULL);
        zassert_equal(len, 0, NULL);
        zassert_equal(buf[base_offset - 1], tp->buf_fill, NULL);
        zassert_equal(buf[base_offset + tp->apdu_len], tp->buf_fill, NULL);

        /* Verify no mocks were called */
        zassert_equal(decode_unsigned16_fake.call_count, 0, NULL);
        zassert_equal(decode_unsigned24_fake.call_count, 0, NULL);
        zassert_equal(decode_unsigned32_fake.call_count, 0, NULL);
        zassert_equal(decode_unsigned40_fake.call_count, 0, NULL);
        zassert_equal(decode_unsigned48_fake.call_count, 0, NULL);
        zassert_equal(decode_unsigned56_fake.call_count, 0, NULL);
        zassert_equal(decode_unsigned64_fake.call_count, 0, NULL);


        /*
         * Verify behavior with real bacint_value reference
         */
        memset(&bacint_value, 0xFFU, sizeof(bacint_value));
        len = bacnet_unsigned_decode(&buf[base_offset], tp->apdu_len,
                                     tp->expected_len_value, &bacint_value);
        zassert_equal(buf[base_offset - 1], tp->buf_fill, NULL);
        zassert_equal(buf[base_offset + tp->apdu_len], tp->buf_fill, NULL);

        /*
         * Verify len and bacint_value.
         *
         * NOTE: zassert_equal() only takes 'int' as a parameter,
         *       so comparison of variables longer than that
         *       must be done with an explicit '==' or '!='.
         */
        if (tp->expected_len_value == 1)
        {
            zassert_equal(len, tp->expected_len_value, NULL);
            zassert_true(bacint_value == tp->expected_bacint_value, NULL);
        }
        else if (2 <= tp->expected_len_value && tp->expected_len_value <= 4)
        {
            zassert_equal(len, tp->expected_len_value, NULL);
            zassert_true(bacint_value == tp->expected_bacint_value, NULL);
        }
#ifdef UINT64_MAX
        else if (5 <= tp->expected_len_value && tp->expected_len_value <= 8)
        {
            zassert_equal(len, tp->expected_len_value, NULL);
            zassert_true(bacint_value == tp->expected_bacint_value, NULL);
        }
#endif /* defined(UINT64_MAX) */
        else
        {
            zassert_equal(len, 0, NULL);
            zassert_true(bacint_value == 0, NULL);
        }

        /* Verify only expected mock was called */
        if (tp->expected_len_value == 2)
        {
            zassert_equal(decode_unsigned16_fake.call_count, 1, NULL);
            zassert_equal(decode_unsigned16_fake.arg0_val,
                            &buf[base_offset], NULL);
            zassert_not_equal(decode_unsigned16_fake.arg1_val,
                              (void*)&bacint_value, NULL);
        }
        else
        {
            zassert_equal(decode_unsigned16_fake.call_count, 0, NULL);
        }

        if (tp->expected_len_value == 3)
        {
            zassert_equal(decode_unsigned24_fake.call_count, 1, NULL);
            zassert_equal(decode_unsigned24_fake.arg0_val,
                            &buf[base_offset], NULL);
            zassert_not_equal(decode_unsigned24_fake.arg1_val,
                              (void*)&bacint_value, NULL);
        }
        else
        {
            zassert_equal(decode_unsigned24_fake.call_count, 0, NULL);
        }

#ifdef UINT64_MAX
        if (tp->expected_len_value == 4)
        {
            zassert_equal(decode_unsigned32_fake.call_count, 1, NULL);
            zassert_equal(decode_unsigned32_fake.arg0_val,
                            &buf[base_offset], NULL);
            zassert_not_equal(decode_unsigned32_fake.arg1_val,
                              (void*)&bacint_value, NULL);
        }
        else
        {
            zassert_equal(decode_unsigned32_fake.call_count, 0, NULL);
        }

        if (tp->expected_len_value == 5)
        {
            zassert_equal(decode_unsigned40_fake.call_count, 1, NULL);
            zassert_equal(decode_unsigned40_fake.arg0_val,
                            &buf[base_offset], NULL);
            zassert_not_equal(decode_unsigned40_fake.arg1_val,
                              (void*)&bacint_value, NULL);
        }
        else
        {
            zassert_equal(decode_unsigned40_fake.call_count, 0, NULL);
        }

        if (tp->expected_len_value == 6)
        {
            zassert_equal(decode_unsigned48_fake.call_count, 1, NULL);
            zassert_equal(decode_unsigned48_fake.arg0_val,
                            &buf[base_offset], NULL);
            zassert_not_equal(decode_unsigned48_fake.arg1_val,
                              (void*)&bacint_value, NULL);
        }
        else
        {
            zassert_equal(decode_unsigned48_fake.call_count, 0, NULL);
        }

        if (tp->expected_len_value == 7)
        {
            zassert_equal(decode_unsigned56_fake.call_count, 1, NULL);
            zassert_equal(decode_unsigned56_fake.arg0_val,
                            &buf[base_offset], NULL);
            zassert_not_equal(decode_unsigned56_fake.arg1_val,
                              (void*)&bacint_value, NULL);
        }
        else
        {
            zassert_equal(decode_unsigned56_fake.call_count, 0, NULL);
        }

        if (tp->expected_len_value == 8)
        {
            zassert_equal(decode_unsigned64_fake.call_count, 1, NULL);
            zassert_equal(decode_unsigned64_fake.arg0_val,
                            &buf[base_offset], NULL);
            zassert_not_equal(decode_unsigned64_fake.arg1_val,
                              (void*)&bacint_value, NULL);
        }
        else
        {
            zassert_equal(decode_unsigned64_fake.call_count, 0, NULL);
        }
#endif /* defined(UINT64_MAX) */
    }

    /* Verify bacint_value not written on NULL apdu */
    bacint_value = ~(test_points[0].expected_bacint_value);
    len = bacnet_unsigned_decode(NULL, sizeof(buf), 1, &bacint_value);
    zassert_equal(len, 0, NULL);
    zassert_true(bacint_value == ~(test_points[0].expected_bacint_value), NULL);

    bacint_value = test_points[0].expected_bacint_value + 1;
    len = bacnet_unsigned_decode(NULL, sizeof(buf), 1, &bacint_value);
    zassert_equal(len, 0, NULL);
    zassert_true(bacint_value == test_points[0].expected_bacint_value + 1, NULL);

    /* Verify return values of mocked functions are used */
    decode_unsigned16_fake.return_val = 2 - 1;
    zassert_equal(bacnet_unsigned_decode(buf, sizeof(buf), 2, &bacint_value),
                  decode_unsigned16_fake.return_val, NULL);

    decode_unsigned24_fake.return_val = 3 - 1;
    zassert_equal(bacnet_unsigned_decode(buf, sizeof(buf), 3, &bacint_value),
                  decode_unsigned24_fake.return_val, NULL);

    decode_unsigned32_fake.return_val = 4 - 1;
    zassert_equal(bacnet_unsigned_decode(buf, sizeof(buf), 4, &bacint_value),
                  decode_unsigned32_fake.return_val, NULL);

#ifdef UINT64_MAX
    decode_unsigned40_fake.return_val = 5 - 1;
    zassert_equal(bacnet_unsigned_decode(buf, sizeof(buf), 5, &bacint_value),
                  decode_unsigned40_fake.return_val, NULL);

    decode_unsigned48_fake.return_val = 6 - 1;
    zassert_equal(bacnet_unsigned_decode(buf, sizeof(buf), 6, &bacint_value),
                  decode_unsigned48_fake.return_val, NULL);

    decode_unsigned56_fake.return_val = 7 - 1;
    zassert_equal(bacnet_unsigned_decode(buf, sizeof(buf), 7, &bacint_value),
                  decode_unsigned56_fake.return_val, NULL);

    decode_unsigned64_fake.return_val = 8 - 1;
    zassert_equal(bacnet_unsigned_decode(buf, sizeof(buf), 8, &bacint_value),
                  decode_unsigned64_fake.return_val, NULL);
#endif /* defined(UINT64_MAX) */
}

static void test_encode_bacnet_unsigned(void)
{
    struct test_point {
        BACNET_UNSIGNED_INTEGER bacint_value;
        uint32_t expected_len;
        uint8_t expected_apdu[16];
        uint16_t expected_apdu_len;
        uint8_t apdu_offset;
        uint8_t buf_fill;
    };
    const struct test_point test_points[] =
    {
        {
            .bacint_value = 0x00U,
            .expected_len = 1,
            .expected_apdu = { 0x00U, },
            .expected_apdu_len = 1,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .bacint_value = 0x01U,
            .expected_len = 1,
            .expected_apdu = { 0x01U, },
            .expected_apdu_len = 1,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .bacint_value = 0x80U,
            .expected_len = 1,
            .expected_apdu = { 0x80U, },
            .expected_apdu_len = 1,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .bacint_value = 0xFFU,
            .expected_len = 1,
            .expected_apdu = { 0xFFU, },
            .expected_apdu_len = 1,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .bacint_value = 0x0100U,
            .expected_len = 2,
            .expected_apdu = { 0x01U, 0x00U, },
            .expected_apdu_len = 2,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .bacint_value = 0x0101U,
            .expected_len = 2,
            .expected_apdu = { 0x01U, 0x01U, },
            .expected_apdu_len = 2,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .bacint_value = 0x8000U,
            .expected_len = 2,
            .expected_apdu = { 0x80U, 0x00U, },
            .expected_apdu_len = 2,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .bacint_value = 0xFFFFU,
            .expected_len = 2,
            .expected_apdu = { 0xFFU, 0xFFU, },
            .expected_apdu_len = 2,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .bacint_value = 0x010000UL,
            .expected_len = 3,
            .expected_apdu = { 0x01U, 0x00U, 0x00U, },
            .expected_apdu_len = 3,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .bacint_value = 0x010001UL,
            .expected_len = 3,
            .expected_apdu = { 0x01U, 0x00U, 0x01U, },
            .expected_apdu_len = 3,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .bacint_value = 0x800000UL,
            .expected_len = 3,
            .expected_apdu = { 0x80U, 0x00U, 0x00U, },
            .expected_apdu_len = 3,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .bacint_value = 0xFFFFFFUL,
            .expected_len = 3,
            .expected_apdu = { 0xFFU, 0xFFU, 0xFFU, },
            .expected_apdu_len = 3,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .bacint_value = 0x01000000UL,
            .expected_len = 4,
            .expected_apdu = { 0x01U, 0x00U, 0x00U, 0x00U, },
            .expected_apdu_len = 4,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .bacint_value = 0x01000001UL,
            .expected_len = 4,
            .expected_apdu = { 0x01U, 0x00U, 0x00U, 0x01U, },
            .expected_apdu_len = 4,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .bacint_value = 0x80000000UL,
            .expected_len = 4,
            .expected_apdu = { 0x80U, 0x00U, 0x00U, 0x00U, },
            .expected_apdu_len = 4,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .bacint_value = 0xFFFFFFFFUL,
            .expected_len = 4,
            .expected_apdu = { 0xFFU, 0xFFU, 0xFFU, 0xFFU, },
            .expected_apdu_len = 4,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .bacint_value = 0x0100000000ULL,
            .expected_len = 5,
            .expected_apdu = { 0x01U, 0x00U, 0x00U, 0x00U,
                               0x00U, },
            .expected_apdu_len = 5,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .bacint_value = 0x0100000001ULL,
            .expected_len = 5,
            .expected_apdu = { 0x01U, 0x00U, 0x00U, 0x00U,
                               0x01U, },
            .expected_apdu_len = 5,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .bacint_value = 0x8000000000ULL,
            .expected_len = 5,
            .expected_apdu = { 0x80U, 0x00U, 0x00U, 0x00U,
                               0x00U, },
            .expected_apdu_len = 5,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .bacint_value = 0xFFFFFFFFFFULL,
            .expected_len = 5,
            .expected_apdu = { 0xFFU, 0xFFU, 0xFFU, 0xFFU,
                               0xFFU, },
            .expected_apdu_len = 5,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .bacint_value = 0x010000000000ULL,
            .expected_len = 6,
            .expected_apdu = { 0x01U, 0x00U, 0x00U, 0x00U,
                               0x00U, 0x00U, },
            .expected_apdu_len = 6,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .bacint_value = 0x010000000001ULL,
            .expected_len = 6,
            .expected_apdu = { 0x01U, 0x00U, 0x00U, 0x00U,
                               0x00U, 0x01U, },
            .expected_apdu_len = 6,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .bacint_value = 0x800000000000ULL,
            .expected_len = 6,
            .expected_apdu = { 0x80U, 0x00U, 0x00U, 0x00U,
                               0x00U, 0x00U, },
            .expected_apdu_len = 6,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .bacint_value = 0xFFFFFFFFFFFFULL,
            .expected_len = 6,
            .expected_apdu = { 0xFFU, 0xFFU, 0xFFU, 0xFFU,
                               0xFFU, 0xFFU, },
            .expected_apdu_len = 6,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .bacint_value = 0x01000000000000ULL,
            .expected_len = 7,
            .expected_apdu = { 0x01U, 0x00U, 0x00U, 0x00U,
                               0x00U, 0x00U, 0x00U, },
            .expected_apdu_len = 7,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .bacint_value = 0x01000000000001ULL,
            .expected_len = 7,
            .expected_apdu = { 0x01U, 0x00U, 0x00U, 0x00U,
                               0x00U, 0x00U, 0x01U, },
            .expected_apdu_len = 7,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .bacint_value = 0x80000000000000ULL,
            .expected_len = 7,
            .expected_apdu = { 0x80U, 0x00U, 0x00U, 0x00U,
                               0x00U, 0x00U, 0x00U, },
            .expected_apdu_len = 7,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .bacint_value = 0xFFFFFFFFFFFFFFULL,
            .expected_len = 7,
            .expected_apdu = { 0xFFU, 0xFFU, 0xFFU, 0xFFU,
                               0xFFU, 0xFFU, 0xFFU, },
            .expected_apdu_len = 7,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .bacint_value = 0x0100000000000000ULL,
            .expected_len = 8,
            .expected_apdu = { 0x01U, 0x00U, 0x00U, 0x00U,
                               0x00U, 0x00U, 0x00U, 0x00U, },
            .expected_apdu_len = 8,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .bacint_value = 0x0100000000000001ULL,
            .expected_len = 8,
            .expected_apdu = { 0x01U, 0x00U, 0x00U, 0x00U,
                               0x00U, 0x00U, 0x00U, 0x01U, },
            .expected_apdu_len = 8,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .bacint_value = 0x8000000000000000ULL,
            .expected_len = 8,
            .expected_apdu = { 0x80U, 0x00U, 0x00U, 0x00U,
                               0x00U, 0x00U, 0x00U, 0x00U, },
            .expected_apdu_len = 8,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .bacint_value = 0xFFFFFFFFFFFFFFFFULL,
            .expected_len = 8,
            .expected_apdu = { 0xFFU, 0xFFU, 0xFFU, 0xFFU,
                               0xFFU, 0xFFU, 0xFFU, 0xFFU, },
            .expected_apdu_len = 8,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
    };
    uint8_t buf[20] = { 0 };
    int len = 0;
    int i = 0;

    for (i = 0; i < sizeof(test_points)/sizeof(test_points[0]); ++i)
    {
        const struct test_point * const tp = &test_points[i];
        const int base_offset = tp->apdu_offset;


        FFF_RESET_HISTORY();
        BACINT_FFF_FAKES_LIST(RESET_FAKE);

        bacnet_unsigned_length_fake.custom_fake = 
            bacnet_unsigned_length_custom_fake;

        encode_unsigned16_fake.custom_fake = encode_unsigned16_custom_fake;
        encode_unsigned16_fake.return_val = 2;
        encode_unsigned24_fake.custom_fake = encode_unsigned24_custom_fake;
        encode_unsigned24_fake.return_val = 3;
        encode_unsigned32_fake.custom_fake = encode_unsigned32_custom_fake;
        encode_unsigned32_fake.return_val = 4;
        encode_unsigned40_fake.custom_fake = encode_unsigned40_custom_fake;
        encode_unsigned40_fake.return_val = 5;
        encode_unsigned48_fake.custom_fake = encode_unsigned48_custom_fake;
        encode_unsigned48_fake.return_val = 6;
        encode_unsigned56_fake.custom_fake = encode_unsigned56_custom_fake;
        encode_unsigned56_fake.return_val = 7;
        encode_unsigned64_fake.custom_fake = encode_unsigned64_custom_fake;
        encode_unsigned64_fake.return_val = 8;

        len = encode_bacnet_unsigned(NULL, tp->bacint_value);
        zassert_equal(len, tp->expected_len, NULL);

        zassert_equal(bacnet_unsigned_length_fake.call_count, 1, NULL);
        zassert_equal(encode_unsigned16_fake.call_count, 0, NULL);
        zassert_equal(encode_unsigned24_fake.call_count, 0, NULL);
        zassert_equal(encode_unsigned32_fake.call_count, 0, NULL);
        zassert_equal(encode_unsigned40_fake.call_count, 0, NULL);
        zassert_equal(encode_unsigned48_fake.call_count, 0, NULL);
        zassert_equal(encode_unsigned56_fake.call_count, 0, NULL);
        zassert_equal(encode_unsigned64_fake.call_count, 0, NULL);


        memset(buf, tp->buf_fill, sizeof(buf));

        len = encode_bacnet_unsigned(&buf[base_offset], tp->bacint_value);

        zassert_equal(buf[base_offset - 1], tp->buf_fill, NULL);
        zassert_equal(buf[base_offset + tp->expected_len], tp->buf_fill, NULL);

        zassert_equal(len, tp->expected_len, NULL);
        if (tp->expected_len == 1)
        {
            zassert_equal(buf[base_offset], (uint8_t)tp->bacint_value, NULL);
            zassert_mem_equal(&buf[base_offset], tp->expected_apdu,
                              tp->expected_len, NULL);
        }

        if (tp->expected_len == 2)
        {
            zassert_equal(encode_unsigned16_fake.call_count, 1, NULL);
            zassert_equal(encode_unsigned16_fake.arg0_val,
                          &buf[base_offset], NULL);
            zassert_true(encode_unsigned16_fake.arg1_val == tp->bacint_value,
                         NULL);
            zassert_equal(tp->expected_len, tp->expected_apdu_len, NULL);
            zassert_mem_equal(&buf[base_offset], tp->expected_apdu,
                              tp->expected_len, NULL);
        }
        else
        {
            zassert_equal(encode_unsigned16_fake.call_count, 0, NULL);
        }

        if (tp->expected_len == 3)
        {
            zassert_equal(encode_unsigned24_fake.call_count, 1, NULL);
            zassert_equal(encode_unsigned24_fake.arg0_val,
                          &buf[base_offset], NULL);
            zassert_true(encode_unsigned24_fake.arg1_val == tp->bacint_value,
                         NULL);
            zassert_equal(tp->expected_len, tp->expected_apdu_len, NULL);
            zassert_mem_equal(&buf[base_offset], tp->expected_apdu,
                              tp->expected_len, NULL);
        }
        else
        {
            zassert_equal(encode_unsigned24_fake.call_count, 0, NULL);
        }

        if (tp->expected_len == 4)
        {
            zassert_equal(encode_unsigned32_fake.call_count, 1, NULL);
            zassert_equal(encode_unsigned32_fake.arg0_val,
                          &buf[base_offset], NULL);
            zassert_true(encode_unsigned32_fake.arg1_val == tp->bacint_value,
                         NULL);
            zassert_equal(tp->expected_len, tp->expected_apdu_len, NULL);
            zassert_mem_equal(&buf[base_offset], tp->expected_apdu,
                              tp->expected_len, NULL);
        }
        else
        {
            zassert_equal(encode_unsigned32_fake.call_count, 0, NULL);
        }

        if (tp->expected_len == 5)
        {
            zassert_equal(encode_unsigned40_fake.call_count, 1, NULL);
            zassert_equal(encode_unsigned40_fake.arg0_val,
                          &buf[base_offset], NULL);
            zassert_true(encode_unsigned40_fake.arg1_val == tp->bacint_value,
                         NULL);
            zassert_equal(tp->expected_len, tp->expected_apdu_len, NULL);
            zassert_mem_equal(&buf[base_offset], tp->expected_apdu,
                              tp->expected_len, NULL);
        }
        else
        {
            zassert_equal(encode_unsigned40_fake.call_count, 0, NULL);
        }

        if (tp->expected_len == 6)
        {
            zassert_equal(encode_unsigned48_fake.call_count, 1, NULL);
            zassert_equal(encode_unsigned48_fake.arg0_val,
                          &buf[base_offset], NULL);
            zassert_true(encode_unsigned48_fake.arg1_val == tp->bacint_value,
                         NULL);
            zassert_equal(tp->expected_len, tp->expected_apdu_len, NULL);
            zassert_mem_equal(&buf[base_offset], tp->expected_apdu,
                              tp->expected_len, NULL);
        }
        else
        {
            zassert_equal(encode_unsigned48_fake.call_count, 0, NULL);
        }

        if (tp->expected_len == 7)
        {
            zassert_equal(encode_unsigned56_fake.call_count, 1, NULL);
            zassert_equal(encode_unsigned56_fake.arg0_val,
                          &buf[base_offset], NULL);
            zassert_true(encode_unsigned56_fake.arg1_val == tp->bacint_value,
                         NULL);
            zassert_equal(tp->expected_len, tp->expected_apdu_len, NULL);
            zassert_mem_equal(&buf[base_offset], tp->expected_apdu,
                              tp->expected_len, NULL);
        }
        else
        {
            zassert_equal(encode_unsigned56_fake.call_count, 0, NULL);
        }

        if (tp->expected_len == 8)
        {
            zassert_equal(encode_unsigned64_fake.call_count, 1, NULL);
            zassert_equal(encode_unsigned64_fake.arg0_val,
                          &buf[base_offset], NULL);
            zassert_true(encode_unsigned64_fake.arg1_val == tp->bacint_value,
                         NULL);
            zassert_equal(tp->expected_len, tp->expected_apdu_len, NULL);
            zassert_mem_equal(&buf[base_offset], tp->expected_apdu,
                              tp->expected_len, NULL);
        }
        else
        {
            zassert_equal(encode_unsigned64_fake.call_count, 0, NULL);
        }
    }
}

static void test_encode_bacnet_signed(void)
{
    struct test_point {
        int32_t value;
        uint32_t expected_len;
        uint8_t expected_apdu[8];
        uint16_t expected_apdu_len;
        uint8_t apdu_offset;
        uint8_t buf_fill;
    };
    const struct test_point test_points[] =
    {

        {
            .value = INT_MIN,
            .expected_len = 4,
            .expected_apdu = { 0x80U, 0x00U, 0x00U, 0x00U, },
            .expected_apdu_len = 4,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .value = -8388608 - 1,
            .expected_len = 4,
            .expected_apdu = { 0xFFU, 0x7FU, 0xFFU, 0xFFU, },
            .expected_apdu_len = 4,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .value = -8388608,
            .expected_len = 3,
            .expected_apdu = { 0x80U, 0x00U, 0x00U, },
            .expected_apdu_len = 3,
            .apdu_offset = 1,
            .buf_fill = 0xC3U,
        },
        {
            .value = SHRT_MIN - 1,
            .expected_len = 3,
            .expected_apdu = { 0xFFU, 0x7FU, 0xFFU, },
            .expected_apdu_len = 3,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .value = SHRT_MIN,
            .expected_len = 2,
            .expected_apdu = { 0x80U, 0x00U, },
            .expected_apdu_len = 2,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .value = CHAR_MIN - 1,
            .expected_len = 2,
            .expected_apdu = { 0xFFU, 0x7FU, },
            .expected_apdu_len = 2,
            .apdu_offset = 1,
            .buf_fill = 0xC3U,
        },
        {
            .value = CHAR_MIN,
            .expected_len = 1,
            .expected_apdu = { 0x80U, },
            .expected_apdu_len = 1,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .value = -1,
            .expected_len = 1,
            .expected_apdu = { 0xFFU, },
            .expected_apdu_len = 1,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .value = 0,
            .expected_len = 1,
            .expected_apdu = { 0x00U, },
            .expected_apdu_len = 1,
            .apdu_offset = 1,
            .buf_fill = 0xC3U,
        },
        {
            .value = 1,
            .expected_len = 1,
            .expected_apdu = { 0x01U, },
            .expected_apdu_len = 1,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .value = CHAR_MAX,
            .expected_len = 1,
            .expected_apdu = { 0x7FU, },
            .expected_apdu_len = 1,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .value = CHAR_MAX + 1,
            .expected_len = 2,
            .expected_apdu = { 0x00U, 0x80U, },
            .expected_apdu_len = 2,
            .apdu_offset = 1,
            .buf_fill = 0xC3U,
        },
        {
            .value = SHRT_MAX,
            .expected_len = 2,
            .expected_apdu = { 0x7FU, 0xFFU, },
            .expected_apdu_len = 2,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .value = SHRT_MAX + 1,
            .expected_len = 3,
            .expected_apdu = { 0x00U, 0x80U, 0x00U, },
            .expected_apdu_len = 3,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .value = 8388607,
            .expected_len = 3,
            .expected_apdu = { 0x7FU, 0xFFU, 0xFFU, },
            .expected_apdu_len = 3,
            .apdu_offset = 1,
            .buf_fill = 0xC3U,
        },
        {
            .value = 8388607 + 1,
            .expected_len = 4,
            .expected_apdu = { 0x00U, 0x80U, 0x00U, 0x00U, },
            .expected_apdu_len = 4,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        {
            .value = INT_MAX,
            .expected_len = 4,
            .expected_apdu = { 0x7FU, 0xFFU, 0xFFU, 0xFFU, },
            .expected_apdu_len = 4,
            .apdu_offset = 1,
            .buf_fill = 0xC3U,
        },
    };
    uint8_t buf[8] = { 0 };
    int len = 0;
    int i = 0;

    for (i = 0; i < sizeof(test_points)/sizeof(test_points[0]); ++i)
    {
        const struct test_point * const tp = &test_points[i];
        const int base_offset = tp->apdu_offset;


        FFF_RESET_HISTORY();
        BACINT_FFF_FAKES_LIST(RESET_FAKE);

        bacnet_signed_length_fake.custom_fake = 
            bacnet_signed_length_custom_fake;

        encode_signed8_fake.custom_fake = encode_signed8_custom_fake;
        encode_signed8_fake.return_val = 1;
        encode_signed16_fake.custom_fake = encode_signed16_custom_fake;
        encode_signed16_fake.return_val = 2;
        encode_signed24_fake.custom_fake = encode_signed24_custom_fake;
        encode_signed24_fake.return_val = 3;
        encode_signed32_fake.custom_fake = encode_signed32_custom_fake;
        encode_signed32_fake.return_val = 4;

        len = encode_bacnet_signed(NULL, tp->value);
        zassert_equal(len, tp->expected_len, NULL);

        zassert_equal(bacnet_signed_length_fake.call_count, 1, NULL);
        zassert_equal(encode_signed8_fake.call_count, 0, NULL);
        zassert_equal(encode_signed16_fake.call_count, 0, NULL);
        zassert_equal(encode_signed24_fake.call_count, 0, NULL);
        zassert_equal(encode_signed32_fake.call_count, 0, NULL);


        memset(buf, tp->buf_fill, sizeof(buf));

        len = encode_bacnet_signed(&buf[base_offset], tp->value);

        zassert_equal(buf[base_offset - 1], tp->buf_fill, NULL);
        zassert_equal(buf[base_offset + tp->expected_len], tp->buf_fill, NULL);

        zassert_equal(len, tp->expected_len, NULL);
        if (tp->expected_len == 1)
        {
            zassert_equal(buf[base_offset], (uint8_t)tp->value, NULL);
            zassert_mem_equal(&buf[base_offset], tp->expected_apdu,
                              tp->expected_len, NULL);
        }

        if (tp->expected_len == 2)
        {
            zassert_equal(encode_signed16_fake.call_count, 1, NULL);
            zassert_equal(encode_signed16_fake.arg0_val,
                          &buf[base_offset], NULL);
            zassert_true(encode_signed16_fake.arg1_val == tp->value,
                         NULL);
            zassert_equal(tp->expected_len, tp->expected_apdu_len, NULL);
            zassert_mem_equal(&buf[base_offset], tp->expected_apdu,
                              tp->expected_len, NULL);
        }
        else
        {
            zassert_equal(encode_signed16_fake.call_count, 0, NULL);
        }

        if (tp->expected_len == 3)
        {
            zassert_equal(encode_signed24_fake.call_count, 1, NULL);
            zassert_equal(encode_signed24_fake.arg0_val,
                          &buf[base_offset], NULL);
            zassert_true(encode_signed24_fake.arg1_val == tp->value,
                         NULL);
            zassert_equal(tp->expected_len, tp->expected_apdu_len, NULL);
            zassert_mem_equal(&buf[base_offset], tp->expected_apdu,
                              tp->expected_len, NULL);
        }
        else
        {
            zassert_equal(encode_signed24_fake.call_count, 0, NULL);
        }

        if (tp->expected_len == 4)
        {
            zassert_equal(encode_signed32_fake.call_count, 1, NULL);
            zassert_equal(encode_signed32_fake.arg0_val,
                          &buf[base_offset], NULL);
            zassert_true(encode_signed32_fake.arg1_val == tp->value,
                         NULL);
            zassert_equal(tp->expected_len, tp->expected_apdu_len, NULL);
            zassert_mem_equal(&buf[base_offset], tp->expected_apdu,
                              tp->expected_len, NULL);
        }
        else
        {
            zassert_equal(encode_signed32_fake.call_count, 0, NULL);
        }
    }
}

static void test_encode_bacnet_time_set(void)
{
    struct test_point {
        BACNET_TIME bacnet_time;
        uint32_t expected_len;
        uint8_t expected_apdu[8];
        uint16_t expected_apdu_len;
        uint8_t apdu_offset;
        uint8_t buf_fill;
    };
    const struct test_point test_points[] =
    {
        { /* Normal encoding, BACnet epoch date */
            .bacnet_time =
                {
		    .hour = 0,
		    .min = 0,
		    .sec = 0,
		    .hundredths = 0,
                },
            .expected_len = 4, /* success */
	    .expected_apdu = { 0x00U, 0x00U, 0x00U, 0x00U },
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
    };
    uint8_t buf[8] = { 0 };
    int len = 0;
    int i = 0;

    for (i = 0; i < sizeof(test_points)/sizeof(test_points[0]); ++i)
    {
        const struct test_point * const tp = &test_points[i];
        const int base_offset = tp->apdu_offset;
        BACNET_TIME bacnet_time = tp->bacnet_time; /* struct copy */


        len = encode_bacnet_time(NULL, &bacnet_time);
        zassert_equal(len, tp->expected_len, NULL);
        zassert_mem_equal(&bacnet_time, &tp->bacnet_time,
                          sizeof(bacnet_time), NULL);

	len = encode_application_time(NULL, &bacnet_time);
        zassert_equal(len, 1 + tp->expected_len, NULL);
        zassert_mem_equal(&bacnet_time, &tp->bacnet_time,
                          sizeof(bacnet_time), NULL);
	//TODO: Verify APDU was written correctly

	len = encode_context_time(NULL, 0x0, &bacnet_time);
        zassert_equal(len, 1 + tp->expected_len, NULL);
        zassert_mem_equal(&bacnet_time, &tp->bacnet_time,
                          sizeof(bacnet_time), NULL);
	//TODO: Verify APDU was written correctly

	len = encode_context_time(NULL, 0xE, &bacnet_time);
        zassert_equal(len, 1 + tp->expected_len, NULL);
        zassert_mem_equal(&bacnet_time, &tp->bacnet_time,
                          sizeof(bacnet_time), NULL);

	len = encode_context_time(NULL, 0xF, &bacnet_time);
        zassert_equal(len, 2 + tp->expected_len, NULL);
        zassert_mem_equal(&bacnet_time, &tp->bacnet_time,
                          sizeof(bacnet_time), NULL);

	len = encode_context_time(NULL, 0xFF, &bacnet_time);
        zassert_equal(len, 2 + tp->expected_len, NULL);
        zassert_mem_equal(&bacnet_time, &tp->bacnet_time,
                          sizeof(bacnet_time), NULL);


        memset(buf, tp->buf_fill, sizeof(buf));
        len = encode_bacnet_time(&buf[base_offset], &bacnet_time);
        zassert_equal(len, tp->expected_len, NULL);
        zassert_mem_equal(&bacnet_time, &tp->bacnet_time,
                          sizeof(bacnet_time), NULL);

        zassert_equal(buf[base_offset - 1], tp->buf_fill, NULL);
        zassert_equal(buf[base_offset + len], tp->buf_fill, NULL);

	zassert_mem_equal(&buf[base_offset], &tp->expected_apdu,
			  tp->expected_len, NULL);
    }
}

static void test_encode_bacnet_date_set(void)
{
    const uint16_t bacnet_date_year_epoch = 1900U; /* AD */
#if 0
    const uint16_t bacnet_date_year_2dig_limit = 0xFFU;
#endif

    const uint8_t bacnet_month_jan = 1;
#if 0
    const uint8_t bacnet_month_feb = 2;
    const uint8_t bacnet_month_mar = 3;
    const uint8_t bacnet_month_apr = 4;
    const uint8_t bacnet_month_may = 5;
    const uint8_t bacnet_month_jun = 6;
    const uint8_t bacnet_month_jul = 7;
    const uint8_t bacnet_month_aug = 8;
    const uint8_t bacnet_month_sep = 9;
    const uint8_t bacnet_month_oct = 10;
    const uint8_t bacnet_month_nov = 11;
    const uint8_t bacnet_month_dec = 12;
#endif

    const uint8_t bacnet_weekday_mon = 1;
    const uint8_t bacnet_weekday_tue = 2;
#if 0
    const uint8_t bacnet_weekday_wed = 3;
#endif
    const uint8_t bacnet_weekday_thu = 4;
#if 0
    const uint8_t bacnet_weekday_fri = 5;
    const uint8_t bacnet_weekday_sat = 6;
    const uint8_t bacnet_weekday_sun = 7;
#endif

    struct test_point {
        BACNET_DATE bacnet_date;
        uint32_t expected_len;
        uint8_t expected_apdu[8];
        uint16_t expected_apdu_len;
        uint8_t apdu_offset;
        uint8_t buf_fill;
    };
    const struct test_point test_points[] =
    {
        { /* Normal encoding, BACnet epoch date */
            .bacnet_date =
                {
                    .year = bacnet_date_year_epoch,
                    .month = 1,
                    .day = 1,
                    .wday = bacnet_weekday_mon,
                },
            .expected_len = 4, /* success */
            .expected_apdu =
                {
                    bacnet_date_year_epoch - bacnet_date_year_epoch,
                    bacnet_month_jan,
                    1,
                    bacnet_weekday_mon,
                },
            .expected_apdu_len = 4,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        { /* Unix epoch rollover - normal encoding */
            .bacnet_date =
                {
                    .year = 2038 - bacnet_date_year_epoch,
                    .month = 1,
                    .day = 19,
                    .wday = bacnet_weekday_tue,
                },
            .expected_len = 4, /* success */
            .expected_apdu =
                {
                    (uint8_t)(2038 - bacnet_date_year_epoch),
                    bacnet_month_jan,
                    19,
                    bacnet_weekday_tue,
                },
            .expected_apdu_len = 4,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        { /* Unix epoch - "2-digit" year encoding (already relative) */
            .bacnet_date =
                {
                    .year = 70, /* 1970 - bacnet_date_year_epoch, < 0x100 */
                    .month = 1,
                    .day = 1,
                    .wday = bacnet_weekday_thu,
                },
            .expected_len = 4, /* success */
            .expected_apdu =
                {
                    1970 - bacnet_date_year_epoch,
                    bacnet_month_jan,
                    1,
                    bacnet_weekday_thu,
                },
            .expected_apdu_len = 4,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
        { /* Invalid - year is out of range */
            .bacnet_date =
                {
                    .year = 0x100,
                    .month = 1,
                    .day = 1,
                    .wday = bacnet_weekday_thu,
                },
            .expected_len = -1, /* BACNET_STATUS_ERROR */
            .expected_apdu =
                {
                    0xC3U, /* buf_fill */
                    0xC3U, /* buf_fill */
                    0xC3U, /* buf_fill */
                    0xC3U, /* buf_fill */
                },
            .expected_apdu_len = 4,
            .apdu_offset = 2,
            .buf_fill = 0xC3U,
        },
    };
    uint8_t buf[8] = { 0 };
    int len = 0;
    int i = 0;

    for (i = 0; i < sizeof(test_points)/sizeof(test_points[0]); ++i)
    {
        const struct test_point * const tp = &test_points[i];
        const int base_offset = tp->apdu_offset;
        BACNET_DATE bacnet_date = tp->bacnet_date; /* struct copy */


        len = encode_bacnet_date(NULL, &bacnet_date);
        zassert_equal(len, tp->expected_len, NULL);
        zassert_mem_equal(&bacnet_date, &tp->bacnet_date,
                          sizeof(bacnet_date), NULL);

	len = encode_application_date(NULL, &bacnet_date);
        zassert_equal(len, 1 + tp->expected_len, NULL);
        zassert_mem_equal(&bacnet_date, &tp->bacnet_date,
                          sizeof(bacnet_date), NULL);

	len = encode_context_date(NULL, 0x0, &bacnet_date);
        zassert_equal(len, 1 + tp->expected_len, NULL);
        zassert_mem_equal(&bacnet_date, &tp->bacnet_date,
                          sizeof(bacnet_date), NULL);

	len = encode_context_date(NULL, 0xE, &bacnet_date);
        zassert_equal(len, 1 + tp->expected_len, NULL);
        zassert_mem_equal(&bacnet_date, &tp->bacnet_date,
                          sizeof(bacnet_date), NULL);

	len = encode_context_date(NULL, 0xF, &bacnet_date);
        zassert_equal(len, 2 + tp->expected_len, NULL);
        zassert_mem_equal(&bacnet_date, &tp->bacnet_date,
                          sizeof(bacnet_date), NULL);

	len = encode_context_date(NULL, 0xFF, &bacnet_date);
        zassert_equal(len, 2 + tp->expected_len, NULL);
        zassert_mem_equal(&bacnet_date, &tp->bacnet_date,
                          sizeof(bacnet_date), NULL);


        memset(buf, tp->buf_fill, sizeof(buf));
        len = encode_bacnet_date(&buf[base_offset], &bacnet_date);
        zassert_equal(len, tp->expected_len, NULL);
        zassert_mem_equal(&bacnet_date, &tp->bacnet_date,
                          sizeof(bacnet_date), NULL);

        zassert_equal(buf[base_offset - 1], tp->buf_fill, NULL);
        zassert_equal(buf[base_offset + len], tp->buf_fill, NULL);

        if (tp->expected_len == -1)
        {
            /* When a BACNET_STATUS_ERROR (-1) is returned for len
             * we need to verify none of the apdu was written to.
             *
             * For this case the .expected_apdu has been filled with
             * .buf_fill values and the .expected_apdu_len is valid
             * to set up a simple memory comparison.
             */
            zassert_mem_equal(&buf[base_offset], tp->expected_apdu,
                              tp->expected_apdu_len, NULL);
        }
        else
        {
            zassert_mem_equal(&buf[base_offset], tp->expected_apdu,
                              tp->expected_len, NULL);
        }
    }
}

static void test_encode_simple_ack(void)
{
    const uint8_t pdu_type_simple_ack = 0x20U;

    uint8_t buf[8] = { 0 };
    const uint8_t buf_fill = 0xFFU;
    const uint8_t base_offset = 1;
    int len = 0;

    zassert_equal(encode_simple_ack(NULL, 0, 0), 3, NULL);
    zassert_equal(encode_simple_ack(NULL, 0xFFU, 0xFFU), 3, NULL);

    memset(buf, buf_fill, sizeof(buf));
    len = encode_simple_ack(&buf[base_offset], 0, 1);
    zassert_equal(len, 3, NULL);
    zassert_equal(buf[base_offset - 1], buf_fill, NULL);
    zassert_equal(buf[base_offset + len], buf_fill, NULL);
    zassert_equal(buf[base_offset + 0], pdu_type_simple_ack, NULL);
    zassert_equal(buf[base_offset + 1], 0, NULL);
    zassert_equal(buf[base_offset + 2], 1, NULL);

    memset(buf, buf_fill, sizeof(buf));
    len = encode_simple_ack(&buf[base_offset], 0x83U, 0xF7U);
    zassert_equal(len, 3, NULL);
    zassert_equal(buf[base_offset - 1], buf_fill, NULL);
    zassert_equal(buf[base_offset + len], buf_fill, NULL);
    zassert_equal(buf[base_offset + 0], pdu_type_simple_ack, NULL);
    zassert_equal(buf[base_offset + 1], 0x83U, NULL);
    zassert_equal(buf[base_offset + 2], 0xF7U, NULL);
}

static void test_nothing(void)
{
    ztest_test_pass();
}

void test_main(void)
{
    ztest_test_suite(bacnet_bacdcode,
     ztest_unit_test(test_decode_max_segs),
     ztest_unit_test(test_decode_max_apdu),
     ztest_unit_test(test_encode_max_segs_max_apdu),
     ztest_unit_test(test_encode_tag),
     ztest_unit_test(test_encode_opening_tag__embedded),
     ztest_unit_test(test_encode_opening_tag__extended),
     ztest_unit_test(test_encode_closing_tag__embedded),
     ztest_unit_test(test_encode_closing_tag__extended),
     ztest_unit_test(test_decode_tag_number),
     ztest_unit_test(test_bacnet_tag_number_decode),
     ztest_unit_test(test_decode_is_opening_tag),
     ztest_unit_test(test_decode_is_closing_tag),
     ztest_unit_test(test_decode_tag_number_and_value__NULL_apdu),
     ztest_unit_test(test_decode_tag_number_and_value__special_tags),
     ztest_unit_test(test_decode_tag_number_and_value__values_below5),
     ztest_unit_test(test_decode_tag_number_and_value__uint8_extension),
     ztest_unit_test(test_decode_tag_number_and_value__uint16_extension),
     ztest_unit_test(test_decode_tag_number_and_value__uint32_extension),
     ztest_unit_test(test_bacnet_tag_number_and_value_decode__NULL_apdu),
     ztest_unit_test(test_bacnet_tag_number_and_value_decode__apdu_len_zero),
     ztest_unit_test(test_bacnet_tag_number_and_value_decode__special_tags),
     ztest_unit_test(test_bacnet_tag_number_and_value_decode__values_below5),
     ztest_unit_test(test_bacnet_tag_number_and_value_decode__uint8_extension),
     ztest_unit_test(test_bacnet_tag_number_and_value_decode__uint16_extension),
     ztest_unit_test(test_bacnet_tag_number_and_value_decode__uint32_extension),
     ztest_unit_test(test_decode_is_context_open_close_tag_set),
     ztest_unit_test(test_encode_application_boolean),
     ztest_unit_test(test_encode_context_boolean),
     ztest_unit_test(test_decode_context_boolean2),
     ztest_unit_test(test_decode_context_boolean),
     ztest_unit_test(test_decode_boolean),
     ztest_unit_test(test_encode_application_null),
     ztest_unit_test(test_encode_context_null),
     ztest_unit_test(test_byte_reverse_bits),
     /* TODO: ztest_unit_test(test_decode_bitstring), */
     /* TODO: ztest_unit_test(test_decode_context_bitstring), */
     /* TODO: ztest_unit_test(test_encode_bitstring), */
     /* TODO: ztest_unit_test(test_encode_application_bitstring), */
     /* TODO: ztest_unit_test(test_encode_context_bitstring), */
     /* TODO: ztest_unit_test(test_decode_object_id_safe), */
     /* TODO: ztest_unit_test(test_decode_object_id), */
     /* TODO: ztest_unit_test(test_bacnet_object_id_decode), */
     /* TODO: ztest_unit_test(test_bacnet_object_id_application_decode), */
     /* TODO: ztest_unit_test(test_bacnet_object_id_context_decode), */
     /* TODO: ztest_unit_test(test_decode_context_object_id), */
     /* TODO: ztest_unit_test(test_encode_bacnet_object_id), */
     /* TODO: ztest_unit_test(test_encode_context_object_id), */
     /* TODO: ztest_unit_test(test_encode_application_object_id), */
     /* TODO: ztest_unit_test(test_encode_octet_string), */
     /* TODO: ztest_unit_test(test_encode_application_octet_string), */
     /* TODO: ztest_unit_test(test_encode_context_octet_string), */
     /* TODO: ztest_unit_test(test_bacnet_octet_string_decode), */
     /* TODO: ztest_unit_test(test_decode_octet_string), */
     /* TODO: ztest_unit_test(test_decode_context_octet_string), */
     /* TODO: ztest_unit_test(test_bacnet_octet_string_application_decode), */
     /* TODO: ztest_unit_test(test_encode_bacnet_character_string_safe), */
     /* TODO: ztest_unit_test(test_encode_bacnet_character_string), */
     /* TODO: ztest_unit_test(test_encode_application_character_string), */
     /* TODO: ztest_unit_test(test_encode_context_character_string), */
     /* TODO: ztest_unit_test(test_bacnet_charater_string_decode), */
     /* TODO: ztest_unit_test(test_decode_character_string), */
     /* TODO: ztest_unit_test(test_bacnet_character_string_context_decode), */
     /* TODO: ztest_unit_test(test_decode_context_character_string), */
     ztest_unit_test(test_bacnet_unsigned_decode),
     /* TODO: ztest_unit_test(test_bacnet_unsigned_context_decode), */
     /* TODO: ztest_unit_test(test_bacnet_unsigned_application_decode), */
     /* TODO: ztest_unit_test(test_decode_unsigned), */
     /* TODO: ztest_unit_test(test_decode_context_unsigned), */
     ztest_unit_test(test_encode_bacnet_unsigned),
     /* TODO: ztest_unit_test(test_encode_context_unsigned), */
     /* TODO: ztest_unit_test(test_encode_application_unsigned), */
     /* TODO: ztest_unit_test(test_bacnet_enumerated_decode), */
     /* TODO: ztest_unit_test(test_decode_enumerated), */
     /* TODO: ztest_unit_test(test_bacnet_enumerated_context_decode), */
     /* TODO: ztest_unit_test(test_decode_context_enumerated), */
     /* TODO: ztest_unit_test(test_encode_bacnet_enumerated), */
     /* TODO: ztest_unit_test(test_encode_application_enumerated), */
     /* TODO: ztest_unit_test(test_encode_context_enumerated), */
     /* TODO: ztest_unit_test(test_bacnet_signed_decode), */
     /* TODO: ztest_unit_test(test_bacnet_signed_context_decode), */
     /* TODO: ztest_unit_test(test_bacnet_signed_application_decode), */
     /* TODO: ztest_unit_test(test_decode_signed), */
     /* TODO: ztest_unit_test(test_decode_context_signed), */
     ztest_unit_test(test_encode_bacnet_signed),
     /* TODO: ztest_unit_test(test_encode_application_signed), */
     /* TODO: ztest_unit_test(test_encode_context_signed), */
     /* TODO: ztest_unit_test(test_encode_application_real), */
     /* TODO: ztest_unit_test(test_encode_context_real), */
     /* TODO: ztest_unit_test(test_decode_context_real), */
     /* TODO: ztest_unit_test(test_encode_application_double), */
     /* TODO: ztest_unit_test(test_encode_context_double), */
     /* TODO: ztest_unit_test(test_decode_context_double), */
     ztest_unit_test(test_encode_bacnet_time_set),
     /* TODO: ztest_unit_test(test_bacnet_time_decode), */
     /* TODO: ztest_unit_test(test_bacnet_time_context_decode), */
     /* TODO: ztest_unit_test(test_bacnet_time_application_decode), */
     /* TODO: ztest_unit_test(test_decode_bacnet_time), */
     /* TODO: ztest_unit_test(test_decode_bacnet_time_safe), */
     /* TODO: ztest_unit_test(test_decode_application_time), */
     /* TODO: ztest_unit_test(test_decode_context_bacnet_time), */
     ztest_unit_test(test_encode_bacnet_date_set),
     /* TODO: ztest_unit_test(test_decode_date), */
     /* TODO: ztest_unit_test(test_decode_date_safe), */
     /* TODO: ztest_unit_test(test_decode_application_date), */
     /* TODO: ztest_unit_test(test_decode_context_date), */
     ztest_unit_test(test_encode_simple_ack),
     /* TODO: ztest_unit_test(test_encode_bacnet_address), */
     /* TODO: ztest_unit_test(test_decode_bacnet_address), */
     /* TODO: ztest_unit_test(test_encode_context_bacnet_address), */
     /* TODO: ztest_unit_test(test_decode_context_bacnet_address), */
     ztest_unit_test(test_nothing)
     );
    ztest_run_test_suite(bacnet_bacdcode);
}

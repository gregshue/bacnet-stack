/*
 * Copyright
 *
 * Private file
 */

#include "bacnet/bacstr.h"
#include <zephyr/fff.h>

/* List of fakes used by this unit tester */
#define BACSTR_FFF_FAKES_LIST(FAKE)        \
                FAKE(bitstring_init)		\
                FAKE(bitstring_set_bit)		\
                FAKE(bitstring_bit)		\
                FAKE(bitstring_bits_used)		\
                FAKE(bitstring_bytes_used)		\
                FAKE(bitstring_bits_capacity)		\
                FAKE(bitstring_octet)		\
                FAKE(bitstring_set_octet)		\
                FAKE(bitstring_set_bits_used)		\
                FAKE(bitstring_copy)		\
                FAKE(bitstring_same)		\
                FAKE(bitstring_init_ascii)		\
                FAKE(characterstring_init)		\
                FAKE(characterstring_init_ansi)		\
                FAKE(characterstring_init_ansi_safe)		\
                FAKE(characterstring_copy)		\
                FAKE(characterstring_ansi_copy)		\
                FAKE(characterstring_same)		\
                FAKE(characterstring_ansi_same)		\
                FAKE(characterstring_append)		\
                FAKE(characterstring_truncate)		\
                FAKE(characterstring_set_encoding)		\
                FAKE(characterstring_value)		\
                FAKE(characterstring_length)		\
                FAKE(characterstring_encoding)		\
                FAKE(characterstring_capacity)		\
                FAKE(characterstring_printable)		\
                FAKE(characterstring_valid)		\
                FAKE(utf8_isvalid)		\
                FAKE(octetstring_init)		\
                FAKE(octetstring_init_ascii_hex)		\
                FAKE(octetstring_copy)		\
                FAKE(octetstring_copy_value)		\
                FAKE(octetstring_append)		\
                FAKE(octetstring_truncate)		\
                FAKE(octetstring_value)		\
                FAKE(octetstring_length)		\
                FAKE(octetstring_capacity)		\
                FAKE(octetstring_value_same)		\


DECLARE_FAKE_VOID_FUNC(
    bitstring_init,
        BACNET_BIT_STRING *);
DECLARE_FAKE_VOID_FUNC(
    bitstring_set_bit,
        BACNET_BIT_STRING *,
        uint8_t,
        bool);
DECLARE_FAKE_VALUE_FUNC(
    bool, bitstring_bit,
        BACNET_BIT_STRING *,
        uint8_t);
DECLARE_FAKE_VALUE_FUNC(
    uint8_t, bitstring_bits_used,
        BACNET_BIT_STRING *);
/* returns the number of bytes that a bit string is using */
DECLARE_FAKE_VALUE_FUNC(
    uint8_t, bitstring_bytes_used,
        BACNET_BIT_STRING *);
DECLARE_FAKE_VALUE_FUNC(
    unsigned, bitstring_bits_capacity,
        BACNET_BIT_STRING *);
/* used for encoding and decoding from the APDU */
DECLARE_FAKE_VALUE_FUNC(
    uint8_t, bitstring_octet,
        BACNET_BIT_STRING *,
        uint8_t);
DECLARE_FAKE_VALUE_FUNC(
    bool, bitstring_set_octet,
        BACNET_BIT_STRING *,
        uint8_t,
        uint8_t);
DECLARE_FAKE_VALUE_FUNC(
    bool, bitstring_set_bits_used,
        BACNET_BIT_STRING *,
        uint8_t,
        uint8_t);
DECLARE_FAKE_VALUE_FUNC(
    bool, bitstring_copy,
        BACNET_BIT_STRING *,
        BACNET_BIT_STRING *);
DECLARE_FAKE_VALUE_FUNC(
    bool, bitstring_same,
        BACNET_BIT_STRING *,
        BACNET_BIT_STRING *);
DECLARE_FAKE_VALUE_FUNC(
    bool, bitstring_init_ascii,
        BACNET_BIT_STRING *,
        const char *);

/* returns false if the string exceeds capacity
   initialize by using length=0 */
DECLARE_FAKE_VALUE_FUNC(
    bool, characterstring_init,
        BACNET_CHARACTER_STRING *,
        uint8_t,
        const char *,
        size_t);
/* used for ANSI C-Strings */
DECLARE_FAKE_VALUE_FUNC(
    bool, characterstring_init_ansi,
        BACNET_CHARACTER_STRING *,
        const char *);
DECLARE_FAKE_VALUE_FUNC(
    bool, characterstring_init_ansi_safe,
        BACNET_CHARACTER_STRING *,
        const char *,
        size_t);
DECLARE_FAKE_VALUE_FUNC(
    bool, characterstring_copy,
        BACNET_CHARACTER_STRING *,
        BACNET_CHARACTER_STRING *);
DECLARE_FAKE_VALUE_FUNC(
    bool, characterstring_ansi_copy,
        char *,
        size_t,
        BACNET_CHARACTER_STRING *);
/* returns true if the strings are the same length, encoding, */
DECLARE_FAKE_VALUE_FUNC(
    bool, characterstring_same,
        BACNET_CHARACTER_STRING *,
        BACNET_CHARACTER_STRING *);
DECLARE_FAKE_VALUE_FUNC(
    bool, characterstring_ansi_same,
        BACNET_CHARACTER_STRING *,
        const char *);
/* returns false if the string exceeds capacity */
DECLARE_FAKE_VALUE_FUNC(
    bool, characterstring_append,
        BACNET_CHARACTER_STRING *,
        const char *,
        size_t);
/* This function sets a new length without changing the.
   If length exceeds capacity, no modification happens and
   function returns false.  */
DECLARE_FAKE_VALUE_FUNC(
    bool, characterstring_truncate,
        BACNET_CHARACTER_STRING *,
        size_t);
DECLARE_FAKE_VALUE_FUNC(
    bool, characterstring_set_encoding,
        BACNET_CHARACTER_STRING *,
        uint8_t);
/* Returns the */
DECLARE_FAKE_VALUE_FUNC(
    char *, characterstring_value,
        BACNET_CHARACTER_STRING *);
/* returns the length */
DECLARE_FAKE_VALUE_FUNC(
    size_t, characterstring_length,
        BACNET_CHARACTER_STRING *);
DECLARE_FAKE_VALUE_FUNC(
    uint8_t, characterstring_encoding,
        BACNET_CHARACTER_STRING *);
DECLARE_FAKE_VALUE_FUNC(
    size_t, characterstring_capacity,
        BACNET_CHARACTER_STRING *);
DECLARE_FAKE_VALUE_FUNC(
    bool, characterstring_printable,
        BACNET_CHARACTER_STRING *);
DECLARE_FAKE_VALUE_FUNC(
    bool, characterstring_valid,
        BACNET_CHARACTER_STRING *);
DECLARE_FAKE_VALUE_FUNC(
    bool, utf8_isvalid,
        const char *,
        size_t);

    /* returns false if the string exceeds capacity
       initialize by using length=0 */
DECLARE_FAKE_VALUE_FUNC(
    bool, octetstring_init,
        BACNET_OCTET_STRING *,
        uint8_t *,
        size_t);
    /* converts an null terminated ASCII Hex string to an octet string.
       returns true if successfully converted and fits; false if too long */
DECLARE_FAKE_VALUE_FUNC(
    bool, octetstring_init_ascii_hex,
        BACNET_OCTET_STRING *,
        const char *);
DECLARE_FAKE_VALUE_FUNC(
    bool, octetstring_copy,
        BACNET_OCTET_STRING *,
        BACNET_OCTET_STRING *);
DECLARE_FAKE_VALUE_FUNC(
    size_t, octetstring_copy_value,
        uint8_t *,
        size_t,
        BACNET_OCTET_STRING *);
/* returns false if the string exceeds capacity */
DECLARE_FAKE_VALUE_FUNC(
    bool, octetstring_append,
        BACNET_OCTET_STRING *,
        uint8_t *,
        size_t);
/* This function sets a new length without changing the.
   If length exceeds capacity, no modification happens and
   function returns false.  */
DECLARE_FAKE_VALUE_FUNC(
    bool, octetstring_truncate,
        BACNET_OCTET_STRING *,
        size_t);
/* Returns the */
DECLARE_FAKE_VALUE_FUNC(
    uint8_t *, octetstring_value,
        BACNET_OCTET_STRING *);
/* Returns the length.*/
DECLARE_FAKE_VALUE_FUNC(
    size_t, octetstring_length,
        BACNET_OCTET_STRING *);
DECLARE_FAKE_VALUE_FUNC(
    size_t, octetstring_capacity,
        BACNET_OCTET_STRING *);
    /* returns true if the same length and contents */
DECLARE_FAKE_VALUE_FUNC(
    bool, octetstring_value_same,
        BACNET_OCTET_STRING *,
        BACNET_OCTET_STRING *);

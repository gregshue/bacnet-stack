/*
 * Copyright
 *
 * Private
 */

#include "mocks/bacstr.h"

DEFINE_FAKE_VOID_FUNC(
    bitstring_init,
        BACNET_BIT_STRING *);
DEFINE_FAKE_VOID_FUNC(
    bitstring_set_bit,
        BACNET_BIT_STRING *,
        uint8_t,
        bool);
DEFINE_FAKE_VALUE_FUNC(
    bool, bitstring_bit,
        BACNET_BIT_STRING *,
        uint8_t);
DEFINE_FAKE_VALUE_FUNC(
    uint8_t, bitstring_bits_used,
        BACNET_BIT_STRING *);
/* returns the number of bytes that a bit string is using */
DEFINE_FAKE_VALUE_FUNC(
    uint8_t, bitstring_bytes_used,
        BACNET_BIT_STRING *);
DEFINE_FAKE_VALUE_FUNC(
    unsigned, bitstring_bits_capacity,
        BACNET_BIT_STRING *);
/* used for encoding and decoding from the APDU */
DEFINE_FAKE_VALUE_FUNC(
    uint8_t, bitstring_octet,
        BACNET_BIT_STRING *,
        uint8_t);
DEFINE_FAKE_VALUE_FUNC(
    bool, bitstring_set_octet,
        BACNET_BIT_STRING *,
        uint8_t,
        uint8_t);
DEFINE_FAKE_VALUE_FUNC(
    bool, bitstring_set_bits_used,
        BACNET_BIT_STRING *,
        uint8_t,
        uint8_t);
DEFINE_FAKE_VALUE_FUNC(
    bool, bitstring_copy,
        BACNET_BIT_STRING *,
        BACNET_BIT_STRING *);
DEFINE_FAKE_VALUE_FUNC(
    bool, bitstring_same,
        BACNET_BIT_STRING *,
        BACNET_BIT_STRING *);
DEFINE_FAKE_VALUE_FUNC(
    bool, bitstring_init_ascii,
        BACNET_BIT_STRING *,
        const char *);

/* returns false if the string exceeds capacity
   initialize by using length=0 */
DEFINE_FAKE_VALUE_FUNC(
    bool, characterstring_init,
        BACNET_CHARACTER_STRING *,
        uint8_t,
        const char *,
        size_t);
/* used for ANSI C-Strings */
DEFINE_FAKE_VALUE_FUNC(
    bool, characterstring_init_ansi,
        BACNET_CHARACTER_STRING *,
        const char *);
DEFINE_FAKE_VALUE_FUNC(
    bool, characterstring_init_ansi_safe,
        BACNET_CHARACTER_STRING *,
        const char *,
        size_t);
DEFINE_FAKE_VALUE_FUNC(
    bool, characterstring_copy,
        BACNET_CHARACTER_STRING *,
        BACNET_CHARACTER_STRING *);
DEFINE_FAKE_VALUE_FUNC(
    bool, characterstring_ansi_copy,
        char *,
        size_t,
        BACNET_CHARACTER_STRING *);
/* returns true if the strings are the same length, encoding, */
DEFINE_FAKE_VALUE_FUNC(
    bool, characterstring_same,
        BACNET_CHARACTER_STRING *,
        BACNET_CHARACTER_STRING *);
DEFINE_FAKE_VALUE_FUNC(
    bool, characterstring_ansi_same,
        BACNET_CHARACTER_STRING *,
        const char *);
/* returns false if the string exceeds capacity */
DEFINE_FAKE_VALUE_FUNC(
    bool, characterstring_append,
        BACNET_CHARACTER_STRING *,
        const char *,
        size_t);
/* This function sets a new length without changing the.
   If length exceeds capacity, no modification happens and
   function returns false.  */
DEFINE_FAKE_VALUE_FUNC(
    bool, characterstring_truncate,
        BACNET_CHARACTER_STRING *,
        size_t);
DEFINE_FAKE_VALUE_FUNC(
    bool, characterstring_set_encoding,
        BACNET_CHARACTER_STRING *,
        uint8_t);
/* Returns the */
DEFINE_FAKE_VALUE_FUNC(
    char *, characterstring_value,
        BACNET_CHARACTER_STRING *);
/* returns the length */
DEFINE_FAKE_VALUE_FUNC(
    size_t, characterstring_length,
        BACNET_CHARACTER_STRING *);
DEFINE_FAKE_VALUE_FUNC(
    uint8_t, characterstring_encoding,
        BACNET_CHARACTER_STRING *);
DEFINE_FAKE_VALUE_FUNC(
    size_t, characterstring_capacity,
        BACNET_CHARACTER_STRING *);
DEFINE_FAKE_VALUE_FUNC(
    bool, characterstring_printable,
        BACNET_CHARACTER_STRING *);
DEFINE_FAKE_VALUE_FUNC(
    bool, characterstring_valid,
        BACNET_CHARACTER_STRING *);
DEFINE_FAKE_VALUE_FUNC(
    bool, utf8_isvalid,
        const char *,
        size_t);

    /* returns false if the string exceeds capacity
       initialize by using length=0 */
DEFINE_FAKE_VALUE_FUNC(
    bool, octetstring_init,
        BACNET_OCTET_STRING *,
        uint8_t *,
        size_t);
    /* converts an null terminated ASCII Hex string to an octet string.
       returns true if successfully converted and fits; false if too long */
DEFINE_FAKE_VALUE_FUNC(
    bool, octetstring_init_ascii_hex,
        BACNET_OCTET_STRING *,
        const char *);
DEFINE_FAKE_VALUE_FUNC(
    bool, octetstring_copy,
        BACNET_OCTET_STRING *,
        BACNET_OCTET_STRING *);
DEFINE_FAKE_VALUE_FUNC(
    size_t, octetstring_copy_value,
        uint8_t *,
        size_t,
        BACNET_OCTET_STRING *);
/* returns false if the string exceeds capacity */
DEFINE_FAKE_VALUE_FUNC(
    bool, octetstring_append,
        BACNET_OCTET_STRING *,
        uint8_t *,
        size_t);
/* This function sets a new length without changing the.
   If length exceeds capacity, no modification happens and
   function returns false.  */
DEFINE_FAKE_VALUE_FUNC(
    bool, octetstring_truncate,
        BACNET_OCTET_STRING *,
        size_t);
/* Returns the */
DEFINE_FAKE_VALUE_FUNC(
    uint8_t *, octetstring_value,
        BACNET_OCTET_STRING *);
/* Returns the length.*/
DEFINE_FAKE_VALUE_FUNC(
    size_t, octetstring_length,
        BACNET_OCTET_STRING *);
DEFINE_FAKE_VALUE_FUNC(
    size_t, octetstring_capacity,
        BACNET_OCTET_STRING *);
    /* returns true if the same length and contents */
DEFINE_FAKE_VALUE_FUNC(
    bool, octetstring_value_same,
        BACNET_OCTET_STRING *,
        BACNET_OCTET_STRING *);

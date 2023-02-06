/*
 * Copyright
 *
 * Private file
 */

#include "bacnet/bacint.h"
#include <zephyr/fff.h>

/* List of fakes used by this unit tester */
#ifdef UINT64_MAX
#define BACINT_FFF_FAKES_LIST(FAKE)        \
		FAKE(encode_unsigned16)		\
		FAKE(decode_unsigned16)		\
		FAKE(encode_unsigned24)		\
		FAKE(decode_unsigned24)		\
		FAKE(encode_unsigned32)		\
		FAKE(decode_unsigned32)		\
		FAKE(encode_unsigned40)		\
		FAKE(decode_unsigned40)		\
		FAKE(encode_unsigned48)		\
		FAKE(decode_unsigned48)		\
		FAKE(encode_unsigned56)		\
		FAKE(decode_unsigned56)		\
		FAKE(encode_unsigned64)		\
		FAKE(decode_unsigned64)		\
		FAKE(bacnet_unsigned_length)		\
		FAKE(bacnet_signed_length)		\
		FAKE(encode_signed8)		\
		FAKE(decode_signed8)		\
		FAKE(encode_signed16)		\
		FAKE(decode_signed16)		\
		FAKE(encode_signed24)		\
		FAKE(decode_signed24)		\
		FAKE(encode_signed32)		\
		FAKE(decode_signed32)		\

#else
#define BACINT_FFF_FAKES_LIST(FAKE)        \
		FAKE(encode_unsigned16)		\
		FAKE(decode_unsigned16)		\
		FAKE(encode_unsigned24)		\
		FAKE(decode_unsigned24)		\
		FAKE(encode_unsigned32)		\
		FAKE(decode_unsigned32)		\
		FAKE(bacnet_unsigned_length)		\
		FAKE(bacnet_signed_length)		\
		FAKE(encode_signed8)		\
		FAKE(decode_signed8)		\
		FAKE(encode_signed16)		\
		FAKE(decode_signed16)		\
		FAKE(encode_signed24)		\
		FAKE(decode_signed24)		\
		FAKE(encode_signed32)		\
		FAKE(decode_signed32)		\

#endif

DECLARE_FAKE_VALUE_FUNC(int, encode_unsigned16, uint8_t *, uint16_t);
DECLARE_FAKE_VALUE_FUNC(int, decode_unsigned16, uint8_t *, uint16_t *);
DECLARE_FAKE_VALUE_FUNC(int, encode_unsigned24, uint8_t *, uint32_t);
DECLARE_FAKE_VALUE_FUNC(int, decode_unsigned24, uint8_t *, uint32_t *);
DECLARE_FAKE_VALUE_FUNC(int, encode_unsigned32, uint8_t *, uint32_t);
DECLARE_FAKE_VALUE_FUNC(int, decode_unsigned32, uint8_t *, uint32_t *);
#ifdef UINT64_MAX
DECLARE_FAKE_VALUE_FUNC(int, encode_unsigned40, uint8_t *, uint64_t);
DECLARE_FAKE_VALUE_FUNC(int, decode_unsigned40, uint8_t *, uint64_t *);
DECLARE_FAKE_VALUE_FUNC(int, encode_unsigned48, uint8_t *, uint64_t);
DECLARE_FAKE_VALUE_FUNC(int, decode_unsigned48, uint8_t *, uint64_t *);
DECLARE_FAKE_VALUE_FUNC(int, encode_unsigned56, uint8_t *, uint64_t);
DECLARE_FAKE_VALUE_FUNC(int, decode_unsigned56, uint8_t *, uint64_t *);
DECLARE_FAKE_VALUE_FUNC(int, encode_unsigned64, uint8_t *, uint64_t);
DECLARE_FAKE_VALUE_FUNC(int, decode_unsigned64, uint8_t *, uint64_t *);
#endif

DECLARE_FAKE_VALUE_FUNC(int, bacnet_unsigned_length, BACNET_UNSIGNED_INTEGER);

DECLARE_FAKE_VALUE_FUNC(int, bacnet_signed_length, int32_t);

/* signed value encoding and decoding */
DECLARE_FAKE_VALUE_FUNC(int, encode_signed8, uint8_t *, int8_t);
DECLARE_FAKE_VALUE_FUNC(int, decode_signed8, uint8_t *, int32_t *);
DECLARE_FAKE_VALUE_FUNC(int, encode_signed16, uint8_t *, int16_t);
DECLARE_FAKE_VALUE_FUNC(int, decode_signed16, uint8_t *, int32_t *);
DECLARE_FAKE_VALUE_FUNC(int, encode_signed24, uint8_t *, int32_t);
DECLARE_FAKE_VALUE_FUNC(int, decode_signed24, uint8_t *, int32_t *);
DECLARE_FAKE_VALUE_FUNC(int, encode_signed32, uint8_t *, int32_t);
DECLARE_FAKE_VALUE_FUNC(int, decode_signed32, uint8_t *, int32_t *);

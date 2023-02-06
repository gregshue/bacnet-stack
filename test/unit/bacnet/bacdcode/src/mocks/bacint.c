/*
 * Copyright
 *
 * Private file
 */

#include "mocks/bacint.h"

DEFINE_FAKE_VALUE_FUNC(int, encode_unsigned16, uint8_t *, uint16_t);
DEFINE_FAKE_VALUE_FUNC(int, decode_unsigned16, uint8_t *, uint16_t *);
DEFINE_FAKE_VALUE_FUNC(int, encode_unsigned24, uint8_t *, uint32_t);
DEFINE_FAKE_VALUE_FUNC(int, decode_unsigned24, uint8_t *, uint32_t *);
DEFINE_FAKE_VALUE_FUNC(int, encode_unsigned32, uint8_t *, uint32_t);
DEFINE_FAKE_VALUE_FUNC(int, decode_unsigned32, uint8_t *, uint32_t *);
#ifdef UINT64_MAX
DEFINE_FAKE_VALUE_FUNC(int, encode_unsigned40, uint8_t *, uint64_t);
DEFINE_FAKE_VALUE_FUNC(int, decode_unsigned40, uint8_t *, uint64_t *);
DEFINE_FAKE_VALUE_FUNC(int, encode_unsigned48, uint8_t *, uint64_t);
DEFINE_FAKE_VALUE_FUNC(int, decode_unsigned48, uint8_t *, uint64_t *);
DEFINE_FAKE_VALUE_FUNC(int, encode_unsigned56, uint8_t *, uint64_t);
DEFINE_FAKE_VALUE_FUNC(int, decode_unsigned56, uint8_t *, uint64_t *);
DEFINE_FAKE_VALUE_FUNC(int, encode_unsigned64, uint8_t *, uint64_t);
DEFINE_FAKE_VALUE_FUNC(int, decode_unsigned64, uint8_t *, uint64_t *);
#endif

DEFINE_FAKE_VALUE_FUNC(int, bacnet_unsigned_length, BACNET_UNSIGNED_INTEGER);

DEFINE_FAKE_VALUE_FUNC(int, bacnet_signed_length, int32_t);

/* signed value encoding and decoding */
DEFINE_FAKE_VALUE_FUNC(int, encode_signed8, uint8_t *, int8_t);
DEFINE_FAKE_VALUE_FUNC(int, decode_signed8, uint8_t *, int32_t *);
DEFINE_FAKE_VALUE_FUNC(int, encode_signed16, uint8_t *, int16_t);
DEFINE_FAKE_VALUE_FUNC(int, decode_signed16, uint8_t *, int32_t *);
DEFINE_FAKE_VALUE_FUNC(int, encode_signed24, uint8_t *, int32_t);
DEFINE_FAKE_VALUE_FUNC(int, decode_signed24, uint8_t *, int32_t *);
DEFINE_FAKE_VALUE_FUNC(int, encode_signed32, uint8_t *, int32_t);
DEFINE_FAKE_VALUE_FUNC(int, decode_signed32, uint8_t *, int32_t *);

/*
 * Copyright
 *
 * Private file
 */

#include "bacnet/bacreal.h"
#include <zephyr/fff.h>

/* List of fakes used by this unit tester */
#define BACREAL_FFF_FAKES_LIST(FAKE)        \
		FAKE(decode_real_safe)		\
		FAKE(decode_real)		\
		FAKE(encode_bacnet_real)		\
		FAKE(decode_double)		\
		FAKE(decode_double_safe)		\
		FAKE(encode_bacnet_double)		\


DECLARE_FAKE_VALUE_FUNC(int, decode_real_safe, uint8_t *, uint32_t, float *);
DECLARE_FAKE_VALUE_FUNC(int, decode_real, uint8_t *, float *);
DECLARE_FAKE_VALUE_FUNC(int, encode_bacnet_real, float, uint8_t *);
DECLARE_FAKE_VALUE_FUNC(int, decode_double, uint8_t *, double *);
DECLARE_FAKE_VALUE_FUNC(int, decode_double_safe, uint8_t *, uint32_t, double *);
DECLARE_FAKE_VALUE_FUNC(int, encode_bacnet_double, double, uint8_t *);

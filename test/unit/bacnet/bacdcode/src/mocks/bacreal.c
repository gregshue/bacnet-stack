/*
 * Copyright
 *
 * Private file
 */

#include "mocks/bacreal.h"

DEFINE_FAKE_VALUE_FUNC(int, decode_real_safe, uint8_t *, uint32_t, float *);
DEFINE_FAKE_VALUE_FUNC(int, decode_real, uint8_t *, float *);
DEFINE_FAKE_VALUE_FUNC(int, encode_bacnet_real, float, uint8_t *);
DEFINE_FAKE_VALUE_FUNC(int, decode_double, uint8_t *, double *);
DEFINE_FAKE_VALUE_FUNC(int, decode_double_safe, uint8_t *, uint32_t, double *);
DEFINE_FAKE_VALUE_FUNC(int, encode_bacnet_double, double, uint8_t *);

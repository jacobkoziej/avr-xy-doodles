// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * sinpi.h -- sin(x * pi) lookup table
 * Copyright (C) 2025  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef AVR_XY_DOODLES_SINPI_H
#define AVR_XY_DOODLES_SINPI_H

#include <stdint.h>

extern const uint8_t sinpi_lookup_table[];

int16_t sinpi(const int16_t x);

#endif  // AVR_XY_DOODLES_SINPI_H

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * fixed-point.h -- fixed-point
 * Copyright (C) 2025  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef AVR_XY_DOODLES_FIXED_POINT_H
#define AVR_XY_DOODLES_FIXED_POINT_H

#include <stdint.h>

typedef union ifp {
	int16_t i16;

	struct {
		uint8_t f8;
		int8_t  i8;
	};
} ifp_t;

typedef union ufp {
	uint16_t u16;

	struct {
		uint8_t f8;
		uint8_t u8;
	};
} ufp_t;

ifp_t ifp_sinpi(const ifp_t x);
ufp_t ufp_round(const ufp_t x);

#endif  // AVR_XY_DOODLES_FIXED_POINT_H

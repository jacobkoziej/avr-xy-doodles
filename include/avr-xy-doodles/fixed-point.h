// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * fixed-point.h -- fixed-point
 * Copyright (C) 2025  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef AVR_XY_DOODLES_FIXED_POINT_H
#define AVR_XY_DOODLES_FIXED_POINT_H

#include <stdint.h>

#define FP_STATIC_(type, x) {.type##16 = (x)}
#define FP_STATIC(type, x)  FP_STATIC_(type, (((double) (x)) * (1 << 8)))
#define FP_(type, x)        ((type##fp_t) FP_STATIC_(type, x))
#define FP(type, x)         FP_(type, (((double) (x)) * (1 << 8)))
#define FP_RAW(type, x)     FP_(type, x)
#define FP_INT_(type, x)    ((type##fp_t) {.type##8 = (x)})
#define FP_INT(type, x)     FP_INT_(type, x)
#define FP_EPS(type)        FP_(type, 1)

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

ufp_t ifp_abs(const ifp_t x);
ifp_t ifp_cospi(const ifp_t x);
ifp_t ifp_mod(const ifp_t x, const int8_t n);
ifp_t ifp_sinpi(const ifp_t x);
ifp_t ifp_sub(const ifp_t a, const ifp_t b);
ufp_t ifp2ufp(const ifp_t x);
ufp_t ufp_add(const ufp_t a, const ufp_t b);
ufp_t ufp_div(const ufp_t a, const ufp_t b);
ufp_t ufp_max(const ufp_t a, const ufp_t b);
ufp_t ufp_min(const ufp_t a, const ufp_t b);
ufp_t ufp_mul(const ufp_t a, const ufp_t b);
ufp_t ufp_round(const ufp_t x);
ufp_t ufp_sub(const ufp_t a, const ufp_t b);

#endif  // AVR_XY_DOODLES_FIXED_POINT_H

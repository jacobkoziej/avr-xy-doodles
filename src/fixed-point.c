// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * fixed-point.c -- fixed-point functionality
 * Copyright (C) 2025  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <avr-xy-doodles/fixed-point.h>
#include <avr-xy-doodles/private/fixed-point.h>

#include <stdint.h>

ufp_t ifp_abs(const ifp_t x)
{
	const int16_t i16      = x.i16;
	const bool    negative = i16 < 0;

	return (ufp_t) {.u16 = (uint16_t) (negative ? -i16 : i16)};
}

ifp_t ifp_cospi(const ifp_t x)
{
	const ifp_t half = FP_RAW(i, 1 << 7);

	return ifp_sinpi(ifp_sub(x, half));
}

ifp_t ifp_mod(const ifp_t x, const int8_t n)
{
	return (ifp_t) {.i16 = x.i16 % FP_INT(i, n).i16};
}

ifp_t ifp_sinpi(const ifp_t x)
{
	const uint8_t index = x.f8;

	ifp_t y = {.f8 = sinpi_lookup_table[index]};

	const bool negative = x.i8 % 2;

	if (negative) y.i16 = -y.i16;

	return y;
}

ifp_t ifp_sub(const ifp_t a, const ifp_t b)
{
	return (ifp_t) {.i16 = a.i16 - b.i16};
}

ufp_t ifp2ufp(const ifp_t x)
{
	return (ufp_t) {.u16 = (uint16_t) x.i16};
}

ufp_t ufp_add(const ufp_t a, const ufp_t b)
{
	return (ufp_t) {.u16 = a.u16 + b.u16};
}

ufp_t ufp_max(const ufp_t a, const ufp_t b)
{
	return a.u16 > b.u16 ? a : b;
}

ufp_t ufp_mul(const ufp_t a, const ufp_t b)
{
	const uint32_t c = ((uint32_t) a.u16) * ((uint32_t) b.u16);

	return (ufp_t) {.u16 = (c >> 8) & 0xFFFF};
}

ufp_t ufp_round(const ufp_t x)
{
	const uint8_t u8    = x.u8;
	const bool    round = x.f8 & (1 << 7);

	return (ufp_t) {.u8 = (uint8_t) (u8 + round)};
}

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * fixed-point.c -- fixed-point functionality
 * Copyright (C) 2025  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <avr-xy-doodles/fixed-point.h>
#include <avr-xy-doodles/private/fixed-point.h>

#include <stdint.h>

ifp_t ifp_sinpi(const ifp_t x)
{
	const uint8_t index = x.f8;

	ifp_t y = {.f8 = sinpi_lookup_table[index]};

	const bool negative = x.i8 % 2;

	if (negative) y.i16 = -y.i16;

	return y;
}

ufp_t ufp_round(const ufp_t x)
{
	const uint8_t u8    = x.u8;
	const bool    round = x.f8 & (1 << 7);

	return (ufp_t) {.u8 = (uint8_t) (u8 + round)};
}

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * sinpi.c -- sin(x * pi) lookup table
 * Copyright (C) 2025  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <avr-xy-doodles/sinpi.h>

int16_t sinpi(const int16_t x)
{
	const uint8_t index = (uint8_t) x & UINT8_MAX;

	int16_t value = sinpi_lookup_table[index];

	const bool negative = x & (1 << 8);

	if (negative) value = -value;

	return value;
}

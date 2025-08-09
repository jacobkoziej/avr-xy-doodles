// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * display.h -- display functionality
 * Copyright (C) 2025  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef AVR_XY_DOODLES_PRIVATE_DISPLAY_H
#define AVR_XY_DOODLES_PRIVATE_DISPLAY_H

#include <avr/io.h>

#include <avr-xy-doodles/display.h>
#include <avr-xy-doodles/shape.h>

#include <stdint.h>

typedef void(draw_shape_t)(const shape_t * const);

typedef union point {
	uint16_t u16;

	struct {
		uint8_t subpixel;
		uint8_t pixel;
	};
} point_t;

// clang-format off
static const uint8_t CHANNEL_X_PORTD_BITS
	= (1 << PD0)
	| (1 << PD1)
	| (1 << PD2)
	| (1 << PD3)
	| (1 << PD4)
	| (1 << PD5)
	| (1 << PD6)
	| (1 << PD7);
static const uint8_t CHANNEL_Y_PORTB_BITS
	= (1 << PB0)
	| (1 << PB1)
	| (1 << PB2)
	| (1 << PB3)
	| (1 << PB4)
	| (1 << PB5);
static const uint8_t CHANNEL_Y_PORTC_BITS
	= (1 << PC4)
	| (1 << PC5);
static const uint8_t CHANNEL_Z_PORTC_BITS
	= (1 << PC0)
	| (1 << PC1)
	| (1 << PC2)
	| (1 << PC3);
// clang-format on

static void disable_cursor(void);
static void draw_line(const shape_t * const shape);
static void render_pixel(const uint8_t x, const uint8_t y, const uint8_t z);
static bool round_subpixel(const uint8_t subpixel);

static draw_shape_t * const SHAPE_TO_DRAW[SHAPE_TOTAL] = {
	[SHAPE_LINE] = draw_line,
};

#endif  // AVR_XY_DOODLES_PRIVATE_DISPLAY_H

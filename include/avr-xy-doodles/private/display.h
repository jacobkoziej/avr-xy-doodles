// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * display.h -- display functionality
 * Copyright (C) 2025  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef AVR_XY_DOODLES_PRIVATE_DISPLAY_H
#define AVR_XY_DOODLES_PRIVATE_DISPLAY_H

#include <avr/io.h>

#include <avr-xy-doodles/display.h>
#include <avr-xy-doodles/fixed-point.h>
#include <avr-xy-doodles/shape.h>

#include <stdint.h>

typedef void(draw_shape_t)(const shape_t * const);

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
static void draw_arc(const shape_t * const shape);
static void draw_circle(const shape_t * const shape);
static void draw_line(const shape_t * const shape);
static void draw_poly(const shape_t * const shape);
static void render_pixel(const uint8_t x, const uint8_t y, const uint8_t z);

static draw_shape_t * const SHAPE_TO_DRAW[SHAPE_TOTAL] = {
	[SHAPE_ARC]    = draw_arc,
	[SHAPE_CIRCLE] = draw_circle,
	[SHAPE_LINE]   = draw_line,
	[SHAPE_POLY]   = draw_poly,
};

#endif  // AVR_XY_DOODLES_PRIVATE_DISPLAY_H

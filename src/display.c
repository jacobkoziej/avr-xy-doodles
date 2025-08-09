// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * display.c -- display functionality
 * Copyright (C) 2025  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <avr/io.h>

#include <avr-xy-doodles/display.h>
#include <avr-xy-doodles/private/display.h>
#include <avr-xy-doodles/shape.h>

#include <limits.h>
#include <stdint.h>

#define MAX(x, y) ((x) <= (y) ? (y) : (x))

void display_init(void)
{
	DDRB |= CHANNEL_Y_PORTB_BITS;
	DDRC |= CHANNEL_Y_PORTC_BITS | CHANNEL_Z_PORTC_BITS;
	DDRD |= CHANNEL_X_PORTD_BITS;

	PORTB &= (uint8_t) ~CHANNEL_Y_PORTB_BITS;
	PORTC &= (uint8_t) ~(CHANNEL_Y_PORTC_BITS | CHANNEL_Z_PORTC_BITS);
	PORTD &= (uint8_t) ~CHANNEL_X_PORTD_BITS;
}

void display_shape(const shape_t * const shape)
{
	draw_shape_t * const draw_shape = SHAPE_TO_DRAW[*shape];

	if (!draw_shape) return;

	draw_shape(shape);
}

static void disable_cursor(void)
{
	PORTC &= (uint8_t) ~CHANNEL_Z_PORTC_BITS;
}

static void draw_line(const shape_t * const shape)
{
	const shape_line_t * const line = SHAPE_OFFSETOF(shape, shape_line_t);

	const point_t x1 = (point_t) {.pixel = line->x1};
	const point_t y1 = (point_t) {.pixel = line->y1};
	const point_t x2 = (point_t) {.pixel = line->x2};
	const point_t y2 = (point_t) {.pixel = line->y2};

	uint16_t width  = (uint16_t) (x2.pixel - x1.pixel) << 8;
	uint16_t height = (uint16_t) (y2.pixel - y1.pixel) << 8;

	if (x1.u16 > x2.u16) width = -width;

	if (y1.u16 > y2.u16) height = -height;

	const size_t steps = MAX(width, height) >> 8;

	uint16_t dx = width / steps;
	uint16_t dy = height / steps;

	if (x1.u16 > x2.u16) dx = -dx;

	if (y1.u16 > y2.u16) dy = -dy;

	point_t x = x1;
	point_t y = y1;

	for (size_t i = 0; i < steps; i++) {
		// clang-format off
		render_pixel(
			(uint8_t) (x.pixel + round_subpixel(x.subpixel)),
			(uint8_t) (y.pixel + round_subpixel(y.subpixel)),
			UINT8_MAX);
		// clang-format on

		x.u16 += dx;
		y.u16 += dy;
	}

	disable_cursor();
}

static void draw_poly(const shape_t * const shape)
{
	const shape_poly_t * const poly = SHAPE_OFFSETOF(shape, shape_poly_t);

	const size_t size = poly->size;

	if (!size) return;

	const size_t lines = size + poly->polygon - 1;

	const shape_point_t * const points = poly->points;

	for (size_t i = 0; i < lines; i++) {
		const shape_point_t * const point1 = points + i;
		const shape_point_t * const point2 = points + ((i + 1) % size);

		const shape_line_t line = (shape_line_t) {
			.shape = SHAPE_LINE,
			.x1    = (*point1)[SHAPE_POINT_X],
			.y1    = (*point1)[SHAPE_POINT_Y],
			.x2    = (*point2)[SHAPE_POINT_X],
			.y2    = (*point2)[SHAPE_POINT_Y],
		};

		draw_line(&line.shape);
	}
}

static void render_pixel(const uint8_t x, const uint8_t y, const uint8_t z)
{
	const uint8_t portb
		= (uint8_t) (y >> __builtin_popcount(CHANNEL_Y_PORTC_BITS))
		& CHANNEL_Y_PORTB_BITS;

	const uint8_t portc
		= (uint8_t) ((y << __builtin_popcount(CHANNEL_Z_PORTC_BITS))
			  & CHANNEL_Y_PORTC_BITS)
		| (z & CHANNEL_Z_PORTC_BITS);

	const uint8_t portd = x & CHANNEL_X_PORTD_BITS;

	PORTC = portc;
	PORTB = portb;
	PORTD = portd;
}

static bool round_subpixel(const uint8_t subpixel)
{
	return subpixel & (1 << 7);
}

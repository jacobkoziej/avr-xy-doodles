// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * display.c -- display functionality
 * Copyright (C) 2025  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <avr/io.h>

#include <avr-xy-doodles/display.h>
#include <avr-xy-doodles/fixed-point.h>
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

static void draw_arc(const shape_t * const shape)
{
	const shape_arc_t * const arc = SHAPE_OFFSETOF(shape, shape_arc_t);

	const ufp_t rx = arc->rx;
	const ufp_t ry = arc->ry;

	const ufp_t r = ufp_max(rx, ry);

	if (!r.u16) return;

	const ufp_t cx = arc->cx;
	const ufp_t cy = arc->cy;
	const ifp_t t0 = arc->t0;
	const ifp_t t1 = arc->t1;

	ufp_t total = ifp_abs(ifp_mod(ifp_sub(t1, t0), 2));

	if (!total.u16 && (t0.i16 != t1.i16))
		total = ifp2ufp(ifp_sub(FP_INT(i, 2), FP_EPS(i)));

	const size_t quadrants = total.u16 >> 7;

	const ufp_t partial_r
		= ufp_mul(FP_RAW(u, total.u16 & ((1U << 7) - 1U)), r);

	const size_t steps = (r.u8 * quadrants) + ufp_round(partial_r).u8;

	// Since we only draw from 0 to (2 - eps), dividing by a large
	// steps value will yeild a dt that is too coarse. We get around
	// this by increasing the number of fractional bits.
	const uint8_t HD_SHIFT = 6;

	const int16_t dt = (int16_t) ((total.u16 << HD_SHIFT) / steps);

	int16_t t_hd = t0.i16 << HD_SHIFT;

	for (size_t i = 0; i < steps; i++) {
		const ifp_t t = FP_RAW(i, t_hd >> HD_SHIFT);
		const ufp_t c = ifp2ufp(ifp_cospi(t));
		const ufp_t s = ifp2ufp(ifp_sinpi(t));

		const ufp_t x = ufp_add(cx, ufp_mul(rx, c));
		const ufp_t y = ufp_add(cy, ufp_mul(ry, s));

		render_pixel(ufp_round(x).u8, ufp_round(y).u8, UINT8_MAX);

		t_hd += dt;
	}

	disable_cursor();
}

static void draw_line(const shape_t * const shape)
{
	const shape_line_t * const line = SHAPE_OFFSETOF(shape, shape_line_t);

	const ufp_t x1 = line->x1;
	const ufp_t y1 = line->y1;
	const ufp_t x2 = line->x2;
	const ufp_t y2 = line->y2;

	uint16_t width  = (uint16_t) (x2.u8 - x1.u8) << 8;
	uint16_t height = (uint16_t) (y2.u8 - y1.u8) << 8;

	if (x1.u16 > x2.u16) width = -width;

	if (y1.u16 > y2.u16) height = -height;

	const size_t steps = MAX(width, height) >> 8;

	uint16_t dx = width / steps;
	uint16_t dy = height / steps;

	if (x1.u16 > x2.u16) dx = -dx;

	if (y1.u16 > y2.u16) dy = -dy;

	ufp_t x = x1;
	ufp_t y = y1;

	for (size_t i = 0; i < steps; i++) {
		render_pixel(ufp_round(x).u8, ufp_round(y).u8, UINT8_MAX);

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

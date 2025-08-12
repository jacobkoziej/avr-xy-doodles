// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * shape.h -- basic shapes
 * Copyright (C) 2025  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef AVR_XY_DOODLES_SHAPE_H
#define AVR_XY_DOODLES_SHAPE_H

#include <avr-xy-doodles/fixed-point.h>

#include <stddef.h>
#include <stdint.h>

#define SHAPE_OFFSETOF(s, type) \
	((type *) (((uintptr_t) s) - offsetof(type, shape)))

typedef enum shape {
	SHAPE_ARC,
	SHAPE_RECT,
	SHAPE_CIRCLE,
	SHAPE_ELLIPSE,
	SHAPE_LINE,
	SHAPE_POLY,
	SHAPE_TOTAL,
} shape_t;

enum {
	SHAPE_POINT_X,
	SHAPE_POINT_Y,
	SHAPE_POINT_TOTAL,
};

typedef uint8_t shape_point_t[SHAPE_POINT_TOTAL];

typedef struct shape_arc {
	shape_t shape;
	ufp_t   cx;
	ufp_t   cy;
	ufp_t   rx;
	ufp_t   ry;
	ifp_t   t0;
	ifp_t   t1;
} shape_arc_t;

typedef struct shape_rect {
	shape_t shape;
	uint8_t x;
	uint8_t y;
	uint8_t width;
	uint8_t height;
	uint8_t rx;
	uint8_t ry;
} shape_rect_t;

typedef struct shape_circle {
	shape_t shape;
	uint8_t cx;
	uint8_t cy;
	uint8_t r;
} shape_circle_t;

typedef struct shape_ellipse {
	shape_t shape;
	uint8_t cx;
	uint8_t cy;
	uint8_t rx;
	uint8_t ry;
} shape_ellipse_t;

typedef struct shape_line {
	shape_t shape;
	uint8_t x1;
	uint8_t y1;
	uint8_t x2;
	uint8_t y2;
} shape_line_t;

typedef struct shape_poly {
	shape_t               shape;
	bool                  polygon;
	size_t                size;
	shape_point_t * const points;
} shape_poly_t;

#endif  // AVR_XY_DOODLES_SHAPE_H

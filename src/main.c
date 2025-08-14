// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * main.c -- waveform doodles, the AVR way
 * Copyright (C) 2025  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include <avr-xy-doodles/display.h>
#include <avr-xy-doodles/doodle.h>
#include <avr-xy-doodles/main.h>
#include <avr-xy-doodles/shape.h>
#include <avr-xy-doodles/timer.h>

#include <alloca.h>
#include <stdlib.h>

int main(void)
{
	sei();

	timer_init();
	display_init();

repeat:
	const doodle_t * const *doodles_pgm = doodles;
	const doodle_t         *doodle_pgm;

	while (memcpy_P(&doodle_pgm, doodles_pgm++, sizeof(doodle_pgm)),
		doodle_pgm)
		render_doodle(doodle_pgm);

	goto repeat;

	return EXIT_SUCCESS;
}

void render_doodle(const doodle_t * const doodle_pgm)
{
	doodle_t doodle;
	memcpy_P(&doodle, doodle_pgm, sizeof(doodle));

	const shape_t *shapes[doodle.size];
	memcpy_P(shapes, doodle.shapes, sizeof(shapes));

	static const size_t SHAPE_TO_SIZE[SHAPE_TOTAL] = {
		[SHAPE_ARC]     = sizeof(shape_arc_t),
		[SHAPE_RECT]    = sizeof(shape_rect_t),
		[SHAPE_CIRCLE]  = sizeof(shape_circle_t),
		[SHAPE_ELLIPSE] = sizeof(shape_ellipse_t),
		[SHAPE_LINE]    = sizeof(shape_line_t),
		[SHAPE_POLY]    = sizeof(shape_poly_t),
	};

	for (size_t i = 0; i < doodle.size; i++) {
		const shape_t * const shape_pgm = shapes[i];

		shape_t shape;
		memcpy_P(&shape, shape_pgm, sizeof(shape));

		if ((shape < 0) || (shape >= SHAPE_TOTAL)) return;

		size_t shape_size = SHAPE_TO_SIZE[shape];

		void * const shape_buffer = alloca(shape_size);
		memcpy_P(shape_buffer, shape_pgm, shape_size);

		shapes[i] = shape_buffer;

		if (shape == SHAPE_POLY) {
			shape_poly_t * const poly = shape_buffer;

			size_t points_size
				= poly->size * sizeof(shape_point_t);

			shape_point_t * const points = alloca(points_size);
			memcpy_P(points, poly->points, points_size);

			poly->points = points;
		}
	}

	doodle.shapes = shapes;

	const uint32_t frame_start_ms = timer_ms();

	while ((timer_ms() - frame_start_ms) < doodle.duration)
		for (size_t j = 0; j < doodle.size; j++)
			display_shape(doodle.shapes[j]);
}

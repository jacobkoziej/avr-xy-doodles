// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * main.c -- waveform doodles, the AVR way
 * Copyright (C) 2025  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <avr/interrupt.h>

#include <avr-xy-doodles/display.h>
#include <avr-xy-doodles/doodle.h>
#include <avr-xy-doodles/shape.h>
#include <avr-xy-doodles/timer.h>

#include <stdlib.h>

int main(void)
{
	sei();

	timer_init();
	display_init();

repeat:
	for (size_t i = 0; i < doodle_count; i++) {
		const doodle_t * const doodle = doodles[i];

		const uint32_t frame_start_ms = timer_ms();

		while ((timer_ms() - frame_start_ms) < doodle->duration)
			for (size_t j = 0; j < doodle->size; j++)
				display_shape(doodle->shapes[j]);
	}

	goto repeat;

	return EXIT_SUCCESS;
}

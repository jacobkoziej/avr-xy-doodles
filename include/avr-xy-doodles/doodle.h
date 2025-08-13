// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * doodle.h -- doodles
 * Copyright (C) 2025  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef AVR_XY_DOODLES_DOODLE_H
#define AVR_XY_DOODLES_DOODLE_H

#include <avr-xy-doodles/shape.h>

#include <stddef.h>
#include <stdint.h>

typedef struct doodle {
	const shape_t **shapes;
	size_t          size;
	uint32_t        duration;
} doodle_t;

extern const doodle_t * const doodles[];
extern const size_t           doodle_count;

#endif  // AVR_XY_DOODLES_DOODLE_H

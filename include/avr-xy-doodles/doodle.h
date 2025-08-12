// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * doodle.h -- doodles
 * Copyright (C) 2025  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef AVR_XY_DOODLES_DOODLE_H
#define AVR_XY_DOODLES_DOODLE_H

#include <avr-xy-doodles/shape.h>

#include <stdint.h>

typedef struct doodle {
	shape_t *shapes;
	uint32_t duration;
} doodle_t;

#endif  // AVR_XY_DOODLES_DOODLE_H

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * display.h -- display functionality
 * Copyright (C) 2025  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef AVR_XY_DOODLES_DISPLAY_H
#define AVR_XY_DOODLES_DISPLAY_H

#include <avr-xy-doodles/shape.h>

void display_init(void);
void display_shape(const shape_t * const shape);

#endif  // AVR_XY_DOODLES_DISPLAY_H

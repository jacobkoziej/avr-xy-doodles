// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * timer.h -- timer functionality
 * Copyright (C) 2025  Jacob Koziej <jacobkoziej@gmail.com>
 */

#ifndef AVR_XY_DOODLES_TIMER_H
#define AVR_XY_DOODLES_TIMER_H

#include <stdint.h>

void     timer_init(void);
uint32_t timer_ms(void);

#endif  // AVR_XY_DOODLES_TIMER_H

// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * fuses.c -- fuse configuration
 * Copyright (C) 2025  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <avr/io.h>

#ifdef NDEGUB
FUSES = {
	.low      = 0xFF,
	.high     = FUSE_BOOTRST & FUSE_SPIEN,
	.extended = FUSE_BODLEVEL1,
};
#endif

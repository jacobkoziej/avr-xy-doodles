// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * main.c -- waveform doodles, the AVR way
 * Copyright (C) 2025  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <avr-xy-doodles/display.h>

#include <stdlib.h>

int main(void)
{
	display_init();

	return EXIT_SUCCESS;
}

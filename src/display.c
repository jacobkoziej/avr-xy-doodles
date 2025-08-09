// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * display.c -- display functionality
 * Copyright (C) 2025  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <avr/io.h>

#include <avr-xy-doodles/display.h>
#include <avr-xy-doodles/private/display.h>

void display_init(void)
{
	DDRB |= CHANNEL_Y_PORTB_BITS;
	DDRC |= CHANNEL_Y_PORTC_BITS | CHANNEL_Z_PORTC_BITS;
	DDRD |= CHANNEL_X_PORTD_BITS;

	PORTB &= (uint8_t) ~CHANNEL_Y_PORTB_BITS;
	PORTC &= (uint8_t) ~(CHANNEL_Y_PORTC_BITS | CHANNEL_Z_PORTC_BITS);
	PORTD &= (uint8_t) ~CHANNEL_X_PORTD_BITS;
}

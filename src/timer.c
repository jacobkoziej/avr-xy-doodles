// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * timer.c -- timer functionality
 * Copyright (C) 2025  Jacob Koziej <jacobkoziej@gmail.com>
 */

#include <avr/interrupt.h>
#include <avr/io.h>

#include <avr-xy-doodles/timer.h>

#include <assert.h>
#include <limits.h>
#include <stdint.h>

#define MS_OVERFLOW (F_CPU / 64 / 1000)

static_assert(MS_OVERFLOW <= UINT8_MAX,
	"F_CPU too high for TIMER0 millisecond overflow.");

static volatile uint32_t ms_count;

void timer_init(void)
{
	// reset timer
	TCCR0B = 0;
	TIFR0  = 0xFF;
	TIMSK0 = 0;
	TCNT0  = 0;

	OCR0A = MS_OVERFLOW;

	ms_count = 0;

	// clang-format off
	TCCR0A
		= (0 << COM0A1)
		| (0 << COM0A0)
		| (0 << COM0B1)
		| (0 << COM0B0)
		| (1 << WGM01)
		| (0 << WGM00);

	TCCR0B
		= (0 << FOC0A)
		| (0 << FOC0B)
		| (0 << WGM02)
		| (0 << CS02)
		| (1 << CS01)
		| (1 << CS00);
	// clang-format on

	TIMSK0 = (0 << OCIE0B) | (1 << OCIE0A) | (0 << TOIE0);
}

uint32_t timer_ms(void)
{
	const uint8_t sreg = SREG;

	cli();

	uint32_t ms = ms_count;

	const uint8_t tcnt0 = TCNT0;

	const bool pending_isr = (TIFR0 & (1 << OCF1A)) && (tcnt0 < UINT8_MAX);

	SREG = sreg;

	return ms + pending_isr;
}

ISR(TIMER0_COMPA_vect)
{
	++ms_count;
}

/**
 * libps2
 * Copyright (C) 2015 David Jolly
 * ----------------------
 *
 * libps2 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libps2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include  "../lib/include/ps2.h"

// TODO: Debug

ISR(INT0_vect)
{
	// TODO
}

void 
int0_initialize(void)
{
	TRACE_ENTRY();

	EIMSK |= _BV(INT0);
	EICRA |= (_BV(ISC01) & ~_BV(ISC00));
	sei();

	TRACE_EXIT();
}

void 
int0_uninitialize(void)
{
	TRACE_ENTRY();

	cli();
	EICRA &= ~_BV(ISC01);
	EIMSK &= ~_BV(INT0);

	TRACE_EXIT();
}

// ---

int 
main(void) 
{
	int result = 0;

	TRACE_INITIALIZE();
	TRACE_ENTRY();

	// TODO

	int0_initialize();

	DDRD &= ~_BV(PD3);
	PORTD &= ~_BV(PD3);

	while(1) { _delay_ms(1000); }

	int0_uninitialize();

	// ---

	TRACE_EXIT_MESSAGE("Return Value: 0x%08x", result);
	return result;
}

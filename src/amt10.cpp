/* Copyright (C) 2019 Enrico Rossi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include <avr/interrupt.h>
#include "amt10.h"

// Globals for IRQ usage
volatile long int AMT10::steps { 0 };

/*! IRQ wakes up when chip send an IRQ. */
ISR(INT0_vect)
{
	if (PIND & (1 << PIND3))
		AMT10::steps++;
	else
		AMT10::steps--;
}

void AMT10::resume()
{
	// Trigger irq on rising edge of the INT0 pin
	EICRA |= _BV(ISC01 | ISC00);

	// Enable irq0
	EIMSK |= _BV(INT0);
}

void AMT10::suspend()
{
	// disable irq0
	EIMSK &= ~_BV(INT0);

	// disable triggers
	EICRA &= ~(_BV(ISC01) | _BV(ISC00));
}

void AMT10::clear()
{
	steps = 0;
}

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
#include "amt10b.h"

// Globals for IRQ usage
volatile long int AMT10::steps { 0 };
volatile bool AMT10::flag { false };

/*! IRQ wakes up when chip send an IRQ. */
ISR(INT0_vect)
{
	if (AMT10::flag) {
		if (PIND & (1 << PIND3))
			AMT10::steps++;
		else
			AMT10::steps--;

		AMT10::flag = false;
	}
}

//
ISR(INT1_vect)
{
	AMT10::flag = true;
}

void AMT10::resume()
{
	// Trigger irq on rising edge of the INTn PIN
	EICRA |= (1 << ISC01) | (1 << ISC00);

	// Enable irq0
	EIMSK |= (1 << INT1) | (1 << INT0);
}

void AMT10::suspend()
{
	// disable irq 1 and irq0
	EIMSK &= ~((1 << INT1) | (1 << INT0));

	// disable triggers
	EICRA &= ~((1 << ISC01) | (1 << ISC00));
}

void AMT10::clear()
{
	steps = 0;
}

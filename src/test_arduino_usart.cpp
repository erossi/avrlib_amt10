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

#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "usart_base.h"
#include "amt10.h"

// Allocate the usart static object
Usart0_Base usart;

int main(void)
{
	long int oldcount {0};
	char buffer[10];
	uint8_t i {0};

	// start the usart port
	usart.resume();

	// ena IRQ
	sei();

	// start the encoder
	AMT10::resume();

	while (true) {
		if (oldcount != AMT10::steps) {
			oldcount = AMT10::steps;
			i = 0;
			sprintf(buffer, "%8li\n", oldcount);

			while (buffer[i]) {
				usart.put(buffer[i]);
				i++;
			}
		}
	}

	return(0);
}

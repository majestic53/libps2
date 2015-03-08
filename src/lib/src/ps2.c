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

#include "../include/ps2.h"

#define CHAR_INVALID '\0'
#define CODE_BREAK 0xf0
#define CODE_EXTEND 0xe0

// TODO: add character maps

void 
ps2_keybuf_initialize(
	__inout ps2_keybuf_t *context,
	__in uint8_t capacity
	)
{
	if(context) {
		context->capacity = capacity;
		context->count = 0;
		context->read = 0;
		context->write = 0;
	}
}

uint8_t 
ps2_keybuf_read(
	__inout ps2_keybuf_t *context
	)
{
	uint8_t result = 0;

	if(context && context->count) {
		result = context->data[context->read++];
		--context->count;
	}

	return result;
}

void 
ps2_keybuf_write(
	__inout ps2_keybuf_t *context,
	__in uint8_t input
	)
{
	if(context && (context->count < KEYBUF_CAP_MAX)) {
		context->data[context->write++] = input;
		++context->count;
	}
}

void 
_ps2_initialize(
	__inout ps2_t *context,
	__in volatile uint8_t *ddr_clock,
	__in volatile uint8_t *port_clock,
	__in uint8_t pin_clock,
	__in volatile uint8_t *ddr_data,
	__in volatile uint8_t *port_data,
	__in uint8_t pin_data,
	__in uint8_t capacity
	)
{
	if(context && ddr_clock && port_clock 
			&& ddr_data && port_data) {
		context->comm.ddr_clock = ddr_clock;
		context->comm.ddr_data = ddr_data;
		context->comm.pin_clock = pin_clock;
		context->comm.pin_data = pin_data;
		context->comm.port_clock = port_clock;
		context->comm.port_data = port_data;
		context->state.caps = 0;
		context->state.control = 0;
		context->state.shift = 0;
		ps2_keybuf_initialize(&context->buffer, capacity);

		// TODO
	}
}

void 
ps2_uninitialize(
	__inout ps2_t *context
	)
{
	if(context) {

		// TODO

	}
}

char 
ps2_read(
	__inout ps2_t *context
	)
{
	char result = CHAR_INVALID;

	if(context) {

		// TODO

	}

	return result;
}

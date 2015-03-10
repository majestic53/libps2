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

#include <avr/io.h>
#include "../include/ps2.h"

#ifndef NDEBUG
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#define BAUD 9600
#include <util/setbaud.h>

#define TRACE_BUF_LEN_MAX 0xff
#define TRACE_MSG_ERR "(Error)"

static char trace_buf[TRACE_BUF_LEN_MAX];

void 
trace_initialize(void)
{
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
#if USE_2X
	UCSR0A |= _BV(U2X0);
#else
	UCSR0A &= ~_BV(U2X0);
#endif // USE_2X
	UCSR0B = _BV(TXEN0);
	UCSR0C = (_BV(UCSZ00) | _BV(UCSZ01));
}

void 
trace_character(
	__in char input
	)
{
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = input;
}

void 
trace_message(	
	__in const char *file,
	__in const char *line,
	__in const char *funct,
	__in const char *prefix,
	__in const char *format,
	...
	)
{
	va_list args;
	int iter, len;
	char *buf = trace_buf;

	if(prefix) {

		for(iter = 0; iter < strlen(prefix); ++iter) {
			trace_character(prefix[iter]);
		}
	}

	if(funct) {

		for(iter = 0; iter < strlen(funct); ++iter) {
			trace_character(funct[iter]);
		}

		trace_character(':');
	}

	if(file || line) {
		trace_character('(');

		if(file) {

			for(iter = 0; iter < strlen(file); ++iter) {
				trace_character(file[iter]);
			}

			trace_character(':');
		}

		if(line) {

			for(iter = 0; iter < strlen(line); ++iter) {
				trace_character(line[iter]);
			}
		}

		trace_character(')');
		trace_character(' ');
	}

	if(format) {
		va_start(args, format);
		len = vsnprintf(buf, TRACE_BUF_LEN_MAX, format, args);
		va_end(args);

		if(len < 0) {
			buf = TRACE_MSG_ERR;
			len = strlen(TRACE_MSG_ERR);
		}

		if(len > 0) {

			for(iter = 0; iter < len; ++iter) {
				trace_character(buf[iter]);
			}

			trace_character(' ');
		}
	}

	trace_character('\r');
	trace_character('\n');
}

#endif // NDEBUG

#define CHAR_INVALID '\0'
#define CODE_BREAK 0xf0
#define CODE_EXTEND 0xe0

typedef enum _ps2_rdstate_t {
        READ_STATE_START = 0,
        READ_STATE_DATA_0,
        READ_STATE_DATA_1,
        READ_STATE_DATA_2,
        READ_STATE_DATA_3,
        READ_STATE_DATA_4,
        READ_STATE_DATA_5,
        READ_STATE_DATA_6,
        READ_STATE_DATA_7,
        READ_STATE_PARITY,
        READ_STATE_STOP,
} ps2_rdstate_t;

typedef struct __attribute__((__packed__)) _ps2_rddata_t {
        uint8_t b0: 1;
        uint8_t b1: 1;
        uint8_t b2: 1;
        uint8_t b3: 1;
        uint8_t b4: 1;
        uint8_t b5: 1;
        uint8_t b6: 1;
        uint8_t b7: 1;
} ps2_rddata_t;

typedef struct _ps2_rdcont_t {
        union {
                ps2_rddata_t bit;
                uint8_t byte;
        } data;
        uint8_t parity;
        ps2_rdstate_t state;
} ps2_rdcont_t;

static volatile ps2_rdcont_t rdcont;

void 
ps2_keybuf_initialize(
	__inout ps2_keybuf_t *context,
	__in uint8_t capacity
	)
{
	TRACE_ENTRY();

	if(context) {
		context->capacity = capacity;
		context->count = 0;
		context->read = 0;
		context->write = 0;
	} else {
		TRACE_EVENT("Initialization FAILED!");
	}

	TRACE_EXIT();
}

uint8_t 
ps2_keybuf_read(
	__inout ps2_keybuf_t *context
	)
{
	uint8_t result = 0;

	TRACE_ENTRY();

	if(context && context->count) {
		result = context->data[context->read++];
		--context->count;
	} else {
		TRACE_EVENT("Read FAILED!");
	}

	TRACE_EXIT_MESSAGE("Return Value: 0x%02x", result);
	return result;
}

void 
ps2_keybuf_write(
	__inout ps2_keybuf_t *context,
	__in uint8_t input
	)
{
	TRACE_ENTRY();

	if(context && (context->count < KEYBUF_CAP_MAX)) {
		context->data[context->write++] = input;
		++context->count;
	} else {
		TRACE_EVENT("Write FAILED!");
	}

	TRACE_EXIT();
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
	TRACE_ENTRY();

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

	} else {
		TRACE_EVENT("Initialization FAILED!");
	}

	TRACE_EXIT();
}

void 
ps2_uninitialize(
	__inout ps2_t *context
	)
{
	TRACE_ENTRY();

	if(context && context->comm.ddr_clock && context->comm.port_clock 
			&& context->comm.ddr_data && context->comm.port_data) {

		// TODO

	} else {
		TRACE_EVENT("Uninitialization FAILED!");
	}

	TRACE_EXIT();
}

char 
ps2_read(
	__inout ps2_t *context
	)
{
	char result = CHAR_INVALID;

	TRACE_ENTRY();

	if(context) {

		// TODO

	} else {
		TRACE_EVENT("Read FAILED!");
	}

	TRACE_EXIT_MESSAGE("\'%c\' (0x%02x)", isprint(result) 
			? result : ' ', result);
	return result;
}

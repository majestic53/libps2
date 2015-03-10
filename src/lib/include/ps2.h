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

#ifndef LIBPS2_H_
#define LIBPS2_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef __in
#define __in
#endif // __in
#ifndef __out
#define __out
#endif // __out
#ifndef __inout
#define __inout
#endif // __inout

//#define NDEBUG
#ifndef NDEBUG
#define _CAT_STR(_STR_) # _STR_
#define CAT_STR(_STR_) _CAT_STR(_STR_)
#define _TRACE_MESSAGE(_PRE_, _FORM_, ...) \
	trace_message(__FILE__, CAT_STR(__LINE__), __func__, _PRE_, _FORM_, __VA_ARGS__)
#define TRACE_ENTRY() _TRACE_MESSAGE("+", "", "")
#define TRACE_ENTRY_MESSAGE(_FORM_, ...) _TRACE_MESSAGE("+", _FORM_, __VA_ARGS__)
#define TRACE_EVENT(_EVT_) _TRACE_MESSAGE("", _EVT_, "")
#define TRACE_EVENT_MESSAGE(_FORM_, ...) _TRACE_MESSAGE("", _FORM_, __VA_ARGS__)
#define TRACE_EXIT() _TRACE_MESSAGE("-", "", "")
#define TRACE_EXIT_MESSAGE(_FORM_, ...) _TRACE_MESSAGE("-", _FORM_, __VA_ARGS__)
#define TRACE_INITIALIZE() trace_initialize()

void trace_initialize(void);

void trace_message(
	__in const char *file,
	__in const char *line,
	__in const char *funct,
	__in const char *prefix,
	__in const char *format,
	...
	);

#else
#define TRACE_ENTRY()
#define TRACE_ENTRY_MESSAGE(_FORM_, ...)
#define TRACE_EVENT(_EVT_)
#define TRACE_EVENT_MESSAGE(_FORM_, ...)
#define TRACE_EXIT()
#define TRACE_EXIT_MESSAGE(_FORM_, ...)
#define TRACE_INITIALIZE()
#endif // NDEBUG

#define KEYBUF_CAP_MAX (UINT8_MAX + 1)

#define DEFINE_DDR(_BNK_) DDR ## _BNK_
#define DEFINE_PIN(_BNK_, _PIN_) P ## _BNK_ ## _PIN_
#define DEFINE_PORT(_BNK_) PORT ## _BNK_

typedef struct _ps2_comm_t {
	volatile uint8_t *ddr_clock;
	volatile uint8_t *ddr_data;
	uint8_t pin_clock;
	uint8_t pin_data;
	volatile uint8_t *port_clock;
	volatile uint8_t *port_data;
} ps2_comm_t;

typedef struct _ps2_keybuf_t {
	uint8_t data[KEYBUF_CAP_MAX];
	uint8_t capacity;
	uint8_t count;
	uint8_t read;
	uint8_t write;
} ps2_keybuf_t;

typedef struct _ps2_state_t {
	uint8_t caps;
	uint8_t control;
	uint8_t shift;
} ps2_state_t;

typedef struct _ps2_t {
	ps2_keybuf_t buffer;
	ps2_comm_t comm;
	ps2_state_t state;
} ps2_t;

#define ps2_initialize(_CONT_, _CLK_BNK_, _CLK_PIN_, _DAT_BNK_, _DAT_PIN_, _CAP_) \
	_ps2_initialize(_CONT_, &DEFINE_DDR(_CLK_BNK_), &DEFINE_PORT(_CLK_BNK_), \
	DEFINE_PIN(_CLK_BNK_, _CLK_PIN_), &DEFINE_DDR(_DAT_BNK_), &DEFINE_PORT(_DAT_BNK_), \
	DEFINE_PIN(_DAT_BNK_, _DAT_PIN_), _CAP_)
void _ps2_initialize(
	__inout ps2_t *context,
	__in volatile uint8_t *ddr_clock,
	__in volatile uint8_t *port_clock,
	__in uint8_t pin_clock,
	__in volatile uint8_t *ddr_data,
	__in volatile uint8_t *port_data,
	__in uint8_t pin_data,
	__in uint8_t capacity
	);

void ps2_uninitialize(
	__inout ps2_t *context
	);

char ps2_read(
	__inout ps2_t *context
	);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // LIBPS2_H_

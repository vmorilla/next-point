#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <stdint.h>
#include "court_dimensions.h"

#ifdef DEBUG

#define debug_message(message) _debug_message(message)
#define debug_number(message, number) _debug_number(message, number)
#define debug_point(message, x, y) _debug_point(message, x, y)

#define debug_draw_point(x, y, color) _debug_draw_point(x, y, color)
#define debug_draw_projection(x, y, color) _debug_draw_point(x_projection(x, y), y, color)

#define assert(condition, message) _assert(condition, message)
#define debug_break() __asm__("  defb 0xfd, 0x00")
#define console_message(message) _console_message(message)

void _debug_message(const char *message);
void _debug_number(const char *label, float value);
void _debug_point(const char *label, float x, float y);
void _debug_draw_point(uint16_t x, uint8_t y, uint8_t color);
void _assert(bool condition, const char *message);
void _console_message(const char *message) __z88dk_fastcall;

#else

#define debug_message(message) ((void)0)
#define debug_number(message, number) ((void)0)
#define debug_point(message, x, y) ((void)0)

#define debug_draw_point(x, y, color) ((void)0)
#define debug_draw_projection(x, y, color) ((void)0)

#define assert(condition, message) ((void)0)
#define debug_break() ((void)0)
#define console_message(message) ((void)0)

#endif

char *float_to_string(float value, char *buffer);

#endif
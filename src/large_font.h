#ifndef _LARGE_FONT_H_
#define _LARGE_FONT_H_

#include <stdint.h>

#define LARGE_FONT_DEFAULT_PALETTE 0
#define LARGE_FONT_HIGHLIGHT_PALETTE 1

void lfont_set_position(uint8_t x, uint8_t y);
void lfont_write_char(const char character);
void lfont_write_text(const char *text);
void lfont_clear_remaining_line(void);
void lfont_set_palette(uint8_t palette);

#endif
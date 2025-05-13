#include "large_font.h"
#include "tilemap.h"

#pragma codeseg BANK_8

static uint8_t lfont_current_x = 0;
static uint8_t lfont_current_y = 0;
static uint8_t lfont_current_palette = LARGE_FONT_DEFAULT_PALETTE;

void lfont_set_position(uint8_t x, uint8_t y)
{
    lfont_current_x = x;
    lfont_current_y = y;
}

void lfont_write_char(const char character)
{
    uint8_t char_code = (character - 0x20) * 4; // Convert to tilemap character code
    tilemap[lfont_current_y][lfont_current_x].character = char_code;
    tilemap[lfont_current_y][lfont_current_x].palette = lfont_current_palette;
    tilemap[lfont_current_y][lfont_current_x + 1].character = char_code + 1;
    tilemap[lfont_current_y][lfont_current_x + 1].palette = lfont_current_palette;
    tilemap[lfont_current_y + 1][lfont_current_x].character = char_code + 2;
    tilemap[lfont_current_y + 1][lfont_current_x].palette = lfont_current_palette;
    tilemap[lfont_current_y + 1][lfont_current_x + 1].character = char_code + 3;
    tilemap[lfont_current_y + 1][lfont_current_x + 1].palette = lfont_current_palette;

    lfont_current_x += 2;
}

void lfont_write_text(const char *text)
{
    uint8_t i = 0;
    while (text[i] != '\0')
    {
        lfont_write_char(text[i++]);
    }
}

void lfont_set_palette(uint8_t palette)
{
    lfont_current_palette = palette;
}

void lfont_clear_remaining_line(void)
{
    for (uint8_t i = lfont_current_x; i < TILEMAP_WIDTH; i++)
    {
        tilemap[lfont_current_y][i].character = 0;
        tilemap[lfont_current_y][i].palette = lfont_current_palette;
        tilemap[lfont_current_y + 1][i].character = 0;
        tilemap[lfont_current_y + 1][i].palette = lfont_current_palette;
    }
}
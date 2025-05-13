#include <string.h>
#include "tilemap.h"

#define BALLOON_COORD_X 10
#define BALLOON_COORD_Y 8
#define BALLOON_CENTER 7
#define MIN_BALLOON_TEXT_LENGTH 9
#define BALLOON_FIRST_TEXT_COL (BALLOON_COORD_X + 2)
#define BALLOON_MAX_WIDTH 35

void draw_balloon_column(uint8_t screen_col, uint8_t balloon_col)
{
    for (uint8_t row = 0; row < BALLOON_HEIGHT; row++)
    {
        TileMapEntry *screen_tile = &tilemap[BALLOON_COORD_Y + row][BALLOON_COORD_X + screen_col];
        TileMapEntry *balloon_tile = &balloon_tilemap[balloon_col][row];
        screen_tile->character = balloon_tile->character;
        screen_tile->attr = balloon_tile->attr;
    }
}

/**
 * Draws the dialogue balloon with the provided text
 */
uint8_t draw_balloon(uint8_t text_length)
{
    uint8_t screen_col = 0;
    uint8_t balloon_col;

    for (balloon_col = 0; balloon_col < BALLOON_CENTER; balloon_col++)
    {
        draw_balloon_column(screen_col++, balloon_col);
    }

    // Adds one character if the length of text is even so that the text is centered
    uint8_t text_area_length = text_length < MIN_BALLOON_TEXT_LENGTH ? MIN_BALLOON_TEXT_LENGTH + (text_length + 1) % 2 : text_length;
    for (uint8_t extra_col = 0; extra_col < text_area_length - MIN_BALLOON_TEXT_LENGTH; extra_col++)
    {
        draw_balloon_column(screen_col++, BALLOON_CENTER - 1);
    }

    for (; balloon_col < BALLOON_WIDTH; balloon_col++)
    {
        draw_balloon_column(screen_col++, balloon_col);
    }

    return text_length >= MIN_BALLOON_TEXT_LENGTH ? 0 : (text_area_length - text_length) / 2;
}

/**
 * Draws text in the existing balloon at the specified column
 */
void draw_text_in_balloon(uint8_t column, const char *text)
{
    uint8_t screen_col = BALLOON_FIRST_TEXT_COL + column;
    for (uint8_t i = 0; text[i] != '\0'; i++)
    {
        tilemap[BALLOON_COORD_Y + 1][screen_col + i].character = text[i];
    }
}

/**
 * Draws the dialogue balloon with the provided text
 * The text is centered in the balloon
 */
void draw_balloon_with_text(const char *text)
{
    uint8_t text_length = strlen(text);
    uint8_t column = draw_balloon(text_length);
    draw_text_in_balloon(column, text);
}

/**
 * Clears the dialogue balloon from the screen
 */
void hide_balloon(void)
{
    for (uint8_t row = 0; row < BALLOON_HEIGHT; row++)
    {
        for (uint8_t col = 0; col < BALLOON_MAX_WIDTH; col++)
        {
            tilemap[BALLOON_COORD_Y + row][BALLOON_COORD_X + col].character = 0;
        }
    }
}

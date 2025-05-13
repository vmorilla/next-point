#include <intrinsic.h>
#include "menu_player_choice.h"

#include "layer2.h"
#include "layer2_screens.h"
#include "game_settings.h"
#include "menu.h"
#include "tilemap.h"
#include "ticks.h"
#include "opponents.h"
#include "sfx.h"

#define MENU_PLAYER_CHOICE_COLUMNS 3
#define MENU_PLAYER_CHOICE_ROWS 4

#define AVATAR_BOX_WIDTH 8
#define AVATAR_BOX_HEIGHT 4

#define AVATAR_BOX_SEPARATION_X 4
#define AVATAR_BOX_SEPARATION_Y 2

#define AVATAR_BOX_FIRST_ROW 7
#define AVATAR_BOX_FIRST_COLUMN 5
#define CORNER_CHAR 0x7D
#define BORDER_CHAR_HOR 0x7E
#define BORDER_CHAR_VERT 0x7F

#pragma codeseg BANK_8

static void draw_char(uint8_t x, uint8_t y, uint8_t character, bool x_mirror, bool y_mirror, uint8_t palette)
{
    tilemap[y][x].character = character;
    tilemap[y][x].palette = palette;
    tilemap[y][x].x_mirror = x_mirror;
    tilemap[y][x].y_mirror = y_mirror;
}

static void
draw_selection_box(uint8_t x, uint8_t y, uint8_t palette)
{
    x = x * (AVATAR_BOX_WIDTH + AVATAR_BOX_SEPARATION_X) + AVATAR_BOX_FIRST_COLUMN;
    y = y * (AVATAR_BOX_HEIGHT + AVATAR_BOX_SEPARATION_Y) + AVATAR_BOX_FIRST_ROW;

    // Draw the corners
    draw_char(x, y, CORNER_CHAR, false, false, palette);
    draw_char(x + AVATAR_BOX_WIDTH + 1, y, CORNER_CHAR, true, false, palette);
    draw_char(x, y + AVATAR_BOX_HEIGHT + 1, CORNER_CHAR, false, true, palette);
    draw_char(x + AVATAR_BOX_WIDTH + 1, y + AVATAR_BOX_HEIGHT + 1, CORNER_CHAR, true, true, palette);

    for (uint8_t i = 1; i < AVATAR_BOX_WIDTH + 1; i++)
    {
        draw_char(x + i, y, BORDER_CHAR_HOR, false, false, palette);
        draw_char(x + i, y + AVATAR_BOX_HEIGHT + 1, BORDER_CHAR_HOR, false, true, palette);
    }

    for (uint8_t i = 1; i < AVATAR_BOX_HEIGHT + 1; i++)
    {
        draw_char(x, y + i, BORDER_CHAR_VERT, false, false, palette);
        draw_char(x + AVATAR_BOX_WIDTH + 1, y + i, BORDER_CHAR_VERT, true, false, palette);
    }
}

int8_t player_choice_menu(int8_t entry)
{
    (void)entry;
    select_tilemap(TILEMAP_GAME);
    load_layer2_screen(LY2_PLAYER_SELECTION);
    uint8_t player_selection = 0;
    uint8_t player_new_selection = 0;

    diplay_profile(0);
    draw_selection_box(0, 0, TILEMAP_PALETTE_PLAYER_CHOICE);

    MenuSelectionKey input;
    do
    {
        input = read_menu_input();
        if (input == MENU_KEY_UP && player_selection >= MENU_PLAYER_CHOICE_COLUMNS)
            player_new_selection = player_selection - MENU_PLAYER_CHOICE_COLUMNS;
        else if (input == MENU_KEY_DOWN && (player_selection / MENU_PLAYER_CHOICE_COLUMNS < MENU_PLAYER_CHOICE_ROWS - 1))
            player_new_selection = player_selection + MENU_PLAYER_CHOICE_COLUMNS;
        else if (input == MENU_KEY_LEFT && player_selection > 0)
            player_new_selection = player_selection - 1;
        else if (input == MENU_KEY_RIGHT && player_selection < (MENU_PLAYER_CHOICE_COLUMNS * MENU_PLAYER_CHOICE_ROWS) - 1)
            player_new_selection = player_selection + 1;
        if (player_new_selection != player_selection)
        {
            sfx_play_sync(SFX_CURSOR_MOVE);
            reset_ticks();
            wait_for_ticks(1);
            diplay_profile(player_new_selection);
            draw_selection_box(player_selection % MENU_PLAYER_CHOICE_COLUMNS, player_selection / MENU_PLAYER_CHOICE_COLUMNS, TILEMAP_PALETTE_BALLOON);
            draw_selection_box(player_new_selection % MENU_PLAYER_CHOICE_COLUMNS, player_new_selection / MENU_PLAYER_CHOICE_COLUMNS, TILEMAP_PALETTE_PLAYER_CHOICE);
            player_selection = player_new_selection;
        }

    } while (!(input == MENU_KEY_ENTER && player_selection == 0) && input != MENU_KEY_BACK);
    sfx_play_sync(SFX_CURSOR_SELECT);

    if (input == MENU_KEY_BACK)
    {
        clear_tilemap();
        select_tilemap(TILEMAP_MENU);
        load_layer2_screen(LY2_INTRO_SCREEN);
        return -1;
    }
    else
    {
        game_settings.opponent_choice = player_selection;
        return 0;
    }
}

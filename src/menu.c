#include <input.h>
#include <intrinsic.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "menu.h"
#include "computer_controls.h"
#include "controls.h"
#include "debug.h"
#include "game_settings.h"
#include "keyboard.h"
#include "large_font.h"
#include "layer2.h"
#include "layer2_screens.h"
#include "menu_redefine_keys.h"
#include "player.h"
#include "ticks.h"
#include "tilemap.h"
#include "zxn.h"
#include "sfx.h"

// #pragma codeseg BANK_8

static MenuSelectionKey prev_input = MENU_KEY_NONE;

static MenuSelectionKey read_menu_keys(void)
{
    if (in_key_pressed(IN_KEY_SCANCODE_q) || in_key_pressed(IN_KEY_SCANCODE_7))
        return MENU_KEY_UP;
    if (in_key_pressed(IN_KEY_SCANCODE_a) || in_key_pressed(IN_KEY_SCANCODE_6))
        return MENU_KEY_DOWN;
    if (in_key_pressed(IN_KEY_SCANCODE_o) || in_key_pressed(IN_KEY_SCANCODE_5))
        return MENU_KEY_LEFT;
    if (in_key_pressed(IN_KEY_SCANCODE_p) || in_key_pressed(IN_KEY_SCANCODE_8))
        return MENU_KEY_RIGHT;
    if (in_key_pressed(IN_KEY_SCANCODE_0) || in_key_pressed(IN_KEY_SCANCODE_m) || in_key_pressed(IN_KEY_SCANCODE_k) || in_key_pressed(IN_KEY_SCANCODE_ENTER))
        return MENU_KEY_ENTER;
    if (break_key_pressed())
        return MENU_KEY_BACK;
    return MENU_KEY_NONE;
}

MenuSelectionKey read_menu_input(void)
{
    MenuSelectionKey input;
    do
    {
        input = read_menu_keys();
    } while (input == prev_input);

    prev_input = input;
    return input;
}

int8_t handle_std_menu(MenuEntry (*menu_entries)[], uint8_t n_entries, uint8_t left_column, bool top_menu)
{
    clear_tilemap();
    uint8_t selection = 0;

    do
    {
        for (uint8_t i = 0; i < n_entries; i++)
        {
            lfont_set_palette(i == selection ? LARGE_FONT_HIGHLIGHT_PALETTE : LARGE_FONT_DEFAULT_PALETTE);
            lfont_set_position(left_column, MENU_FIRST_ROW + i * MENU_ROW_SEPARATION);
            lfont_write_text((*menu_entries)[i].label);
            if ((*menu_entries)[i].suffix_fn != NULL)
            {
                (*menu_entries)[i].suffix_fn(i);
            }
        }

        // Wait for keyboard input
        uint8_t prev_selection = selection;
        while (prev_selection == selection)
        {
            MenuSelectionKey input = read_menu_input();
            if (input == MENU_KEY_UP && selection > 0)
            {
                sfx_play_sync(SFX_CURSOR_MOVE);
                selection--;
            }
            else if (input == MENU_KEY_DOWN && selection < n_entries - 1)
            {
                sfx_play_sync(SFX_CURSOR_MOVE);
                selection++;
            }
            else if (input == MENU_KEY_ENTER)
            {
                sfx_play_sync(SFX_CURSOR_SELECT);
                // Call action
                if ((*menu_entries)[selection].action != NULL)
                {
                    const int8_t cmd_result = (*menu_entries)[selection].action(selection);

                    if (cmd_result >= 0)
                        return cmd_result;
                    else if (cmd_result < -1)
                        return cmd_result + 1;
                    else
                        break;
                }
            }
            else if (input == MENU_KEY_BACK && !top_menu)
            {
                clear_tilemap();
                return -1;
            }
        }
    } while (true);
}

int8_t menu_go_back(int8_t entry)
{
    (void)entry;
    clear_tilemap();

    return -2;
}

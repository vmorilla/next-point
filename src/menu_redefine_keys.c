#include <input.h>
#include "keyboard.h"
#include "menu_redefine_keys.h"
#include "game_settings.h"
#include "large_font.h"
#include "menu.h"
#include "tilemap.h"
#include "sfx.h"

#define REFINE_KEYS_MENU_LEFT_COLUMN 30

// Stores the custom keys as ascii characters... must be consistent with the scancodes in game settings
static char custom_keys[2][N_CUSTOM_CONTROL_KEYS] = {
    {'M', 'P', 'O', 'A', 'Q'},
    {'0', '8', '5', '6', '7'},
};

static const char *control_names[N_CUSTOM_CONTROL_KEYS] = {
    "HIT.....",
    "RIGHT...",
    "LEFT....",
    "DOWN....",
    "UP......",
};

static void write_key_name(char character)
{
    switch (character)
    {
    case 13:
        lfont_write_text("ENTER");
        break;
    case 32:
        lfont_write_text("ENTER");
        break;
    default:
        lfont_write_char(character);
        lfont_write_text("    ");
    }
}

static void display_keyset(uint8_t selection, uint8_t set)
{
    // Loop traversed backwards to present the keys in the more natural order QAOPM
    for (uint8_t i = 0; i < N_CUSTOM_CONTROL_KEYS; i++)
    {
        lfont_set_palette(i == selection ? LARGE_FONT_HIGHLIGHT_PALETTE : LARGE_FONT_DEFAULT_PALETTE);
        lfont_set_palette(i == selection ? LARGE_FONT_HIGHLIGHT_PALETTE : LARGE_FONT_DEFAULT_PALETTE);
        lfont_set_position(REFINE_KEYS_MENU_LEFT_COLUMN, MENU_FIRST_ROW + i * MENU_ROW_SEPARATION);
        lfont_write_text(control_names[N_CUSTOM_CONTROL_KEYS - 1 - i]);
        write_key_name(custom_keys[set][N_CUSTOM_CONTROL_KEYS - 1 - i]);
    }
}

int8_t redefine_keys_menu(int8_t keyset)
{
    keyset = keyset % 2;

    clear_tilemap();
    // Displays current key set
    while (1)
    {
        display_keyset(255, keyset);
        lfont_set_position(5, MENU_NOTES_ROWS);
        lfont_set_palette(LARGE_FONT_DEFAULT_PALETTE);
        lfont_write_text("[ENTER] TO CHANGE / [BREAK] TO EXIT");

        while (in_key_pressed(IN_KEY_SCANCODE_ENTER) == 0)
        {
            // Wait for key press
            if (break_key_pressed() || in_key_pressed(IN_KEY_SCANCODE_SPACE))
            {
                sfx_play_sync(SFX_CURSOR_SELECT);
                return menu_go_back(0);
            }
        }

        lfont_set_position(5, MENU_NOTES_ROWS);
        lfont_write_text("       CHOOSE YOUR OWN KEYS        ");

        for (uint8_t i = 0; i < N_CUSTOM_CONTROL_KEYS; i++)
        {
            in_wait_nokey();
            display_keyset(i, keyset);
            in_wait_key();
            int key = in_inkey();
            sfx_play_sync(SFX_CURSOR_SELECT);
            char key_char = key > 'Z' ? key - 32 : key;
            int scancode = in_key_scancode(key);
            custom_keys[keyset][N_CUSTOM_CONTROL_KEYS - i - 1] = key_char;
            game_settings.user_defined_keys[keyset].scancodes[N_CUSTOM_CONTROL_KEYS - i - 1] = scancode;
        }
        in_wait_nokey();
    }
}

// #include <input.h>
// #include <intrinsic.h>
#include <stdint.h>
#include <stddef.h>

#include "menu.h"
#include "menu_main.h"
#include "menu_options.h"
#include "menu_controls.h"
#include "game_settings.h"
#include "layer2.h"
#include "layer2_screens.h"
#include "menu_player_choice.h"
#include "tilemap.h"

#pragma codeseg BANK_8

#define LONGEST_TEXT (11 * 2)
#define MAIN_MENU_LEFT_COLUMN ((80 - LONGEST_TEXT) / 2)

int8_t simple_action(int8_t entry)
{
    return entry;
}

static MenuEntry main_menu[] = {
    {"ONE PLAYER", player_choice_menu, NULL},
    {"TWO PLAYERS", simple_action, NULL},
    {"DEMO", simple_action, NULL},
    {"CONTROLS", start_controls_menu, NULL},
    {"OPTIONS", start_options_menu, NULL},
    {"CREDITS", NULL, NULL}};

void start_main_menu(void)
{
    select_tilemap(TILEMAP_MENU);
    load_layer2_screen(LY2_INTRO_SCREEN);
    game_settings.game_type = handle_std_menu(&main_menu, sizeof(main_menu) / sizeof(MenuEntry), MAIN_MENU_LEFT_COLUMN, true);
    // if (game_settings.game_type == GAME_TYPE_SINGLE_PLAYER)
    // {
    //     player_choice_menu();
    // }
}

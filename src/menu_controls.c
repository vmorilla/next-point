#include <stddef.h>
#include <stdbool.h>

#include "menu_controls.h"

#include "large_font.h"
#include "game_settings.h"
#include "menu.h"
#include "menu_redefine_keys.h"

#define CONTROLS_MENU_LEFT_COLUMN 23

#pragma codeseg BANK_8

static const char *user_control_names[N_USER_CONTROL_TYPES] = {
    "KEYSET 1",
    "JOYSTICK 1",
    "KEYSET 2",
    "JOYSTICK 2",
};

int8_t change_user_control_type(int8_t entry)
{
    game_settings.user_control_type[entry] = (game_settings.user_control_type[entry] + 1) % SURFACE_TYPE_COUNT;
    return -1;
}

void display_user_control_type(int8_t entry)
{
    lfont_write_text(user_control_names[game_settings.user_control_type[entry]]);
    lfont_clear_remaining_line();
}

static MenuEntry user_control_menu[] = {
    {"PLAYER 1: ", change_user_control_type, display_user_control_type},
    {"PLAYER 2: ", change_user_control_type, display_user_control_type},
    {"REDEFINE KEYSET 1", redefine_keys_menu, NULL},
    {"REDEFINE KEYSET 2", redefine_keys_menu, NULL},
    {"BACK", menu_go_back, NULL}};

int8_t start_controls_menu(int8_t entry)
{
    (void)entry;
    return handle_std_menu(&user_control_menu, sizeof(user_control_menu) / sizeof(MenuEntry), CONTROLS_MENU_LEFT_COLUMN, false);
}
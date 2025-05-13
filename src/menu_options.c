#include <stddef.h>
#include <stdbool.h>

#include "menu_options.h"

#include "large_font.h"
#include "game_settings.h"
#include "menu.h"

#define OPTIONS_MENU_LEFT_COLUMN 21

static const char *surface_names[SURFACE_TYPE_COUNT] = {
    "GRASS",
    "CLAY",
    "HARD",
    "RANDOM",
};

static const char *speed_names[SPEED_TYPE_COUNT] = {
    "SLOW",
    "NORMAL",
    "FAST",
};

int8_t change_surface(int8_t entry)
{
    (void)entry;
    game_settings.surface_choice = (game_settings.surface_choice + 1) % SURFACE_TYPE_COUNT;
    return -1;
}

void display_surface(int8_t entry)
{
    (void)entry;
    lfont_write_text(surface_names[game_settings.surface_choice]);
    lfont_clear_remaining_line();
}

int8_t change_speed(int8_t entry)
{
    (void)entry;
    game_settings.speed = (game_settings.speed + 1) % SPEED_TYPE_COUNT;
    return -1;
}

void display_speed(int8_t entry)
{
    (void)entry;
    lfont_write_text(speed_names[game_settings.speed]);
    lfont_clear_remaining_line();
}

static MenuEntry options_menu[] = {
    {"SURFACE: ", change_surface, display_surface},
    {"GAME SPEED: ", change_speed, display_speed},
    {"BACK", menu_go_back, NULL}};

int8_t start_options_menu(int8_t entry)
{
    (void)entry;
    return handle_std_menu(&options_menu, sizeof(options_menu) / sizeof(MenuEntry), OPTIONS_MENU_LEFT_COLUMN, false);
}
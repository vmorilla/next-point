#include <input.h>
#include "game_settings.h"

GameSettings game_settings = {
    .game_type = GAME_TYPE_SINGLE_PLAYER,
    .user_control_type = {
        USER_CONTROL_KEYBOARD_1,
        USER_CONTROL_KEYBOARD_2,
    },
    .user_defined_keys = {
        {.scancodes = {IN_KEY_SCANCODE_m, IN_KEY_SCANCODE_p, IN_KEY_SCANCODE_o, IN_KEY_SCANCODE_a, IN_KEY_SCANCODE_q}},
        {.scancodes = {IN_KEY_SCANCODE_0, IN_KEY_SCANCODE_8, IN_KEY_SCANCODE_5, IN_KEY_SCANCODE_6, IN_KEY_SCANCODE_7}},
    },
    .surface_choice = SURFACE_RANDOM,
    .speed = SPEED_NORMAL,
};

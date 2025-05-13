#include <input.h>
#include "controls.h"
#include "computer_controls.h"
#include "game_settings.h"
#include "game_state.h"
#include "player.h"

static udk_t *user_defined_keys1;
static udk_t *user_defined_keys2;

ControlInput in_stick_keyboard1(uint8_t player_id)
{
    (void)player_id;
    return in_stick_keyboard(user_defined_keys1);
}

ControlInput in_stick_keyboard2(uint8_t player_id)
{
    (void)player_id;
    return in_stick_keyboard(user_defined_keys2);
}

/**
 * Next joysticks are set to cursor and kempston in the hardware initialization
 */
static ControlInput in_stick_joystick1(uint8_t player_id)
{
    (void)player_id;
    return in_stick_cursor();
}

static ControlInput in_stick_joystick2(uint8_t player_id)
{
    (void)player_id;
    return in_stick_kempston();
}

static ControlInputFn control_menu_table[N_USER_CONTROL_TYPES] = {
    in_stick_keyboard1, // USER_CONTROL_KEYBOARD_1
    in_stick_joystick1, // USER_CONTROL_JOYSTICK_1
    in_stick_keyboard2, // USER_CONTROL_KEYBOARD_2
    in_stick_joystick2, // USER_CONTROL_JOYSTICK_2
};

// =================================================================================================
// Public functions
// =================================================================================================

void init_controls(void)
{
    user_defined_keys1 = (udk_t *)&game_settings.user_defined_keys[0].user_defined_keys;
    user_defined_keys2 = (udk_t *)&game_settings.user_defined_keys[1].user_defined_keys;

    // Bottom player is player 1, top player is player 2
    player_bottom.control_fn = control_menu_table[game_settings.user_control_type[0]];
    player_top.control_fn = control_menu_table[game_settings.user_control_type[1]];

    switch (game_settings.game_type)
    {
    case GAME_TYPE_SINGLE_PLAYER:
        player_top.control_fn = read_computer_control;
        break;
    case GAME_TYPE_TWO_PLAYERS:
        break;
    case GAME_TYPE_DEMO:
        player_top.control_fn = read_computer_control;
        player_bottom.control_fn = read_computer_control;

        break;
    }
}
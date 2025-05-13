#ifndef _CONTROLS_H_
#define _CONTROLS_H_

#include <input.h>
#include "game_settings.h"

/** Bit flags of the control of a player */
typedef enum
{
    CONTROL_UP = IN_STICK_UP,
    CONTROL_DOWN = IN_STICK_DOWN,
    CONTROL_LEFT = IN_STICK_LEFT,
    CONTROL_RIGHT = IN_STICK_RIGHT,
    CONTROL_A = IN_STICK_FIRE,
    CONTROL_B = IN_STICK_FIRE_2,
    CONTROL_C = IN_STICK_FIRE_3,
    CONTROL_FIRE = 0x10, // Any fire button, used to trigger fire only once
    CONTROL_DIRECTIONS = CONTROL_UP | CONTROL_DOWN | CONTROL_LEFT | CONTROL_RIGHT,
} ControlInput;

/** Function defintion to control players */
typedef ControlInput (*ControlInputFn)(uint8_t playerId);

/** Initializes the controls in players. */
void init_controls(void);

#endif
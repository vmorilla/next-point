#ifndef _COMPUTER_CONTROLS_H_
#define _COMPUTER_CONTROLS_H_

#include <stdint.h>
#include "controls.h"

ControlInput read_computer_control(uint8_t player_id);

// To be called by the game state to restart the computer player state machine
void restart_computer_controls(void);

// TODO: add some configuration functions to set the computer player behaviour

#endif
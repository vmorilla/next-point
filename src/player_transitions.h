#ifndef _PLAYER_TRANSITIONS_H_
#define _PLAYER_TRANSITIONS_H_

#include "player.h"

// Transitions between states
void transition_idle(Player *player);
void transition_serve_ready(Player *player);
void transition_serve_ball_up(Player *player);
void transition_receive_serve(Player *player);

/**
 * The player presses the button to hit the ball during service
 */
void transition_serve_hit_ball(Player *player);
void transition_lateral_move(Player *player);

/**
 *  The player presses the buttons to hit the ball during the game
 * */
void transition_hit_ball(Player *player);
void transition_front_walk(Player *player);

/**
 * The player is hit by the ball
 */
void transition_touch(Player *player);

#endif

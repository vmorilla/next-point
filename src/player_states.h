#ifndef _PLAYER_STATES_H_
#define _PLAYER_STATES_H_

/**
 * Player state machine functions
 */

#include "player.h"

void player_idle(Player *player);
void player_service_ready(Player *player);
void player_service_ball_up(Player *player);
void player_service_ball_hit(Player *player);
// Many states require no action (behaviour controlled fully by frame events)
void player_no_action(Player *player);
void player_walk(Player *player);

#endif
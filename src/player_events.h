#ifndef _PLAYER_EVENTS_H_
#define _PLAYER_EVENTS_H_

#include "player.h"

// Frame animation Events
void event_serve_ball_up(Player *player);
void event_serve_hit_ball(Player *player);
void event_serve_hit_end_animation(Player *player);
void event_hit_ball(Player *player);

#endif
#ifndef _GAME_LOOP_H
#define _GAME_LOOP_H

void game_loop(void);

// This variable represents the time passed since last game cycle
extern float next_iteration_delta;

// This is the speed of the game in frames per second
extern float game_frame_speed;

#endif
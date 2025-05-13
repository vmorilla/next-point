#include "player_states.h"
#include "player_transitions.h"
#include "court_dimensions.h"
#include "ball.h"
#include "game_loop.h"
#include "game_state.h"
#include "debug.h"

// #pragma codeseg BANK_8

#define PLAYER_LAT_WALK_SPEED 3
#define PLAYER_UPDOWN_WALK_SPEED 2

// ---------------------------------------------------------
// Common functions
// ---------------------------------------------------------

void move_player(Player *player, const Boundary *boundary)
{
    ControlInput control = player->control;

    if (control & CONTROL_UP && player->y > boundary->top)
        player->y -= PLAYER_UPDOWN_WALK_SPEED * next_iteration_delta;
    if (control & CONTROL_DOWN && player->y < boundary->bottom)
        player->y += PLAYER_UPDOWN_WALK_SPEED * next_iteration_delta;
    if (control & CONTROL_LEFT && player->x > boundary->left)
        player->x -= PLAYER_LAT_WALK_SPEED * next_iteration_delta;
    if (control & CONTROL_RIGHT && player->x < boundary->right)
        player->x += PLAYER_LAT_WALK_SPEED * next_iteration_delta;
}

// ---------------------------------------------------------
// Player state machine functions
// ---------------------------------------------------------

void player_idle(Player *player)
{
    const Boundary *boundary = &court_boundaries[player->id];
    move_player(player, boundary);
    // if (player->control & (CONTROL_LEFT | CONTROL_RIGHT))
    //     transition_lateral_move(player);
    if (player->control & (CONTROL_UP | CONTROL_DOWN | CONTROL_LEFT | CONTROL_RIGHT))
        transition_front_walk(player);
    if (player->control & CONTROL_FIRE)
        transition_hit_ball(player);
}

void player_service_ready(Player *player)
{
    if (player->control & CONTROL_FIRE)
    {
        // Service ball up
        transition_serve_ball_up(player);
    }
}

void player_service_ball_up(Player *player)
{
    // Only if the ball is shown (it has been tossed up, we can try to hit it)
    if (player->control & CONTROL_FIRE && !ball.hidden)
    {
        transition_serve_hit_ball(player);
    }
}

void player_service_ball_hit(Player *player)
{
    (void)player;
}

void player_no_action(Player *player)
{
    // No action required
    (void)player;
}

void player_walk(Player *player)
{
    if (player->control & CONTROL_FIRE)
        transition_hit_ball(player);
    else
    {
        const Boundary *boundary = &court_boundaries[player->id];
        move_player(player, boundary);
        if (player->control & (CONTROL_UP | CONTROL_DOWN | CONTROL_LEFT | CONTROL_RIGHT))
            player->state_timer = WALK_DURATION;
        else if (--player->state_timer == 0) // Time to go back to idle
            transition_idle(player);
    }
}

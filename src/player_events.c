#include <stdbool.h>
#include <math.h>
#include "player_events.h"
#include "player_transitions.h"
#include "ball.h"
#include "game_state.h"
#include "score_board.h"
#include "physics.h"
#include "player_ref_points.h"
#include "messages.h"
#include "sound.h"
#include "player_hits.h"

#include "debug.h"

#pragma codeseg BANK_8

// ---------------------------------------------------------
// Constants
// ---------------------------------------------------------

#define MINIMUM_SERVE_HIT_DISTANCE 14
#define SERVICE_FAST_SPEED 20
#define SERVICE_SLOW_SPEED 10

#define SERVICE_BALL_SPEED_UP 4
#define SERVICE_BALL_SPEED_SIDE 0.2

// ---------------------------------------------------------
// Helper functions
// ---------------------------------------------------------

float distance_ball_raquet(Player *player, RefPoint *point)
{
    const float x = player->x + point->x;
    const float y = player->y + point->y;
    const float h = point->height;

    const float x_distance = ball.x - x;
    const float y_distance = ball.y - y;
    const float h_distance = ball.height - h;

    return x_distance * x_distance + y_distance * y_distance + h_distance * h_distance;
}

// ---------------------------------------------------------
// State transition functions and animation events
// ---------------------------------------------------------

/**
 * Event triggered by the last frame of the service ball up animation to toss the ball up
 */
void event_serve_ball_up(Player *player)
{
    hide_score_board();
    ball.x = player->x + (player->id == PLAYER_TOP ? -4 : 4);
    ball.y = player->y + (player->id == PLAYER_TOP ? 1 : -1);
    ball.height = 43;
    ball.hidden = false;
    // Throws the ball slighly towards the player
    ball.v_x = player->id == PLAYER_TOP ? SERVICE_BALL_SPEED_SIDE : -SERVICE_BALL_SPEED_SIDE;
    ball.v_y = 0;
    ball.v_h = -SERVICE_BALL_SPEED_UP;
}

// Event triggered in the frame where the player hits the ball during serve
void event_serve_hit_ball(Player *player)
{
    uint8_t ball_distance = fabs(ball.height - ref_points[REF_POINT_SERVICE][player->id].height);
    if (ball_distance <= MINIMUM_SERVE_HIT_DISTANCE)
    {
        service_hit(player, ball_distance);
        // TODO: sound depending on the speed of the ball
        play_sound(SOUND_BALL_HIT);
        event_player_hit(player->id);
    }
    else
    {
        // The player made an attempt to hit the ball, it will be fault if it bounces
        game_state.point_status = POS_SERVICE_HIT_ATTEMPT;
    }
}

void event_serve_hit_end_animation(Player *player)
{
    // Little jump after hitting the ball
    if (player->id == PLAYER_TOP)
        player->y += 4;
    else
        player->y -= 4;
    transition_idle(player);
}

void event_hit_ball(Player *player)
{
    // Hits the ball
    if (ball_at_reach(player))
    {
        play_sound(SOUND_BALL_HIT_2);

        // Update the game state correspondingly
        event_player_hit(player->id);

        if (player->control_at_hit & (player->id == PLAYER_BOTTOM ? CONTROL_DOWN : CONTROL_UP))
        {
            set_lob_ball_velocity(CENTER_LINE, BASELINE_TOP + 20, 160);
            // set_ball_velocity_net_height(CENTER_LINE, BASELINE_TOP + 20, MAXIMUM_NET_HEIGHT + 2);
        }
        else
        {
            // TODO: different hit approach
            // We set the speed of the ball depending on the distance to the player
            standard_hit(player, 10);
            // set_ball_speed_at_drive(player->id, 10, distance / 10);
        }
    }
}

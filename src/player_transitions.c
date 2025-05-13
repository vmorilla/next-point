#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "ball.h"
#include "court_dimensions.h"
#include "game_state.h"
#include "player_transitions.h"
#include "physics.h"
#include "player_ref_points.h"
#include "physical_coords.h"
#include "sound.h"

#pragma codeseg BANK_8

void transition_idle(Player *player)
{
    set_player_status(player, PS_IDLE);
}

// Serve
void transition_serve_ready(Player *player)
{
    set_player_status(player, PS_SERVICE_READY);

    bool ad_service = is_ad_service();
    // Serves in the right part of the court
    bool right_service = player->id ^ ad_service;

    // Set players positions
    player->x = right_service ? CENTER_LINE_RIGHT + PLAYER_WIDTH / 2 : CENTER_LINE_LEFT - PLAYER_WIDTH / 2;
    player->y = player->id == PLAYER_TOP ? BASELINE_TOP - 2 : BASELINE_BOTTOM + 4;
    hide_ball();
    // Though hidden, the position and speed of the ball will determine the camera position
    ball.x = player->x;
    ball.y = player->y;
    ball.v_x = 0;
    ball.v_y = 0;
    // set_camera_target(x_projection(ball.x, ball.y), ball.y - 48);
}

void transition_receive_serve(Player *player)
{
    set_player_status(player, PS_IDLE);

    bool ad_service = is_ad_service();
    bool right_service_receive = player->id ^ ad_service;
    player->x = right_service_receive ? CENTER_LINE_RIGHT + PLAYER_WIDTH / 2 : CENTER_LINE_LEFT - PLAYER_WIDTH / 2;
    player->y = player->id == PLAYER_TOP ? BASELINE_TOP - 2 : BASELINE_BOTTOM + 4;
}

void transition_serve_ball_up(Player *player)
{
    set_player_status(player, PS_SERVICE_BALL_UP);
}

void transition_front_walk(Player *player)
{
    player->state_timer = WALK_DURATION;
    set_player_status(player, PS_WALK);
}

void transition_serve_hit_ball(Player *player)
{
    play_sound_channel_b(SOUND_UMPF_DIEGO_1 + 2 * player->id + ((rand() / 16) % 2));
    player->control_at_hit = player->control;
    set_player_status(player, PS_SERVICE_BALL_HIT);
}

float distance_ref_point(RefPoint *point, float offset_x, float offset_h)
{
    const float x_distance = point->x - offset_x;
    const float h_distance = point->height - offset_h;
    return x_distance * x_distance + h_distance * h_distance;
}

void transition_hit_ball(Player *player)
{
    player->control_at_hit = player->control;

    PlayerState hit = closest_type_of_hit(player);

    set_player_status(player, hit);
}

void transition_touch(Player *player)
{
    set_player_status(player, PS_TOUCH);
    play_sound_channel_b(SOUND_TOUCH_HIT_MALE);
}

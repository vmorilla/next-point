#include <math.h>
#include "player_hits.h"
#include "ball.h"
#include "court_dimensions.h"
#include "game_state.h"
#include "xrandom.h"
#include "debug.h"
#include "colors.h"

#pragma codeseg BANK_8

// Max deviation from target, Y component of standard hit
#define STD_HIT_MAX_Y_DEVIATION 20
#define STD_HIT_MAX_X_DEVIATION 45

// Max distance of the target hit in the X axis opposed to the direction of the hit
// (i.e. for the drive, distance to the right or for the backhand, distance to the left
// (from the point of of the player at the bottom))
// Distance is calculated for a Y distance of the length of the court (VALID_COURT_AREA_HEIGHT)
#define STD_HIT_MAX_X_COUNTER_HIT_RANGE 25

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ABS(a) ((a) < 0 ? -(a) : (a))

// -----------------------------------------------------------------------
// Auxiliar functions for service... to be personalized for each player
// -----------------------------------------------------------------------

bool fast_service(Player *player)
{
    return player->control_at_hit & CONTROL_B;
}

/**
 * 0 means left corner, 100 means right corner
 */
int8_t service_target_x(Player *player, bool fast)
{
    uint8_t margin = fast ? 5 : 10;
    if (player->control_at_hit & CONTROL_LEFT)
    {
        return margin;
    }
    else if (player->control_at_hit & CONTROL_RIGHT)
    {
        return 100 - margin;
    }
    else
    {
        return 50;
    }
}

uint8_t service_margin_y(Player *player, bool fast)
{
    (void)player;
    return fast ? 2 : 7;
}

uint8_t service_margin_net(Player *player, bool fast)
{
    (void)player;
    return fast ? 2 : 8;
}

// -----------------------------------------------------------------------
// Auxiliar functions for service... to be personalized for each player
// -----------------------------------------------------------------------

bool fast_std_hit(Player *player)
{
    return player->control_at_hit & (player->id == PLAYER_TOP ? CONTROL_DOWN : CONTROL_UP);
}

/**
 * 0 means left corner, 100 means right corner
 */
int8_t std_hit_target_x(Player *player, bool fast)
{
    uint8_t margin = fast ? 5 : 10;
    if (player->control_at_hit & CONTROL_LEFT)
    {
        return margin;
    }
    else if (player->control_at_hit & CONTROL_RIGHT)
    {
        return 100 - margin;
    }
    else
    {
        return 50;
    }
}

uint8_t std_hit_margin_net(Player *player, bool fast)
{
    (void)player;
    return fast ? 2 : 8;
}

// -----------------------------------------------------------------------
// Hit functions
// -----------------------------------------------------------------------

void service_hit(Player *player, uint8_t ball_distance)
{
    if (ball.v_h < 0)
        ball_distance += 10;

    // Corners
    const Boundary *serv_area = service_area();

    bool fast = fast_service(player);
    int16_t target_x = service_target_x(player, fast) / 100.0f * (serv_area->right - serv_area->left) + serv_area->left;

    uint8_t serv_marg = service_margin_y(player, fast);
    int16_t target_y = player->id == PLAYER_TOP ? serv_area->bottom - serv_marg : serv_area->top + serv_marg;

    // debug_draw_projection(target_x, target_y, COLOR_YELLOW);

    target_x += normal_dist_random(0, ball_distance * 2);
    target_y += normal_dist_random(0, ball_distance / 2);

    // debug_draw_projection(target_x, target_y, COLOR_BLACK);

    uint8_t net_height = MAXIMUM_NET_HEIGHT + service_margin_net(player, fast) + normal_dist_random(0, ball_distance / 2);
    // uint8_t net_height = MAXIMUM_NET_HEIGHT - 2;

    // debug_number("Ball distance", ball_distance);
    set_ball_velocity_net_height(target_x, target_y, net_height);
}

void standard_hit(Player *player, uint8_t ball_distance)
{
    // debug_number("STD ball dist", ball_distance);
    bool fast = fast_std_hit(player);
    uint8_t y_deviation = MIN(ball_distance, STD_HIT_MAX_Y_DEVIATION);
    int y_target_mean = player->id == PLAYER_BOTTOM ? (BASELINE_TOP + y_deviation) : (BASELINE_BOTTOM - y_deviation);
    int y_target = limited_normal_dist_random(y_target_mean, y_deviation, y_deviation * 3);

    float y_distance = ABS(y_target - ball.y);
    uint8_t x_deviation = MIN(ball_distance * (y_distance / VALID_COURT_AREA_HEIGHT), STD_HIT_MAX_X_DEVIATION);
    float x_max_opposed_range = STD_HIT_MAX_X_COUNTER_HIT_RANGE * (y_distance / VALID_COURT_AREA_HEIGHT);

    // True for the bottom player when hitting with the backhand or the top player when hitting with the drive
    bool back_hand = (player->state == PS_DRIVE) ^ player->id;
    service_target_x(player, fast_service(player));

    int counter_hit_range = STD_HIT_MAX_X_COUNTER_HIT_RANGE * (y_distance / VALID_COURT_AREA_HEIGHT);
    int x_min = back_hand ? MAX(LEFT_SIDE_LINE, ball.x - counter_hit_range) : LEFT_SIDE_LINE;
    int x_max = back_hand ? RIGHT_SIDE_LINE : MIN(RIGHT_SIDE_LINE, ball.x + counter_hit_range);

    // if (player->id == PLAYER_BOTTOM)
    // {
    //     debug_message(back_hand ? "BACKHAND" : "DRIVE");
    //     debug_point("X min-max", x_min, x_max);
    //     debug_point("X / opp range", ball.x, x_max_opposed_range);
    //     debug_halt_next_frame();
    // }

    int x_mean = x_min + (std_hit_target_x(player, fast) / 100.0f) * (x_max - x_min);

    float x_target = limited_normal_dist_random(x_mean, x_deviation, x_deviation * 3);

    uint8_t net_height = normal_dist_random(MAXIMUM_NET_HEIGHT + std_hit_margin_net(player, fast), ball_distance / 2);

    set_ball_velocity_net_height(x_target, y_target, net_height);
}

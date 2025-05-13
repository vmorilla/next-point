#include <input.h>
#include <math.h>
#include "ball.h"
#include "net.h"
#include "game_state.h"
// TODO: avoid dependencies with player rather than with game state
#include "player.h"
#include "court_dimensions.h"
#include "physics.h"
#include "sound.h"
#include "debug.h"
#include "colors.h"
#include "game_loop.h"

#pragma codeseg BANK_8

// ---------------------------------------------------------
// Constants
// ---------------------------------------------------------

// Relation between the velocity after and before bouncing against the net and walls
#define WALL_COEF_RESTITUTION 0.05
#define MAX_HORIZONTAL_SPEED 10
#define MAX_VERTICAL_SPEED 8

// 0.175 too long
// 0.170 too long
#define NET_COEF_RESTITUTION 0.165
#define BALL_DIAMETER 5

#define PLAYER_COEF_RESTITUTION 0.25
#define PLAYER_TOUCH_WIDTH 16
#define PLAYER_TOUCH_HEIGHT 44

#define sq(x) ((x) * (x))

// ---------------------------------------------------------
// Variables
// ---------------------------------------------------------

// Ball ball = {150, 150, 30, 0, 0, 0};
Ball ball = {
    .x = 160,
    .y = 150,
    .height = 30,
    .v_h = 0,
    .v_x = 0,
    .v_y = 0,
    .hidden = true,
};

// ---------------------------------------------------------
// Functions
// ---------------------------------------------------------

void ball_hits_ground(void)
{
    if (ball.v_h > 1.5)
        play_sound(SOUND_BALL_BOUNCE);
    ball.height = 0;
    event_ball_bounced();
    // The condition prevents excessive bounces when the ball is very close to the ground
    if (ball.v_h > 1)
    {
        // TODO: consider friction for vectors v_x and v_y
        ball.v_h = -ball.v_h * GROUND_COEF_RESTITUTION;
    }
    else
    {
        ball.v_h = 0;
        // Avoid odd movemement when the ball is in the ground
        ball.v_x = 0;
        ball.v_y = 0;
    }
}

void ball_crosses_net(void)
{
    // TODO: consider touching the upper part of the net
    // The ball has crossed the net
    uint8_t net_height = net_height_at(ball.x);
    if (ball.height < net_height)
    {
        event_net_touched();
        play_sound(SOUND_BALL_HITS_NET);
        float ball_net_overlap = net_height - ball.height;
        // debug_number("RIM", ball_net_overlap);
        if (ball_net_overlap >= BALL_DIAMETER)
        {
            ball.y = NET_POSITION_Y - ball.v_y; // Move the ball back... make sure it does not cross the net
            ball.v_y = -ball.v_y * NET_COEF_RESTITUTION;
        }
        else
        {
            // debug_number("RIM", ball_net_overlap);
            // 100 -> coef como over/ball -> y, inverse, 100/coef = y / (over/ball) => y = 100 * ball_net_overlap / ball_height / coef
            // TODO: introduce random factor
            float corrected_coef_restitution = NET_COEF_RESTITUTION * (0.5 + 2 * ball_net_overlap / BALL_DIAMETER);
            // debug_number("CORRECTED", corrected_coef_restitution);
            ball.v_y *= NET_COEF_RESTITUTION;
            ball.v_x *= NET_COEF_RESTITUTION;
            ball.v_h = -fabs(ball.v_h) * corrected_coef_restitution;
        }
    }
}

bool check_player_touch(Player *player)
{
    if ((player->state == PS_IDLE || player->state == PS_WALK) &&
        fabs(ball.x - player->x) < PLAYER_WIDTH / 2 && ball.height <= PLAYER_TOUCH_HEIGHT)
    {
        event_player_touch(player);
        ball.v_y = -ball.v_y * PLAYER_COEF_RESTITUTION;
        ball.v_x = ball.v_x * PLAYER_COEF_RESTITUTION;
        ball.v_h = ball.v_h * PLAYER_COEF_RESTITUTION;

        return true;
    }
    return false;
}

bool check_players_touch(void)
{
    float next_ball_y = ball.y + ball.v_y * next_iteration_delta;
    if (ball.v_y > 0)
    {
        if (ball.y < player_bottom.y && next_ball_y >= player_bottom.y)
            return check_player_touch(&player_bottom);
    }
    else if (ball.v_y < 0)
    {
        if (ball.y > player_top.y && next_ball_y <= player_top.y)
            return check_player_touch(&player_top);
    }
    return false;
}

void update_ball(void)
{
    if (ball.hidden)
        return;

    ball.v_h += GRAVITY * next_iteration_delta;
    ball.height -= ball.v_h * next_iteration_delta;
    if (ball.height < 0)
        ball_hits_ground();

    ball.x += ball.v_x * next_iteration_delta;
    if (ball.x < COURT_LEFT)
    {
        ball.x = COURT_LEFT;
        ball.v_x *= -WALL_COEF_RESTITUTION;
        event_ball_bounced();
    }
    else if (ball.x > COURT_RIGHT)
    {
        ball.x = COURT_RIGHT;
        ball.v_x *= -WALL_COEF_RESTITUTION;
        event_ball_bounced();
    }

    if (check_players_touch())
        return;

    bool ball_before_net = ball.y < NET_POSITION_Y;
    ball.y += ball.v_y * next_iteration_delta;
    if (ball_before_net != (ball.y < NET_POSITION_Y))
        ball_crosses_net();

    if (ball.y < COURT_TOP)
    {
        ball.y = COURT_TOP;
        ball.v_y *= -WALL_COEF_RESTITUTION;
        event_ball_bounced();
    }
    else if (ball.y > COURT_BOTTOM)
    {
        ball.y = COURT_BOTTOM;
        ball.v_y *= -WALL_COEF_RESTITUTION;
        event_ball_bounced();
    }
}

/**
 * Sets the ball speed to reach a maximum height and a target bounce point
 */
void set_lob_ball_velocity(int16_t x, int16_t y, uint8_t top_height)
{
    if (ball.height >= top_height)
    {
        // No solution
        // debug_number("Lob impossible", top_height - ball.height);
        return;
    }
    ball.v_h = -fmin(sqrt(2 * GRAVITY * (top_height - ball.height)), MAX_VERTICAL_SPEED);
    // debug_number("v_h", ball.v_h);
    // Moment the ball will hit the ground
    float tg = (-ball.v_h + sqrt(ball.v_h * ball.v_h + 2 * GRAVITY * ball.height)) / GRAVITY;
    ball.v_x = (x - ball.x) / tg;
    ball.v_y = (y - ball.y) / tg;
    // debug_point("vxy", ball.v_x, ball.v_y);
    // debug_halt_next_frame();
}

/**
 * Sets the ball speed to reach a certain height at the net and a target bounce point
 */
void set_ball_velocity_net_height(int16_t x, int16_t y, uint8_t net_height)
{
    // debug_point("Target", x, y);
    // debug_point("Ball / net height", ball.height, net_height);
    // debug_draw_projection(x, y, COLOR_GREEN);
    // Distance to the target point
    float d = sqrt(sq(x - ball.x) + sq(y - ball.y));
    // Distance to the net (always positive)
    float d_net = d * (NET_REFERENCE_Y - ball.y) / (y - ball.y);

    // height at net at max (horizontal) speed ==> establishes the minimum height
    // mhs stands for max_horizontal_speed
    float mhs_time_to_ground = d / MAX_HORIZONTAL_SPEED;
    float mhs_time_to_net = d_net / MAX_HORIZONTAL_SPEED;
    float vert_speed_for_mhs = ball.height / mhs_time_to_ground - GRAVITY * mhs_time_to_ground / 2;
    float net_height_for_mhs = ball.height - vert_speed_for_mhs * mhs_time_to_net - GRAVITY * mhs_time_to_net * mhs_time_to_net / 2;

    // We preassign the max horizontal speed
    float v_horizontal = MAX_HORIZONTAL_SPEED;

    // But if target net_hegiht is higher, it is safe to calculate the speed
    if (net_height > net_height_for_mhs)
    {
        // denominator to calculate horizontal speed

        float denominator = 2 * (net_height * d - ball.height * (d - d_net));
        assert(denominator > 0, "Denominator must be positive");

        // Horizontal speed for target height
        v_horizontal = sqrt((GRAVITY * d_net * d * (d - d_net)) / denominator);
    }

    // Components of the speed
    ball.v_x = v_horizontal * (x - ball.x) / d;
    ball.v_y = v_horizontal * (y - ball.y) / d;
    ball.v_h = ball.height * v_horizontal / d - GRAVITY * d / (2 * v_horizontal);

#ifdef DEBUG
    // if (fabs(ball.v_h) > MAX_VERTICAL_SPEED || fabs(ball.v_y) < 0.01)
    // {
    //     debug_number("v_vertical", ball.v_h);
    //     debug_point("v_horizontal", ball.v_x, ball.v_y);
    //     assert(false, "Ball speed too high");
    // }
#endif
}

/**
 * Estimates the position of the ball (x and z) after a certain time has passed
 */
void ball_future_x_and_height(float time, float *future_x, float *future_z)
{
    *future_z = ball.height - ball.v_h * time - GRAVITY * sq(time) / 2;
    if (*future_z < 0)
    {
        // The time to bounce is obtained as a proportion of height of the ball and the total height run in the period
        float time_to_bounce = time * ball.height / (ball.height - *future_z);
        float speed_at_bounce = ball.v_h - GRAVITY * time_to_bounce;
        // TODO: apply surface type conditions... this should be a function
        float speed_after_bounce = -speed_at_bounce * GROUND_COEF_RESTITUTION;
        float time_after_bounce = time - time_to_bounce;
        *future_z = -speed_after_bounce * time_after_bounce - GRAVITY * sq(time_after_bounce) / 2;
    }

    *future_x = ball.x + ball.v_x * time;
}
#include <math.h>
#include <stdlib.h>
#include "debug.h"
#include "physics.h"
#include "ball.h"
#include "game_state.h"
#include "court_dimensions.h"
#include "messages.h"
#include "colors.h"
#include "debug.h"

#pragma codeseg BANK_8

// Otherwise, I'm getting an internal error of the compiler because of the optimizer
// #pragma opt_code_speed

#define sq(x) ((x) * (x))

Ball ball_at_ground_hit;
BallTrajectory ball_trajectory;

#ifdef DEBUG

// typedef struct
// {
//     int n_points;
//     IPoint points[4];
// } DebugBallTrajectory;

// DebugBallTrajectory debug_ball_trajectory;
#endif

void set_target_for_ball(float target_x, float target_y, float v_xy)
{
    float distance2 = sq(ball.x - target_x) + sq(ball.y - target_y);
    float distance = sqrt(distance2);
    float t = distance / v_xy;
    ball.v_x = (target_x - ball.x) / t;
    ball.v_y = (target_y - ball.y) / t;
    ball.v_h = (ball.height / t - GRAVITY * t / 2);
}

#define TARGET_AREA_HEIGHT (BASELINE_BOTTOM - SERVICE_BOX_BOTTOM)

void set_ball_speed_at_drive(uint8_t player, uint8_t strength, uint8_t precision)
{
    int16_t deviation_y = TARGET_AREA_HEIGHT * precision / 100;
    int16_t target_y = player == PLAYER_TOP ? BASELINE_BOTTOM - deviation_y : BASELINE_TOP + deviation_y;
    int16_t target_x = CENTER_LINE + ((rand() % 200) - 100) * SIDE_LINE_WIDTH / 200;

    float v_xy = 2 + strength * 0.3;

    set_target_for_ball(target_x, target_y, v_xy);
}

/** Discriminant part of the ball quadratic equation for the ball to reach a certain height */
float discrimant_for_height(Ball *ball, float height)
{
    float discriminant = sq(ball->v_h) - 2 * GRAVITY * (height - ball->height);
#ifdef DEBUG
    // if (discriminant < 0)
    // {
    //     debug_number("Discriminant negative", discriminant);
    //     debug_point("Ball", ball->x, ball->y);
    //     debug_point("Ball height / height", ball->height, height);
    //     debug_number("Ball speed", ball->v_h);
    //     assert(discriminant >= 0, "Discriminant for height negative");
    // }
#endif
    return discriminant;
}

void point_at_time(float time, Point *output)
{
    output->x = ball.x + ball.v_x * time;
    output->y = ball.y + ball.v_y * time;
}

/**
 * Calculates whether the ball hits the ground in the valid area. It updates the _ball_at_ground_hit variable if so
 */
bool will_ball_hit_ground(void)
{
    const Boundary *court_half = &court_boundaries[COURT_AREA_VALID_UPPER + game_state.ball_owner ^ 1];
    float discriminant = discrimant_for_height(&ball, 0);
    assert(discriminant >= 0, "Neg SQ: No solution to hit ground");
    float t_ground = (-ball.v_h + sqrt(discriminant)) / GRAVITY;
    ball_at_ground_hit.x = ball.x + ball.v_x * t_ground;
    ball_at_ground_hit.y = ball.y + ball.v_y * t_ground;
    ball_at_ground_hit.height = 0;
    ball_at_ground_hit.v_h = -(ball.v_h + GRAVITY * t_ground) * GROUND_COEF_RESTITUTION;
    ball_at_ground_hit.v_x = ball.v_x;
    ball_at_ground_hit.v_y = ball.v_y;

    // debug_draw_projection(ball_at_ground_hit.x, ball_at_ground_hit.y, COLOR_GREEN);

    bool hits_ground = xy_in_boundary(ball_at_ground_hit.x, ball_at_ground_hit.y, court_half);

    if (!hits_ground)
    {
        // debug_point("Ball won't hit ground", ball_at_ground_hit.x, ball_at_ground_hit.y);
        // debug_draw_projection(ball_at_ground_hit.x, ball_at_ground_hit.y, COLOR_RED);
    }

    return hits_ground;
}

void add_trajectory_point(Ball *ball, const Boundary *boundary, float time)
{
    if (time > 0)
    {
        const int16_t x = ball->x + ball->v_x * time;
        const int16_t y = ball->y + ball->v_y * time;
        // debug_point("Point", x, y);
        // debug_draw_projection(x, y, COLOR_RED);
        if (xy_in_boundary(x, y, boundary))
        {
            // debug_draw_projection(x, y, COLOR_GREEN);
            ball_trajectory.points[ball_trajectory.n_points].x = x;
            ball_trajectory.points[ball_trajectory.n_points].y = y;
            ball_trajectory.n_points++;

#ifdef DEBUG
            // }
            // else
            // {
            //     debug_ball_trajectory.points[debug_ball_trajectory.n_points].x = x;
            //     debug_ball_trajectory.points[debug_ball_trajectory.n_points].y = y;
            //     debug_ball_trajectory.n_points++;
#endif
        }
    }
}

/**
 * Calculates the points where the trajectory of the ball reaches a certain height
 * (up to four points). The results are stored in the ball_trajectory variable.
 *
 */
void trajectory_points(Ball *ball, const Boundary *boundary, uint8_t height)
{
    // debug_number("Target Height", height);
    const float discriminant = discrimant_for_height(ball, height);
    // Discriminant is negative if the ball is not going to reach the provided height
    if (discriminant >= 0)
    {
        // If the height is above the ball, it is possible to reach height twice (goind up and down)
        float discriminant_sqrt = sqrt(discriminant);
        add_trajectory_point(ball, boundary, (-ball->v_h - discriminant_sqrt) / GRAVITY);
        add_trajectory_point(ball, boundary, (-ball->v_h + discriminant_sqrt) / GRAVITY);
    }
}

/**
 * Calculates different points of the ball trajectory given a target height:
 */
BallTrajectory *ball_trajectory_points(uint8_t height)
{
    ball_trajectory.n_points = 0;
#ifdef DEBUG
    // debug_ball_trajectory.n_points = 0;
#endif

    // Only identify points before the ball hits the ground if it is not expected to go out
    if (will_ball_hit_ground())
    {
        const Boundary *court_half = &court_boundaries[COURT_AREA_VALID_UPPER + game_state.ball_owner ^ 1];
        trajectory_points(&ball, court_half, height);
    }
    const Boundary *full_court_half = &court_boundaries[COURT_AREA_UPPER + game_state.ball_owner ^ 1];
    trajectory_points(&ball_at_ground_hit, full_court_half, height * GROUND_COEF_RESTITUTION);

    if (ball_trajectory.n_points == 0)
    {
        // debug_message("No points");
        // debug_point("NP ground: ", ball_at_ground_hit.x, ball_at_ground_hit.y);
        // debug_draw_projection(ball_at_ground_hit.x, ball_at_ground_hit.y, COLOR_RED);
        // debug_halt_next_frame();
    }

    return &ball_trajectory;
}

/**
 * Calculates different points of the ball trajectory given a target height at service.
 * It assumes the ball hits a valid area and only points after bounce are considering
 */
BallTrajectory *ball_trajectory_points_at_service(uint8_t height)
{
    ball_trajectory.n_points = 0;
#ifdef DEBUG
    // debug_ball_trajectory.n_points = 0;
#endif
    // Ignore the results of the function, but we need the side effect of calculating the position of the ball bouncing
    will_ball_hit_ground();

    // debug_point("Ground", ball_at_ground_hit.x, ball_at_ground_hit.y);
    // debug_point("Speed", ball_at_ground_hit.v_x, ball_at_ground_hit.v_y);
    // debug_number("Speed V", ball_at_ground_hit.v_h);

    const Boundary *full_court_half = &court_boundaries[COURT_AREA_UPPER + game_state.ball_owner ^ 1];
    trajectory_points(&ball_at_ground_hit, full_court_half, height * GROUND_COEF_RESTITUTION);

    return &ball_trajectory;
}

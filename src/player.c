#include <stddef.h>
#include <math.h>
#include "player.h"
#include "ball.h"
#include "court_dimensions.h"
#include "game_loop.h"
#include "game_state.h"
#include "physics.h"
#include "score_board.h"
#include "player_states.h"
#include "player_transitions.h"
#include "player_events.h"
#include "debug.h"

// ---------------------------------------------------------
// Internal types
// ---------------------------------------------------------

#pragma codeseg BANK_8

typedef struct
{
    uint8_t index;
    uint8_t duration;
} AnimationFrame;

/**
 *  Player function definition, used for animation events and the player state machine
 */
typedef void (*PlayerFunc)(Player *player);

typedef struct
{
    // Number of frames in the animation (considering the first frame)
    uint8_t num_frames;
    // Animation
    AnimationFrame (*frames)[];
    // Events: pointer to an array of function events that take the player as argument
    PlayerFunc (*events)[];
} PlayerAnimation;

// ---------------------------------------------------------
// Animation macros and constants
// ---------------------------------------------------------

#define ANIMATION_DEF(name) {                                        \
    .num_frames = sizeof(name##_animation) / sizeof(AnimationFrame), \
    .frames = &name##_animation,                                     \
    .events = NULL,                                                  \
}

#define ANIMATION_DEF_WITH_EVENTS(name) {                            \
    .num_frames = sizeof(name##_animation) / sizeof(AnimationFrame), \
    .frames = &name##_animation,                                     \
    .events = &name##_events,                                        \
}

// ---------------------------------------------------------
// Animation definitions
// ---------------------------------------------------------

// Idle -------------------------------------------------

AnimationFrame idle_animation[] = {{PLAYER_FRAME_IDLE, 0}};

AnimationFrame service_wait_animation[] = {{PLAYER_FRAME_SERVICE_WAIT, 0}};

// Service ball up -----------------------------------------

AnimationFrame service_ball_up_animation[] = {
    {PLAYER_FRAME_SERVICE_BALL_UP, 10},
    {PLAYER_FRAME_SERVICE_BALL_UP + 1, 0}};

PlayerFunc service_ball_up_events[] = {
    event_serve_ball_up, NULL};

// Service hit --------------------------------------------
#define FR_DURATION_HIT_PREPARATION 5
#define FR_DURATION_HIT 5
#define FR_DURATION_POST_HIT1 5
#define FR_DURATION_POST_HIT2 10
#define FR_DURATION_POST_HIT3 5

AnimationFrame service_hit_animation[] = {
    {PLAYER_FRAME_SERVICE_HIT, 5},
    {PLAYER_FRAME_SERVICE_HIT + 1, 3},
    {PLAYER_FRAME_SERVICE_HIT + 2, 4},
    {PLAYER_FRAME_SERVICE_HIT + 3, 5}};

PlayerFunc service_hit_events[] = {
    event_serve_hit_ball, NULL, NULL, event_serve_hit_end_animation};

// Drive --------------------------------------------------

AnimationFrame drive_animation[] = {
    {PLAYER_FRAME_DRIVE, FR_DURATION_HIT_PREPARATION},
    {PLAYER_FRAME_DRIVE + 1, FR_DURATION_HIT},
    {PLAYER_FRAME_DRIVE + 2, FR_DURATION_POST_HIT1},
    {PLAYER_FRAME_DRIVE + 3, FR_DURATION_POST_HIT2},
    {PLAYER_FRAME_DRIVE + 2, FR_DURATION_POST_HIT3}};

PlayerFunc drive_events[] = {
    event_hit_ball, NULL, NULL, NULL, transition_idle};

// Backhand -----------------------------------------------

AnimationFrame backhand_animation[] = {
    {PLAYER_FRAME_BACKHAND, FR_DURATION_HIT_PREPARATION},
    {PLAYER_FRAME_BACKHAND + 1, FR_DURATION_HIT},
    {PLAYER_FRAME_BACKHAND + 2, FR_DURATION_POST_HIT1},
    {PLAYER_FRAME_BACKHAND + 3, FR_DURATION_POST_HIT2},
    {PLAYER_FRAME_BACKHAND + 2, FR_DURATION_POST_HIT3}};

PlayerFunc backhand_events[] = {
    event_hit_ball, NULL, NULL, NULL, transition_idle};

// Walk ---------------------------------------------
AnimationFrame walk_animation[] = {
    {PLAYER_FRAME_WALK, WALK_DURATION},
    {PLAYER_FRAME_WALK + 1, WALK_DURATION},
    {PLAYER_FRAME_WALK + 2, WALK_DURATION},
    {PLAYER_FRAME_WALK + 3, WALK_DURATION}};

// Smash ---------------------------------------------
AnimationFrame smash_animation[] = {
    {PLAYER_FRAME_SERVICE_HIT, FR_DURATION_HIT_PREPARATION},
    {PLAYER_FRAME_SERVICE_HIT + 1, FR_DURATION_HIT},
    {PLAYER_FRAME_SERVICE_HIT + 2, FR_DURATION_POST_HIT1},
    {PLAYER_FRAME_SERVICE_HIT + 3, FR_DURATION_POST_HIT2}};

PlayerFunc smash_events[] = {
    event_hit_ball, NULL, NULL, transition_idle};

// Touched ---------------------------------------------
AnimationFrame touched_animation[] = {
    {PLAYER_FRAME_TOUCH, 0}};

// All animations

PlayerAnimation animations[PS_MAX] = {
    ANIMATION_DEF(idle),
    ANIMATION_DEF(service_wait),
    ANIMATION_DEF_WITH_EVENTS(service_ball_up),
    ANIMATION_DEF_WITH_EVENTS(service_hit),
    ANIMATION_DEF_WITH_EVENTS(drive),
    ANIMATION_DEF_WITH_EVENTS(backhand),
    ANIMATION_DEF(walk),
    ANIMATION_DEF_WITH_EVENTS(smash),
    ANIMATION_DEF(touched)};

// State machine
PlayerFunc stateMachine[PS_MAX] = {
    [PS_IDLE] = player_idle,
    [PS_SERVICE_READY] = player_service_ready,
    [PS_SERVICE_BALL_UP] = player_service_ball_up,
    [PS_SERVICE_BALL_HIT] = player_service_ball_hit,
    [PS_DRIVE] = player_no_action,
    [PS_BACKHAND] = player_no_action,
    [PS_WALK] = player_walk,
    [PS_SMASH] = player_no_action,
    [PS_TOUCH] = player_no_action};

// ---------------------------------------------------------
// Player variables
// ---------------------------------------------------------

Player player_top = {
    .id = PLAYER_TOP,
    .state = PS_IDLE,
    .frame_offset = 0,
    .frame_timer = 0,
    .frame_index = 0,
    .control_fn = NULL,
    .x = CENTER_LINE_LEFT - PLAYER_WIDTH / 2,
    .y = BASELINE_TOP - 1,
    .name = "FedExpress"};

Player player_bottom = {
    .id = PLAYER_BOTTOM,
    .state = PS_IDLE,
    .frame_offset = 0,
    .frame_timer = 0,
    .frame_index = 0,
    .control_fn = NULL,
    .x = CENTER_LINE_RIGHT + PLAYER_WIDTH / 2,
    .y = BASELINE_BOTTOM + 1,
    .name = "PLAYER 1"};

// ---------------------------------------------------------
// Private auxiliar functions
// ---------------------------------------------------------

/**
 * Updates the frame index in the status of the player and calls necessary events
 */
void player_next_frame(Player *player)
{
    PlayerAnimation *animation = &animations[player->state];
    // If there is an event associated with the current ending frame, we will call it later
    PlayerFunc frameEndEvent = NULL;
    if (animation->events != NULL)
        frameEndEvent = (*animation->events)[player->frame_offset];

    player->frame_offset++;
    if (player->frame_offset >= animation->num_frames)
        player->frame_offset = 0;

    AnimationFrame *frame = &(*animation->frames)[player->frame_offset];
    player->frame_timer = frame->duration;
    player->frame_index = frame->index;

    // If there is an event associated with the transition to the new frame, we call it
    if (frameEndEvent != NULL)
        (*frameEndEvent)(player);
}

/**
 * Updates the player status
 */
void update_player_frame(Player *player)
{
    // If frame_timer is 0, there is nothing to do...
    // Otherwise, we decrement the timer and check if we have to change the frame
    if (player->frame_timer != 0 && --player->frame_timer == 0)
    {
        player_next_frame(player);
    }
}

/**
 * Changes the status of one of the players
 */
void set_player_status(Player *player, PlayerState state)
{
    AnimationFrame *frame = &(*animations[state].frames)[0];
    player->state = state;
    player->frame_offset = 0;
    player->frame_timer = frame->duration;
    player->frame_index = frame->index;
}

void read_player_controls(Player *player)
{
    ControlInput control = player->control_fn(player->id);
    ControlInput fire_on = control & (CONTROL_A | CONTROL_B | CONTROL_C);
    if (fire_on)
    {
        if (player->control & CONTROL_FIRE)
        {

            // Fire was already pressed in the last check... we will not trigger it again
            player->control = (control & CONTROL_DIRECTIONS) | CONTROL_FIRE;
        }
        else
        {
            // Fire was not pressed in the last check... we will trigger it now and prevent it from being triggered again
            player->control = control | CONTROL_FIRE;
        }
    }
    else
    {
        // No fire button is pressed... we will reset the fire flag
        player->control = control;
    }
}

// ---------------------------------------------------------
// Update functions
// ---------------------------------------------------------

/**
 * Update the state of an individual player
 */
void update_player(Player *player)
{
    PlayerFunc func = stateMachine[player->state];
    read_player_controls(player);
    func(player);
    update_player_frame(player);
}

/**
 * Updates the state of the players
 */
void update_players(void)
{
    update_player(&player_top);
    update_player(&player_bottom);
}

Player *get_player(PlayerId player_id)
{
    return player_id == PLAYER_TOP ? &player_top : &player_bottom;
}

// ------------------------------------------------------------------------------
// Ball reach functions
// ------------------------------------------------------------------------------

#define HIT_FACTOR 1

#define MINIMUM_READY_TO_CHECK_DISTANCE_Y_AXIS (30 * HIT_FACTOR)
#define MINIMUM_READY_TO_HIT_DISTANCE_Y_AXIS 20

#define MINIMUM_HEIGHT_TO_DRIVE_HIT 2
#define MAXIMUM_HEIGHT_TO_DRIVE_HIT 45
#define MAXIMUM_X_RANGE_TO_DRIVE_HIT (40 * HIT_FACTOR)
// The slack allows a certain margin of error to allow the player to hit the drive if the ball is slightly at the left
// side of the center of the player (or right side if backhand)
#define X_RANGE_SLACK_TO_DRIVE_HIT (5 * HIT_FACTOR)
#define Y_RANGE_SLACK_TO_HIT (5 * HIT_FACTOR)
#define Y_RANGE_SLACK_AT_NET 5

#define MINIMUM_HEIGHT_TO_SMASH 30 // Could have some overlap if needed
#define MAXIMUM_HEIGHT_TO_SMASH 90
#define MAXIMUM_X_RANGE_TO_SMASH (25 * HIT_FACTOR)
// #define MINIMUM_HIT_DISTANCE_X_AXIS 20
// #define MINIMUM_HIT_DISTANCE_Z_AXIS 20

/** Original distance2 = 1000
 *  Ref points:
 *  Drive and backhand: X range [-24, 21]  Z - range: [6, 14]
 *  Smash: X range [-2, 3], Z range [55-55]
    [REF_POINT_SERVICE] = {{3, 0, 55}, {-2, -1, 55}},
    [REF_POINT_DRIVE] = {{-24, 0, 9}, {21, -1, 6}},
    [REF_POINT_BACKHAND] = {{21, 0, 14}, {-15, -1, 11}},

    Raquet size: 10 x 10 approx

    Simplification...
    - First check the distance on the y axis... common for all:AIR_DRAG
        - ball.y is over or has passed the net.
        - Distance y < MINIMUM_HIT_DISTANCE_Y_AXIS / MINIMUM_READY_TO_HIT_DISTANCE_Y_AXIS

    - Then, the position of the ball when reaching the player is calculated
    - Drive and backhand:
        - MINIMUM_HEIGHT_TO_HIT: Although in the hit we will consider a minimum height (e.g. >= 2), to prepare the player for hit we won't
        - MAXIMUM_HEIGHT_TO_DRIVE_HIT: Not above a certain height, which should be the same as the minimum for
        - Up to a certain height: MAXIMUM

        >taking up to a certain height
        for both drive and backhand and a certain in X range
        - ball.height > MINIMUM_HEIGHT_FOR_HIT
        - are chosen base on the X offet (0) from the player
    - Smash:
        - above the top height of drive and backhand and till a certain height
        - A range around X



};

 */

/** Checks whether the ball is at reach for the player */
// bool ball_at_reach(Player *player)
// {
//     // y distance

//     const RefPoint *ref_point = &ref_points[ref_point_type][player->id];
//     const float distance2 = distance_ball_raquet(player, ref_point);

//     if (distance2 < MINIMUM_HIT_DISTANCE)
// }

// Returns drive of backhand given the position of the player in the field
PlayerState simple_hit_reponse(Player *player)
{
    int8_t sign = player->id == PLAYER_BOTTOM ? 1 : -1;
    const int x_from_center = player->x - CENTER_LINE;
    return x_from_center * sign > 0 ? PS_DRIVE : PS_BACKHAND;
}

/** Identifies the closes type of hit given the position of the ball in relation to the player */
PlayerState closest_type_of_hit(Player *player)
{
    int8_t sign = player->id == PLAYER_BOTTOM ? 1 : -1;
    float player_distance_to_ball = (player->y - ball.y) * sign; // Positive if the ball has not reached the player yet
    // debug_number("Dist to ball", player_distance_to_ball);

    // Either the ball has passed the player or not approaching the player
    if (player_distance_to_ball < -Y_RANGE_SLACK_TO_HIT || ball.v_y * sign < 0)
        return simple_hit_reponse(player);

    // Now, we obtain its position in the next frame
    float time_to_hit = game_frame_speed * FR_DURATION_HIT_PREPARATION;
    float ball_future_x, ball_future_height;
    ball_future_x_and_height(time_to_hit, &ball_future_x, &ball_future_height);

    // debug_point("Future_x_h", ball_future_x, ball_future_height);

    // High ball... tries to smash
    if (ball_future_height > MAXIMUM_HEIGHT_TO_DRIVE_HIT)
        return PS_SMASH;

    // Checks on x range
    float x_distance = ball_future_x - player->x;
    return (x_distance * sign > 0) ? PS_DRIVE : PS_BACKHAND;
}

/** Checks whether the ball is at reach for the player. To be used at the moment of hitting the ball */
bool ball_at_reach(Player *player)
{
    int8_t sign = player->id == PLAYER_BOTTOM ? 1 : -1;
    int distance_to_net = ((int)ball.y - NET_REFERENCE_Y) * sign; // Positive if ball in player's part of the court
    // Ball is still not on our side of the court
    if (distance_to_net < -Y_RANGE_SLACK_AT_NET)
        return false;

    float player_distance_to_ball = (player->y - ball.y) * sign; // Positive if the ball has not reached the player yet
    if (player_distance_to_ball < -Y_RANGE_SLACK_TO_HIT || player_distance_to_ball > MINIMUM_READY_TO_HIT_DISTANCE_Y_AXIS)
        return false; // Either the ball has passed the player or not close enough

    // Checks on x range
    float x_distance = ball.x - player->x;
    float abs_x_distance = fabs(x_distance);

    // SMASH check...
    if (player->state == PS_SMASH)
    {
        return ball.height >= MINIMUM_HEIGHT_TO_SMASH && ball.height <= MAXIMUM_HEIGHT_TO_SMASH &&
               abs_x_distance <= MAXIMUM_X_RANGE_TO_SMASH;
    }
    else if (ball.height >= MINIMUM_HEIGHT_TO_DRIVE_HIT && ball.height <= MAXIMUM_HEIGHT_TO_DRIVE_HIT &&
             abs_x_distance <= MAXIMUM_X_RANGE_TO_DRIVE_HIT)
    {
        // TODO: add some slack
        return player->state == PS_DRIVE ? x_distance * sign > -X_RANGE_SLACK_TO_DRIVE_HIT
                                         : x_distance * sign < X_RANGE_SLACK_TO_DRIVE_HIT;
    }

    return false;
}

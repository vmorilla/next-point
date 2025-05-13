#include <stdlib.h>
#include <math.h>
#include "computer_controls.h"
#include "player.h"
#include "game_state.h"
#include "ball.h"
#include "player_ref_points.h"
#include "court_dimensions.h"
#include "physics.h"
#include "colors.h"
#include "debug.h"

#define COMPUTER_REACTION_TIME 20
#define POSITIONING_NUMBER_OF_MOVES 2
// TODO: the positioning function can be made dependent on how aggressive the player is to go to the net
#define POSITIONING_OPPONENT_DISTANCE ((BASELINE_BOTTOM - BASELINE_TOP) / 1.2)
#define TARGET_BALL_HEIGHT_FOR_STANDARD_HIT 16
#define TARGET_BALL_HEIGHT_FOR_SERVICE_HIT 20

typedef enum
{
    COMPUTER_PLAYER_IDLE,           // Default state
    COMPUTER_PLAYER_WAIT_TO_SERVE,  // The player is waiting to serve
    COMPUTER_PLAYER_SERVE,          // Ball tossed up in serve: the player is going to hit the ball
    COMPUTER_PLAYER_POSITION,       // The player has hit the ball and is going to position himself
    COMPUTER_PLAYER_APPROACH_BALL,  // The goes to intercept the ball
    COMPUTER_PLAYER_WAIT_FOR_SERVE, // The player is waiting for the opponent to serve
    COMPUTER_PLAYER_RECEIVE_SERVE,  // The player is going to hit the ball from the serve
    COMPUTER_PLAYER_MAX,
} ComputerPlayerState;

typedef ControlInput (*ComputerPlayerStateFn)(PlayerId playerId);
typedef void (*ComputerPlayerTransitionFn)(PlayerId playerId);

// ---------------------------------------------------------
// Computer player function prototypes
// ---------------------------------------------------------

void computer_transition_idle(PlayerId playerId);
void computer_transition_wait_to_serve(PlayerId playerId);
void computer_transition_serve(PlayerId playerId);
void computer_transition_position(PlayerId playerId);
void computer_transition_approach_ball(PlayerId playerId);

void computer_transition_wait_for_serve(PlayerId playerId);
void computer_transition_receive_serve(PlayerId playerId);

ControlInput computer_state_idle(PlayerId playerId);
ControlInput computer_state_wait_to_serve(PlayerId playerId);
ControlInput computer_state_serve(PlayerId playerId);
ControlInput computer_state_position(PlayerId playerId);
ControlInput computer_state_approach_ball(PlayerId playerId);

ControlInput computer_state_wait_for_serve(PlayerId playerId);
// Equivalent to approach ball but for serve
ControlInput computer_state_receive_serve(PlayerId playerId);

typedef struct
{
    uint8_t countdown;
} WaitToServeStateData;

// Serve
typedef struct
{
    uint8_t target_height;
} ServeStateData;

typedef struct
{
    int target_x;
    int target_y;
    uint8_t n_moves;            // Number of times a target has been fixed... to avoid many small movements
    uint8_t reaction_countdown; // Timer to wait before moving to the target
} PositionStateData;

typedef struct
{
    ComputerPlayerStateFn stateFn;
    union
    {
        // TODO: report bug z88dk / sdcc... the size allocated is the one of the first structure (should be the largets)
        PositionStateData position;
        WaitToServeStateData wait_to_serve;
        ServeStateData serve;
    };

} ComputerPlayer;

ComputerPlayer computer_state[2] = {
    {
        .stateFn = computer_state_idle,
    },
    {
        .stateFn = computer_state_idle,
    }};

// ---------------------------------------------------------
// Computer player state functions
// ---------------------------------------------------------

ControlInput read_computer_control(uint8_t player_id)
{
    ComputerPlayer *computer = &computer_state[player_id];
    return computer->stateFn(player_id);
}

// ---------------------------------------------------------
// Idle state
// ---------------------------------------------------------

void computer_transition_idle(PlayerId player_id)
{
    // debug_message("CT IDLE");
    ComputerPlayer *computer = &computer_state[player_id];
    computer->stateFn = computer_state_idle;
}

ControlInput computer_state_idle(PlayerId playerId)
{
    Player *player = get_player(playerId);
    Player *opponent = get_player(playerId ^ 1);
    // Checking for the game_state to react
    if (game_state.point_status == POS_SERVICE_START)
    {
        // The player is waiting for the ball to be served
        if (player->state == PS_SERVICE_READY)
        {
            // The player is the one serving
            computer_transition_wait_to_serve(playerId);
        }
        else if (opponent->state == PS_SERVICE_READY)
        {
            // The player is waiting for the opponent to serve
            computer_transition_wait_for_serve(playerId);
        }
    }
    return 0;
}

// ---------------------------------------------------------
// Wait to serve (wait to throw the ball)
// ---------------------------------------------------------

void computer_transition_wait_to_serve(PlayerId player_id)
{
    // debug_message("CT WAIT TO SERVE");
    ComputerPlayer *computer = &computer_state[player_id];
    // Timer to wait before serving
    computer->wait_to_serve.countdown = rand() % 50 + 50;
    computer->stateFn = computer_state_wait_to_serve;
}

ControlInput computer_state_wait_to_serve(PlayerId playerId)
{
    ComputerPlayer *computer = &computer_state[playerId];
    if (--computer->wait_to_serve.countdown > 0)
    {
        return 0;
    }
    else
    {
        // Ball up
        computer_transition_serve(playerId);
        return CONTROL_A;
    }
}

// ---------------------------------------------------------
// Serve (hit the ball)
// ---------------------------------------------------------

void computer_transition_serve(PlayerId player_id)
{
    // debug_message("CT SERVE");
    ComputerPlayer *computer = &computer_state[player_id];
    computer->stateFn = computer_state_serve;
    // Uses the countdown as the reference for the height of the ball to hit
    computer->serve.target_height = ref_points[REF_POINT_SERVICE][player_id].height + 14;
}

ControlInput computer_state_serve(PlayerId player_id)
{
    ComputerPlayer *computer = &computer_state[player_id];

    // The player has not tossed the ball yet
    if (ball.hidden)
        return 0;

    // Ball is in the air
    if (ball.v_h > 0 && ball.height <= computer->serve.target_height)
    {
        computer_transition_position(player_id);
        return CONTROL_A;
    }
    else
        return 0;
}

// ---------------------------------------------------------
// Position to be better placed in the court (ball still not hit by the opponent)
// ---------------------------------------------------------

void computer_transition_position(PlayerId player_id)
{
    // debug_message("CT POSITION");
    ComputerPlayer *computer = &computer_state[player_id];
    computer->stateFn = computer_state_position;

    computer->position.reaction_countdown = COMPUTER_REACTION_TIME;
    computer->position.n_moves = POSITIONING_NUMBER_OF_MOVES;
}

/**
 * Calculates the target position of the player considering the position of the opponent
 */
void calculate_player_positioning(PlayerId player_id)
{
    ComputerPlayer *computer = &computer_state[player_id];
    Player *opponent = get_player(player_id ^ 1);
    computer->position.target_x = (opponent->x - CENTER_LINE) / 1.5 + CENTER_LINE;
    computer->position.target_y = +opponent->y + (player_id == PLAYER_TOP ? -POSITIONING_OPPONENT_DISTANCE : POSITIONING_OPPONENT_DISTANCE);
    computer->position.n_moves--;

    // debug_point("COMP TARGET", computer->position.target_x, computer->position.target_y);
}

/**
 * Moves the player to the target position
 * The function returns the control input to be sent to the player
 */
ControlInput move_to_target(PlayerId player_id)
{
    ComputerPlayer *computer = &computer_state[player_id];
    Player *player = get_player(player_id);
    uint8_t control = 0;
    int diff_x = computer->position.target_x - player->x;
    int diff_y = computer->position.target_y - player->y;

    // TODO: report |= not working and driving me crazy
    if (diff_x >= 2)
        control = CONTROL_RIGHT;
    else if (diff_x <= -2)
        control = CONTROL_LEFT;
    if (diff_y >= 2)
        control = control |= CONTROL_DOWN;
    else if (diff_y <= -2)
        control = control |= CONTROL_UP;

    return control;
}

ControlInput computer_state_position(PlayerId player_id)
{
    ComputerPlayer *computer = &computer_state[player_id];
    if (computer->position.reaction_countdown > 0)
    {
        computer->position.reaction_countdown--;
        if (computer->position.reaction_countdown == 0)
            calculate_player_positioning(player_id);
        return 0;
    }

    ControlInput control = move_to_target(player_id);

    // Target reached... we might reposition the player another time
    if (control == 0 && computer->position.n_moves > 0)
        calculate_player_positioning(player_id);

    // Check if the ball owner changes (the opposite has hit the ball)
    if (game_state.ball_owner != player_id)
        // Now the player has to move to the ball
        computer_transition_approach_ball(player_id);

    return control;
}

// ---------------------------------------------------------
// Approach the ball (the player is going to hit the ball)
// ---------------------------------------------------------

/**
 * Internal function to calculate the target position of the player considering the trajectory of the ball
 * Different strategies could be implemented depending on the player, but, for the moment, we will consider
 * what is the closes point to the current position of the player where the ball reaches a certain height and
 * then we decide which stroke to use.
 *
 * The function returns false if the ball won't hit the valid area of the ground, so, we let it bounce
 * TODO: case of smash
 */
bool calculate_position_to_receive_ball(PlayerId player_id, bool is_serve)
{
    ComputerPlayer *computer = &computer_state[player_id];
    Player *player = get_player(player_id);

    BallTrajectory *trajectory = is_serve ? ball_trajectory_points_at_service(TARGET_BALL_HEIGHT_FOR_SERVICE_HIT)
                                          : ball_trajectory_points(TARGET_BALL_HEIGHT_FOR_STANDARD_HIT);

    // The ball is going to bounce out or we do not have a valid trajectory
    if (trajectory->n_points == 0)
        return false;

    int8_t point_selected = -1;
    uint16_t point_selected_distance = 0xFFFF;

    for (uint8_t point = 0; point < trajectory->n_points; point++)
    {
        int x_distance = trajectory->points[point].x - player->x;
        int y_distance = trajectory->points[point].y - player->y;

        uint16_t distance = x_distance * x_distance + y_distance * y_distance;
        if (distance < point_selected_distance)
        {
            point_selected = point;
            point_selected_distance = distance;
        }
    }

    computer->position.target_x = trajectory->points[point_selected].x;
    computer->position.target_y = trajectory->points[point_selected].y;

    // debug_draw_projection(computer->position.target_x, computer->position.target_y, COLOR_PINK);

    return true;
}

void computer_transition_approach_ball(PlayerId playerId)
{
    // debug_message("CT APPROACH BALL");

    ComputerPlayer *computer = &computer_state[playerId];
    computer->stateFn = computer_state_approach_ball;
    // Reuses the data of the position state
    computer->position.reaction_countdown = 5; // COMPUTER_REACTION_TIME;
}

ControlInput computer_state_approach_ball(PlayerId playerId)
{
    ComputerPlayer *computer = &computer_state[playerId];

    if (computer->position.reaction_countdown > 0)
    {
        computer->position.reaction_countdown--;
        if (computer->position.reaction_countdown == 0)
        {
            if (!calculate_position_to_receive_ball(playerId, false))
                computer_transition_idle(playerId);
        }
        return 0;
    }

    Player *player = get_player(playerId);

    // Ball at reach?
    int distance_to_ball_y = player->y - ball.y;
    if (distance_to_ball_y < 0)
        distance_to_ball_y = -distance_to_ball_y;

    if (distance_to_ball_y < 15)
    {
        // The player is going to hit the ball
        computer_transition_position(playerId);
        return CONTROL_A;
    }
    else
        return move_to_target(playerId);
}

// ---------------------------------------------------------
// Wait for serve (wait for the opponent to serve)
// ---------------------------------------------------------

void computer_transition_wait_for_serve(PlayerId player_id)
{
    // debug_message("CT WAIT FOR SERVE");

    ComputerPlayer *computer = &computer_state[player_id];
    computer->stateFn = computer_state_wait_for_serve;
}

ControlInput computer_state_wait_for_serve(PlayerId player_id)
{
    ComputerPlayer *computer = &computer_state[player_id];
    if (game_state.point_status == POS_SERVICE_HIT)
    {
        // The player is going to hit the ball
        computer_transition_receive_serve(player_id);
    }
    return 0;
}

// ---------------------------------------------------------
// Receive serve (the opponent has served the ball)
// ---------------------------------------------------------

void computer_transition_receive_serve(PlayerId player_id)
{
    // debug_message("CT RECEIVE SERVE");
    ComputerPlayer *computer = &computer_state[player_id];
    computer->stateFn = computer_state_receive_serve;
    // Reuses the data of the position state
    computer->position.reaction_countdown = 5; // COMPUTER_REACTION_TIME;
}

ControlInput computer_state_receive_serve(PlayerId player_id)
{
    ComputerPlayer *computer = &computer_state[player_id];
    if (computer->position.reaction_countdown > 0)
    {
        computer->position.reaction_countdown--;
        if (computer->position.reaction_countdown == 0)
        {
            if (!calculate_position_to_receive_ball(player_id, true))
            {
                // debug_message("No points");
                computer_transition_idle(player_id);
            }
        }
        return 0;
    }

    Player *player = get_player(player_id);

    // Ball at reach?
    int distance_to_ball_y = player->y - ball.y;
    if (distance_to_ball_y < 0)
        distance_to_ball_y = -distance_to_ball_y;

    if (distance_to_ball_y < 15)
    {
        // The player is going to hit the ball
        computer_transition_position(player_id);
        return CONTROL_A;
    }
    else
        return move_to_target(player_id);
}

// ---------------------------------------------------------
// Public functions
// ---------------------------------------------------------

void restart_computer_controls(void)
{
    computer_transition_idle(PLAYER_TOP);
    computer_transition_idle(PLAYER_BOTTOM);
    // computer_state[PLAYER_TOP].stateFn = computer_state_idle;
    // computer_state[PLAYER_BOTTOM].stateFn = computer_state_idle;
}

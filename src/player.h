#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <stdint.h>
#include <stdbool.h>
#include "controls.h"

// ---------------------------------------------------------
// Common constants
// ---------------------------------------------------------

#define MINIMUM_HIT_DISTANCE 1000 // Minimum square distance to hit the ball

// ---------------------------------------------------------
// Data types
// ---------------------------------------------------------
typedef enum
{
    PLAYER_TOP,
    PLAYER_BOTTOM,
} PlayerId;

/**
 * Player state enumeration
 */
typedef enum
{
    PS_IDLE,
    PS_SERVICE_READY,
    PS_SERVICE_BALL_UP,
    PS_SERVICE_BALL_HIT,
    PS_DRIVE,
    PS_BACKHAND,
    PS_WALK,
    PS_SMASH,
    PS_TOUCH,
    PS_MAX,
} PlayerState;

/**
 * Indexes of the frames of the animations in the sprite sheet
 */
typedef enum
{
    PLAYER_FRAME_IDLE = 0,
    PLAYER_FRAME_SERVICE_WAIT = 1,
    PLAYER_FRAME_SERVICE_BALL_UP = 2,
    PLAYER_FRAME_SERVICE_HIT = 4,
    PLAYER_FRAME_DRIVE = 8,
    PLAYER_FRAME_BACKHAND = 12,
    PLAYER_FRAME_WALK = 16,
    PLAYER_FRAME_TOUCH = 20,
} PlayerFrames;

/**
 * Complete variable state of a player
 */
typedef struct
{
    PlayerId id;
    ControlInput control;        // Current state of the input
    ControlInput control_at_hit; // Records the state of the input when the player press hit
    PlayerState state;
    uint8_t frame_offset; // Offset in the animation (from 0 to num_frames - 1)
    uint8_t frame_timer;  // Timer to change the frame
    uint8_t frame_index;  // Absolute index of the current frame in the animation
    ControlInputFn control_fn;
    uint8_t state_timer; // Timer used in certain states
    float x;
    float y;
    char *name; // Name of the player
} Player;

/** Variables with the state of both players */
extern Player player_top;
extern Player player_bottom;

/** Common constants */
#define WALK_DURATION 10

/**
 * Updates the state of the players
 */
void update_players(void);

Player *get_player(PlayerId player_id);

// ---------------------------------------------------------
// Externally called player transition functions
// ---------------------------------------------------------

// For testing purposes only
/**
 * Sets the state of the player and updates its sprite frame
 */
void set_player_status(Player *player, PlayerState state);

/** Checks whether the ball is at reach for the player hit */
bool ball_at_reach(Player *player);

/** Identifies the closes type of hit given the position of the ball in relation to the player */
PlayerState closest_type_of_hit(Player *player);

#endif
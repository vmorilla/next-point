/**
 * Defines data structures and global variables necessaries to maintain the game state.
 * This includes the logical coodinates of the players and the ball in the court.
 */
#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

#include <stdint.h>
#include <stdbool.h>
#include "player.h"
#include "court_dimensions.h"

/**
 * Game logical state data structures
 */

typedef enum
{
    POS_SERVICE_START,       // Player is going to serve
    POS_SERVICE_HIT_ATTEMPT, // Player has attempted to hit the ball during service
    POS_SERVICE_HIT,         // Player has hit to service
    POS_HIT,                 // Player has hit a regular point
    POS_BOUNCED_IN,          // Ball has bounced outside the valid area
    POS_END_MATCH
} PointStatus;

typedef enum
{
    GAME_ZERO,
    GAME_FIFTEEN,
    GAME_THIRTY,
    GAME_FORTY,
    GAME_ADVANTAGE,
} GamePoint;

typedef struct
{
    uint8_t sets[2];
    uint8_t games[2];
    GamePoint points[2];
} Score;

typedef struct
{
    uint8_t n_games;
    uint8_t n_sets;
} MatchType;

// TODO: internal structure
typedef struct
{
    PointStatus point_status; // Status of the current point
    PlayerId ball_owner;      // Player who last hit the ball... used to determine who wins the point
    PlayerId service_owner;   // Player who is serving in the current game. First time initialized randomly
    bool second_service;      // True if the player failed the first service
    bool touched_net;         // True if the ball touched the net... necessary to determine if service is repeated
    PlayerId touched_ball;    // if there's been a touch, it indicates the player who touched the ball
    uint8_t fadeout_timer;    // Used in certain states to leave a period before changing status
    void (*fadeout_fn)(void); // Function to be called when the fadeout timer reaches 0
} GameState;

/**
 * Update methods
 */

/**
 * Updates the game state and the entities in the game.
 */
void update_entities(void);

// --------------------------------------------------------------------
// Reaction to events triggered externally (by the ball or the player)
// --------------------------------------------------------------------

/**
 * Event triggered when the ball bounces
 */
void event_ball_bounced(void);

/**
 * Event triggered when a player hits the ball
 */
void event_player_hit(PlayerId player);

/**
 * Event triggered when the ball touches the net
 */
void event_net_touched(void);

/**
 * Event triggered when the ball touches a player
 */
void event_player_touch(Player *player);

// --------------------------------------------------------------------
// Game state transitions called externally
// --------------------------------------------------------------------

/**
 * Start of a new match
 */
void start_match(void);

// --------------------------------------------------------------------
// Utility functions
// --------------------------------------------------------------------

/**
 * True if we are in an ad point (and thus, service is the ad service area)
 */
bool is_ad_service(void);

/**
 * Pointer to the player who is serving
 */
Player *get_serving_player(void);

/**
 * Returns the valid service area for the serving player
 */
const Boundary *service_area(void);

/**
 * Returns the valid are for a given player (the area where the ball must bounce after hitting)
 */
const Boundary *valid_area(PlayerId player_id);

// --------------------------------------------------------------------
// Global variables
// --------------------------------------------------------------------

extern Score score;
extern MatchType match_type;
extern GameState game_state;

#endif

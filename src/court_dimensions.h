#ifndef _COURT_DIMENSIONS_H_
#define _COURT_DIMENSIONS_H_

#include <stdint.h>
#include <stdbool.h>
#include "geometry.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 256

#define VISIBLE_SCREEN_WIDTH 256
#define VISIBLE_SCREEN_HEIGHT 192

// All lines coordinates are the last pixel of the line... So, to check if the ball is
// inside the boundaries <= or >= must be used

#define COURT_TOP 74
#define COURT_BOTTOM 272
#define COURT_BOTTOM_FOR_PLAYER 253 // To temporarily avoid blinking effect of the sprites
#define COURT_WIDTH 350
#define COURT_LEFT ((SCREEN_WIDTH - COURT_WIDTH) / 2)
#define COURT_RIGHT (COURT_LEFT + COURT_WIDTH)
#define COURT_HEIGHT (COURT_BOTTOM - COURT_TOP)

#define BASELINE_TOP (99 + 1)
#define BASELINE_BOTTOM (219 + 1)
#define VALID_COURT_AREA_HEIGHT (BASELINE_BOTTOM - BASELINE_TOP)

#define SERVICE_BOX_TOP (129 + 1)
#define SERVICE_BOX_BOTTOM 190

#define SIDE_LINE_WIDTH 203
#define LEFT_SIDE_LINE ((SCREEN_WIDTH - SIDE_LINE_WIDTH) / 2 + 2)
#define RIGHT_SIDE_LINE ((LEFT_SIDE_LINE + SIDE_LINE_WIDTH) + 2 - 4)

// Position where the net is drawn
#define NET_POSITION_Y (BASELINE_TOP + (BASELINE_BOTTOM + 1 - BASELINE_TOP) / 2)
#define NET_POSITION_X (SCREEN_WIDTH / 2)

// Position for the umpire
#define UMPIRE_CHAIR_X (COURT_LEFT + 50)
#define UMPIRE_CHAIR_Y (NET_POSITION_Y)
#define UMPIRE_HEAD_X (UMPIRE_CHAIR_X + 5)
#define UMPIRE_HEAD_Y (UMPIRE_CHAIR_Y - 53)

// The reference is different than the visualization of the net.
// All checks of valid balls should use this reference
#define NET_REFERENCE_Y 160

// The maximum height of the net at the borders
#define MAXIMUM_NET_HEIGHT 18

// The minimum height of the net at the center
#define MINIMUM_NET_HEIGHT 16

/**
 * Extremes where the ball can bounce in the center line during service
 * For the bottom player service:
 *   - Deuce service: X <= CENTER_LINE_RIGHT
 *  - Ad service:     X >= CENTER_LINE_LEFT
 */
#define CENTER_LINE_LEFT 159
#define CENTER_LINE_RIGHT 163
#define CENTER_LINE ((CENTER_LINE_RIGHT + CENTER_LINE_LEFT) / 2)

#define SERVICE_BOX_WIDTH (RIGHT_SIDE_LINE - CENTER_LINE_RIGHT)
#define SERVICE_BOX_HEIGHT (SERVICE_BOX_BOTTOM - NET_REFERENCE_Y)

#define VANISHING_POINT 502

#define PLAYER_WIDTH 32

/**
 * Structure defining a rectangle defining the boundaries of a part of the court
 */
typedef struct
{
    int16_t left;
    int16_t right;
    int16_t top;
    int16_t bottom;
} Boundary;

typedef enum
{
    COURT_AREA_UPPER,             // Upper part of the court where the top player can move
    COURT_AREA_LOWER,             // Lower part of the court where the bottom player can move
    COURT_AREA_VALID_UPPER,       // Upper part of the court where the ball bounce is valid
    COURT_AREA_VALID_LOWER,       // Lower part of the court where the ball bounce is valid
    COURT_AREA_UPPER_DEUCE_SERVE, // Valid service area (upper side, deuce serve)
    COURT_AREA_LOWER_DEUCE_SERVE,
    COURT_AREA_UPPER_AD_SERVE,
    COURT_AREA_LOWER_AD_SERVE,
    N_COURT_AREAS
} CourtArea;

extern const Boundary court_boundaries[N_COURT_AREAS];

/**
 * Projects a point in the court to the screen
 */
float x_projection(float x, float y);
bool point_in_boundary(const Point *point, const Boundary *boundary);
bool xy_in_boundary(int16_t x, int16_t y, const Boundary *boundary);

#endif
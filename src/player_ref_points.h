#ifndef _PLAYER_REF_POINTS_H_
#define _PLAYER_REF_POINTS_H_

#include <stdint.h>

// --------------------------------------------------------------------------------------
// Reference points (used to calculate the position of the racket relative to the player)
// --------------------------------------------------------------------------------------

typedef enum
{
    REF_POINT_SERVICE,
    REF_POINT_DRIVE,
    REF_POINT_BACKHAND,
    N_REF_POINT_TYPES
} RefPointType;

typedef struct
{
    int8_t x;
    int8_t y;
    int8_t height;
} RefPoint;

/**
 * Pairs of reference points for the players
 * The first one of each pair is for top player and the second for the bottom one
 */
extern RefPoint ref_points[N_REF_POINT_TYPES][2];

#endif
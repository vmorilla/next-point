#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include <stdint.h>
#include <stdbool.h>
#include "court_dimensions.h"

/**
 * Acceleration due to gravity
 */
#define GRAVITY 0.15

// Relation between the velocity after and before bouncing against the ground
#define GROUND_COEF_RESTITUTION 0.75

#define AIR_DRAG 0.001

/**
 * Coefficient to calculate the friction in the air (negative force proportional to the of velocity)
 * A value of 1 means no friction, a value of 0 means the ball stops immediately
 */
#define AIR_DRAG_FACTOR 0.98

void set_ball_speed_at_drive(uint8_t player, uint8_t strength, uint8_t precision);

typedef struct
{
    int n_points;
    Point points[4];
} BallTrajectory;

BallTrajectory *ball_trajectory_points(uint8_t height);

/**
 * Calculates different points of the ball trajectory given a target height at service.
 * It assumes the ball hits a valid area and only points after bounce are considering
 */
BallTrajectory *ball_trajectory_points_at_service(uint8_t height);

#endif
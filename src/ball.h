#ifndef _BALL_H_
#define _BALL_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    float x;
    float y;
    float height;
    float v_h;
    float v_x;
    float v_y;
    bool hidden;
} Ball;

extern Ball ball;

void update_ball(void);

/**
 * Sets the ball speed to reach a maximum height and target bounce point
 */
void set_lob_ball_velocity(int16_t x, int16_t y, uint8_t top_height);

/**
 * Sets the ball speed to reach a certain height at the net and a target bounce point
 */
void set_ball_velocity_net_height(int16_t x, int16_t y, uint8_t net_height);

/**
 * Estimates the position of the ball (x and z) after a certain time has passed
 */
void ball_future_x_and_height(float time, float *future_x, float *future_z);

#endif
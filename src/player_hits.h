/**
 * Functions to establish the speed of the ball after a hit
 */

#ifndef _PLAYER_HITS_H_
#define _PLAYER_HITS_H_

#include "player.h"

/**
 * Sets the speed of the ball at service
 *
 * @param player Player that is serving
 * @param ball_distance Distance to the ball... used as a measure of the precision of the hit
 */
void service_hit(Player *player, uint8_t ball_distance);

/**
 * Sets the speed of the ball at standard hit
 *
 * @param player Player that is hitting the ball
 * @param ball_distance Distance to the ball... used as a measure of the precision of the hit
 */
void standard_hit(Player *player, uint8_t ball_distance);

/**
 * Sets the speed of the ball in a lob hit
 *
 * @param player Player that is hitting the ball
 * @param ball_distance Distance to the ball... used as a measure of the precision of the hit
 */
void lob_hit(Player *player, uint8_t ball_distance);

/**
 * Sets the speed of the ball in a smash hit
 *
 * @param player Player that is hitting the ball
 * @param ball_distance Distance to the ball... used as a measure of the precision of the hit
 */
void smash_hit(Player *player, uint8_t ball_distance);

#endif
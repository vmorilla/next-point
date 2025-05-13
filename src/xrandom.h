#ifndef _X_RANDOM_H_
#define _X_RANDOM_H_

#include <stdint.h>

/**
 * Generates a random integer value centered around a reference point with variable randomness
 * The function warrants the value won't be ouside the rage [mean - max_dev, mean + max_dev]
 */
int16_t limited_normal_dist_random(int mean, int stddev, int max_dev);

/**
 * Generates a random integer value based on a normal distribution
 */
int16_t normal_dist_random(int16_t mean, int16_t stddev);

#endif
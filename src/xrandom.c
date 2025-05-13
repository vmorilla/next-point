#include "xrandom.h"
#include <math.h>
#include <stdlib.h>

#pragma codeseg BANK_8

/**
 * Generates a random integer value centered around a reference point with variable randomness
 * The function warrants the value won't be ouside the rage [mean - max_dev, mean + max_dev]
 */
int16_t limited_normal_dist_random(int mean, int stddev, int max_dev)
{
    int value = normal_dist_random(mean, stddev);
    if (value < mean - max_dev)
        value = mean - max_dev;
    else if (value > mean + max_dev)
        value = mean + max_dev;
    return value;
}

const float std_norm_dist_lookup[11] = {
    -2.33f, // 0.01
    -1.28f, // 0.1
    -0.84f, // 0.2
    -0.52f, // 0.3
    -0.25f, // 0.4
    0.0f,   // 0.5
    0.25f,  // 0.6
    0.52f,  // 0.7
    0.84f,  // 0.8
    1.28f,  // 0.9
    2.33f   // 0.99
};

/**
 * Generates a random integer value based on a normal distribution
 */
int normal_dist_random(int mean, int stddev)
{
    // Generate uniform random value in [0,1]
    float u = (float)(rand() % 256) / 255.0f;

    // Constrain u to avoid extreme values (0.01 to 0.99 range)
    u = 0.01f + u * 0.98f;

    // Linear interpolation between table values
    float index_f = u * 10.0f;
    int index = (int)index_f;
    float fraction = index_f - (float)index;

    // Get interpolated z-score
    float z = std_norm_dist_lookup[index] + fraction * (std_norm_dist_lookup[index + 1] - std_norm_dist_lookup[index]);

    // Scale and shift to desired mean and standard deviation
    return (int)(mean + z * stddev + 0.5f);
}

#include "net.h"
#include "court_dimensions.h"

#pragma codeseg BANK_8

/**
 * Provides the height of the net at a given x coordinate of the court
 */
uint8_t net_height_at(int x)
{
    float px = x_projection(x, NET_POSITION_Y);
    // 12 de ancho
    // Distance from the center of the net
    int x_diff = px - NET_POSITION_X;
    if (x_diff < 0)
        x_diff = -x_diff;

    // Outside the net
    if (x_diff > 16 * 6)
        return 0;

    if (x_diff > 16 * 4 + 3)
        return MAXIMUM_NET_HEIGHT;

    if (x_diff > 16 + 5)
        return (MAXIMUM_NET_HEIGHT + MINIMUM_NET_HEIGHT) / 2;

    return MINIMUM_NET_HEIGHT;
}
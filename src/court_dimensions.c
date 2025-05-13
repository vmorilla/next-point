#include "court_dimensions.h"

#pragma codeseg BANK_8

const Boundary court_boundaries[N_COURT_AREAS] = {
    [COURT_AREA_UPPER] = {
        .left = COURT_LEFT,
        .right = COURT_RIGHT,
        .top = COURT_TOP,
        .bottom = NET_REFERENCE_Y - 1,
    },
    [COURT_AREA_LOWER] = {
        .left = COURT_LEFT,
        .right = COURT_RIGHT,
        .top = NET_REFERENCE_Y + 1,
        .bottom = COURT_BOTTOM_FOR_PLAYER,
    },
    [COURT_AREA_VALID_UPPER] = {.left = LEFT_SIDE_LINE, .right = RIGHT_SIDE_LINE, .top = BASELINE_TOP, .bottom = NET_REFERENCE_Y - 2},
    [COURT_AREA_VALID_LOWER] = {.left = LEFT_SIDE_LINE, .right = RIGHT_SIDE_LINE, .top = NET_REFERENCE_Y - 1, .bottom = BASELINE_BOTTOM},
    [COURT_AREA_UPPER_DEUCE_SERVE] = {.left = LEFT_SIDE_LINE, .right = CENTER_LINE_RIGHT, .top = SERVICE_BOX_TOP, .bottom = NET_REFERENCE_Y - 2},
    [COURT_AREA_LOWER_DEUCE_SERVE] = {.left = CENTER_LINE_LEFT, .right = RIGHT_SIDE_LINE, .top = NET_REFERENCE_Y + 2, .bottom = SERVICE_BOX_BOTTOM},
    [COURT_AREA_UPPER_AD_SERVE] = {.left = CENTER_LINE_LEFT, .right = RIGHT_SIDE_LINE, .top = SERVICE_BOX_TOP, .bottom = NET_REFERENCE_Y - 2},
    [COURT_AREA_LOWER_AD_SERVE] = {.left = LEFT_SIDE_LINE, .right = CENTER_LINE_RIGHT, .top = NET_REFERENCE_Y + 2, .bottom = SERVICE_BOX_BOTTOM},
};

float x_projection(float x, float y)
{
    float dx = x - SCREEN_WIDTH / 2;
    float dy = SCREEN_HEIGHT - y;
    float px = dx * dy / VANISHING_POINT;
    return x - px;
}

bool point_in_boundary(const Point *point, const Boundary *boundary)
{
    return point->x >= boundary->left && point->x <= boundary->right && point->y >= boundary->top && point->y <= boundary->bottom;
}

bool xy_in_boundary(int16_t x, int16_t y, const Boundary *boundary)
{
    return x >= boundary->left && x <= boundary->right && y >= boundary->top && y <= boundary->bottom;
}
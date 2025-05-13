#include "player_ref_points.h"

extern RefPoint ref_points[N_REF_POINT_TYPES][2] = {
    [REF_POINT_SERVICE] = {{3, 0, 55}, {-2, -1, 55}},
    [REF_POINT_DRIVE] = {{-24, 0, 9}, {21, -1, 6}},
    [REF_POINT_BACKHAND] = {{21, 0, 14}, {-15, -1, 11}},
};

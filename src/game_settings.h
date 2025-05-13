#ifndef _GAME_SETTINGS_H_
#define _GAME_SETTINGS_H_

#include <stdint.h>
#include "opponents.h"

typedef enum
{
    GAME_TYPE_SINGLE_PLAYER,
    GAME_TYPE_TWO_PLAYERS,
    GAME_TYPE_DEMO,
} GameType;

// Order for the custom controls in the array of UserDefinedKeys
typedef enum
{
    CUSTOM_CONTROL_HIT,
    CUSTOM_CONTROL_RIGHT,
    CUSTOM_CONTROL_LEFT,
    CUSTOM_CONTROL_DOWN,
    CUSTOM_CONTROL_UP,
    N_CUSTOM_CONTROL_KEYS,
} CustomControlKey;

typedef struct
{
    uint16_t fire;
    uint16_t right;
    uint16_t left;
    uint16_t down;
    uint16_t up;
} UDK;

// udk_t not being accepted by the compiler... seems to be a bug in the headers
typedef union
{
    uint16_t scancodes[N_CUSTOM_CONTROL_KEYS];
    UDK user_defined_keys;
} UserDefinedKeys;
// typedef uint16_t UserDefinedKeys[N_CUSTOM_CONTROL_KEYS];

typedef enum
{
    SURFACE_GRASS,
    SURFACE_CLAY,
    SURFACE_HARD,
    SURFACE_RANDOM,
    SURFACE_TYPE_COUNT,
} SurfaceType;

typedef enum
{
    SPEED_SLOW,
    SPEED_NORMAL,
    SPEED_FAST,
    SPEED_TYPE_COUNT,
} SpeedOption;

typedef enum
{
    USER_CONTROL_KEYBOARD_1,
    USER_CONTROL_JOYSTICK_1,
    USER_CONTROL_KEYBOARD_2,
    USER_CONTROL_JOYSTICK_2,
    N_USER_CONTROL_TYPES,
} UserControlType;

typedef struct
{
    GameType game_type;
    UserControlType user_control_type[2]; // 0: player 1, 1: player 2
    UserDefinedKeys user_defined_keys[2]; // 0: player 1, 1: player 2

    SurfaceType surface_choice; // Can be random
    SurfaceType actual_surface; // In the case of random, this is the actual surface
    SpeedOption speed;
    Opponent opponent_choice; // in the case of single player game

} GameSettings;

extern GameSettings game_settings;

#endif
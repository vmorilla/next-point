/**
 * This file centralizes operations to convert logical coordinates to physical ones and takes care of
 * updating the screen.
 */
#include <intrinsic.h>
#include "physical_coords.h"
#include "sprite_slots.h"
#include "zxn.h"
#include "court_dimensions.h"
#include "ball.h"
#include "player.h"
#include "ula_interrupt.h"
#include "ticks.h"
#include "umpire.h"

#pragma codeseg BANK_8

/**
 * Game sprites physical position data structures
 */

// =================================================================================================
// Auxiliary definitions
// =================================================================================================

typedef enum
{
    SLOT_BALL_SHADOW = 0,
    SLOT_BALL_TOP,
    SLOT_PLAYER_TOP,
    SLOT_BALL_MIDDLE_TOP,
    SLOT_NET,
    SLOT_UMPIRE_CHAIR,
    SLOT_UMPIRE_HEAD,
    SLOT_BALL_MIDDLE_BOTTOM,
    SLOT_PLAYER_BOTTOM,
    SLOT_BALL_BOTTOM,
    SLOT_COUNT
} Slot;

typedef struct
{
    uint16_t x;
    uint16_t y;
} RelativePosition;

// =================================================================================================
// Global variables
// =================================================================================================

Slot formerBallSlot = SLOT_COUNT;
bool formerBallHidden = false;
RelativePosition ballPosition, ballShadowPosition, topPlayerPosition, bottomPlayerPosition, netPosition;

// =================================================================================================
// Helper functions
// =================================================================================================

// =================================================================================================
// Public functions
// =================================================================================================

/**
 * Necessary to avoid certain situations where ball appeared despite being hidden
 */
void hide_ball(void)
{
    ball.hidden = true;
    hide_sprite(&spriteSlots[SLOT_BALL_TOP]);
    hide_sprite(&spriteSlots[SLOT_BALL_MIDDLE_TOP]);
    hide_sprite(&spriteSlots[SLOT_BALL_MIDDLE_BOTTOM]);
    hide_sprite(&spriteSlots[SLOT_BALL_BOTTOM]);
    hide_sprite(&spriteSlots[SLOT_BALL_SHADOW]);
}

// TODO: simplify (camera has been removed)
uint8_t update_screen(void)
{
    Slot ball_slot = ball.y < NET_POSITION_Y ? (ball.y < player_top.y ? SLOT_BALL_TOP : SLOT_BALL_MIDDLE_TOP)
                                             : (ball.y < player_bottom.y ? SLOT_BALL_MIDDLE_BOTTOM : SLOT_BALL_BOTTOM);

    // Ball projected position
    float ball_px = x_projection(ball.x, ball.y);
    float ball_py = ball.y - ball.height;

    // Updates ball and ball shadow position in the sacreen
    ballPosition.x = ball_px;
    ballPosition.y = ball_py;
    ballShadowPosition.x = ball_px;
    ballShadowPosition.y = ball.y;

    // Updates player positions in the screen
    topPlayerPosition.x = x_projection(player_top.x, player_top.y);
    topPlayerPosition.y = player_top.y;

    bottomPlayerPosition.x = x_projection(player_bottom.x, player_bottom.y);
    bottomPlayerPosition.y = player_bottom.y;

    // TODO: simplify
    // Updates net position in the screen
    netPosition.x = NET_POSITION_X;
    netPosition.y = NET_POSITION_Y;

    uint8_t passed_ticks = wait_for_ticks(1);

    if (ball_slot != formerBallSlot)
    {
        if (formerBallSlot != SLOT_COUNT)
            hide_sprite(&spriteSlots[formerBallSlot]);
        formerBallSlot = ball_slot;
        if (!ball.hidden)
            show_sprite(&spriteSlots[ball_slot]);
    }

    if (ball.hidden != formerBallHidden)
    {
        formerBallHidden = ball.hidden;
        if (ball.hidden)
        {
            hide_sprite(&spriteSlots[ball_slot]);
            hide_sprite(&spriteSlots[SLOT_BALL_SHADOW]);
        }
        else
        {
            show_sprite(&spriteSlots[ball_slot]);
            show_sprite(&spriteSlots[SLOT_BALL_SHADOW]);
        }
    }

    move_sprite(&spriteSlots[ball_slot], ballPosition.x, ballPosition.y, 0);
    move_sprite(&spriteSlots[SLOT_BALL_SHADOW], ballShadowPosition.x, ballShadowPosition.y, 0);

    move_sprite(&spriteSlots[SLOT_NET], netPosition.x, netPosition.y, 0);
    move_sprite(&spriteSlots[SLOT_UMPIRE_CHAIR], UMPIRE_CHAIR_X, UMPIRE_CHAIR_Y, 0);
    move_sprite(&spriteSlots[SLOT_UMPIRE_HEAD], UMPIRE_HEAD_X, UMPIRE_HEAD_Y, umpire_frame);

    move_sprite(&spriteSlots[SLOT_PLAYER_TOP], topPlayerPosition.x, topPlayerPosition.y, player_top.frame_index);
    move_sprite(&spriteSlots[SLOT_PLAYER_BOTTOM], bottomPlayerPosition.x, bottomPlayerPosition.y, player_bottom.frame_index);

    return passed_ticks;
}

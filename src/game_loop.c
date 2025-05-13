#include <intrinsic.h>
#include "game_loop.h"
#include "ball.h"
#include "computer_controls.h"
#include "debug.h"
#include "game_state.h"
#include "geometry.h"
#include "gfx.h"
#include "keyboard.h"
#include "messages.h"
#include "player.h"
#include "physical_coords.h"
#include "ticks.h"
#include "umpire_balloon.h"

float next_iteration_delta = 1.0f;
float game_frame_speed;

// Game speed options
static float game_speeds[SPEED_TYPE_COUNT] = {0.6f, 0.8f, 1.0f};

// TODO: remove
extern void _assert(bool condition, const char *message);

static bool confirm_exit(void)
{
    // Wait until the user releases the break key
    in_wait_nokey();

    draw_balloon_with_text("PRESS BREAK AGAIN TO EXIT");

    // Wait until the user presses any key
    in_wait_key();

    hide_balloon();
    reset_ticks();

    return break_key_pressed();
}

void game_loop(void)
{
    // assert(false, "Exit");
    // User defined speed multiplied by the factor of video timing
    game_frame_speed = game_speeds[game_settings.speed] * 50.f / video_timing();
    next_iteration_delta = game_frame_speed;

    // Initializes the game state machine
    start_match();

    reset_ticks();

    while (game_state.point_status != POS_END_MATCH)
    {
        if (break_key_pressed() && confirm_exit())
            break;

        // assert(*(volatile uint8_t *)0xFF4D == 0, "memory corruption");
        update_entities();
        update_messages();
        next_iteration_delta = update_screen() * game_frame_speed;

        // _assert(*(volatile uint8_t *)0xBA99 != 0, "memory corruption");
#ifdef DEBUG
        // debug_points[0]
        //     .x = player_top.x;
        // debug_points[0].y = player_top.y;
        // debug_points[1].x = player_bottom.x;
        // debug_points[1].y = player_bottom.y;
        // debug_points[2].x = ball.x;
        // debug_points[2].y = ball.y;
        // debug_points[3].x = ball.height;
        // debug_points[3].y = ball.v_h * 10;
        // debug_points[4].x = ball.v_x * 10;
        // debug_points[4].y = ball.v_y * 10;
#endif
    }
}

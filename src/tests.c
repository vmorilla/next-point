#ifdef DEBUG

#include <intrinsic.h>
#include <stdlib.h>
#include <input.h>
#include <math.h>

#include "court.h"
#include "court_dimensions.h"
#include "debug.h"
#include "ball.h"
#include "game_state.h"
#include "messages.h"
#include "controls.h"
#include "physical_coords.h"
#include "player.h"
#include "player_ref_points.h"
#include "sound.h"
#include "xrandom.h"
#include "colors.h"
#include "net.h"
#include "sprite_slots.h"

#pragma codeseg BANK_8

// char message[100] = "  ";

// bool ball_in_boundaries(CourtArea area)
// {
//     Boundary *b = &court_boundaries[area];
//     return ball.x >= b->left && ball.x <= b->right && ball.y <= b->bottom && ball.y >= b->top;
// }

// void move_ball(void)
// {
//     ControlInput input = in_stick_cursor();
//     if (input & CONTROL_UP && ball.y > 0)
//         ball.y--;
//     if (input & CONTROL_DOWN && ball.y < COURT_BOTTOM)
//         ball.y++;
//     if (input & CONTROL_LEFT && ball.x > COURT_LEFT)
//         ball.x--;
//     if (input & CONTROL_RIGHT && ball.x < COURT_RIGHT)
//         ball.x++;

//     // ControlInput second_input = read_keyboard_cursor();
//     // if (second_input & CONTROL_UP)
//     //     ball.height++;
//     // if (second_input & CONTROL_DOWN && ball.height > 0)
//     //     ball.height--;

//     ball.hidden = false;
//     intrinsic_halt();
// }

// void message_boundaries(void)
// {
//     debug_point(ball_in_boundaries(COURT_AREA_LOWER_AD_SERVE) ? "In" : "Out", ball.x, ball.y);
// }

// void test_court_boundaries(void)
// {
//     ball.height = 0;
//     ball.hidden = false;

//     while (1)
//     {
//         move_ball();
//         message_boundaries();
//         update_messages();
//         update_screen();
//     }
// }

// void message_reference_points(RefPointType type)
// {
//     Player *player = &player_bottom;
//     RefPoint *ref = &ref_points[type][player->id];
//     char message[100] = "  ";

//     char *ptr = int_to_str(player->x + ref->x - ball.x, message);
//     *(ptr++) = ',';
//     ptr = int_to_str(player->y + ref->y - ball.y, ptr);
//     *(ptr++) = ',';
//     int_to_str(ref->height - ball.height, ptr);

//     send_message(message);
// }

// void test_reference_points(void)
// {
//     ball.height = 0;
//     ball.y = player_bottom.y - 1;

//     // set_player_status(&player_top, PS_DRIVE);
//     player_bottom.frame_index = 17;

//     while (1)
//     {
//         move_ball();
//         message_reference_points(REF_POINT_BACKHAND);
//         update_messages();
//         update_screen(1);
//     }
// }

// int odd;

// void test_net_height(void)
// {
//     ball.height = 0;
//     ball.hidden = false;
//     ball.x = NET_POSITION_X;
//     ball.y = NET_REFERENCE_Y;

//     debug_point("x,y", ball.x, ball.y);

//     while (1)
//     {
//         ControlInput input = read_keyboard_qaop(0);
//         if (input & CONTROL_UP)
//             ball.height++;
//         if ((input & CONTROL_DOWN) && ball.height > 0)
//             ball.height--;
//         if ((input & CONTROL_LEFT) && ball.x > LEFT_SIDE_LINE - 40)
//             ball.x--;
//         if ((input & CONTROL_RIGHT) && ball.x < RIGHT_SIDE_LINE + 40)
//             ball.x++;

//         odd = ball.x;
//         odd = ball.y;

//         debug_point("Ball / Net", ball.height, net_height_at(ball.x));
//         // message_reference_points(REF_POINT_BACKHAND);
//         // update_ball();
//         update_messages();
//         update_screen(1);
//     }
// }

// void test_sounds(void)
// {
//     while (1)
//     {
//         if (!in_inkey() == 0)
//         {
//             play_sound(SOUND_BALL_BOUNCE);
//             if (in_key_pressed(IN_KEY_SCANCODE_f))
//                 play_sound(VOICE_FAULT);
//             if (in_key_pressed(IN_KEY_SCANCODE_o))
//                 play_sound(VOICE_OUT);
//             if (in_key_pressed(IN_KEY_SCANCODE_d))
//                 play_sound(VOICE_DOUBLE_FAULT);

//             in_wait_nokey();
//         }
//         intrinsic_halt();
//     }
// }

// void test_random(void)
// {
//     uint8_t randomness, sample;
//     int16_t value = 0;
//     for (randomness = 0; randomness <= 100; randomness++)
//     {

//         for (sample = 0; sample < 50; sample++)
//         {
//             value = normal_dist_random(160, randomness + 1);
//             debug_draw_point(value, randomness + 50, COLOR_GREEN);
//         }
//     }
//     debug_break();
// }

// void test_math(void)
// {

//     float a = exp(0.5);
//     debug_number("Exp: ", a);

//     float a = cos(0.5);
//     debug_number("Cos: ", a);

//     a = log(0.5);
//     debug_number("Log: ", a);
// }

void _run_tests(void)
{
    // setup_court();
    // prepare_sprite_slots();
    // show_sprite(&spriteSlots[ENTITY_BALL_SHADOW]);

    // test_court_boundaries();

    // test_reference_points();
    // test_sounds();
    // test_random();
    // test_math();
    // test_net_height();
}

#endif
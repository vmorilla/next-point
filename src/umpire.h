#ifndef _UMPIRE_H_
#define _UMPIRE_H_

#include <stdint.h>

typedef enum
{
    UMPIRE_FRAME_LOOK_UP = 0,
    UMPIRE_FRAME_LOOK_CENTER,
    UMPIRE_FRAME_LOOK_DOWN,
    UMPIRE_FRAME_SPEAK,
} UmpireFrame;

/**
 * Initializes the umpire
 */
void init_umpire(void);

/**
 * Updates the umpire provided the position of the ball and whether there is an announcement
 */
void update_umpire(void);

/**
 * Selected frame of the sprite of the umpire
 */
extern UmpireFrame umpire_frame;

// --------------------------------------------
// Umpire specific calls
// --------------------------------------------

void umpire_call_ace(void);

void umpire_call_double_fault(void);

void umpire_call_fault(void);

void umpire_call_first_service(void);

void umpire_call_let(void);

void umpire_call_out(void);

void umpire_call_second_service(void);

void umpire_call_service(void);

void umpire_call_score(void);

void umpire_call_game_for_player(uint8_t player);

void umpire_call_set_for_player(uint8_t player);

void umpire_call_match_for_player(uint8_t player);

void umpire_call_game_point(void);

void umpire_call_set_point(void);

void umpire_call_match_point(void);

void umpire_call_touch(void);

// /**
//  * Draws the dialogue balloon with the provided text length
//  * Returns the column for the text to be diplayed
//  */
// uint8_t draw_balloon(uint8_t text_length);

// /**
//  * Draws text in the existing balloon at the specified column
//  */
// void draw_text_in_balloon(uint8_t column, const char *text);

// /**
//  * Draws the dialogue balloon with the provided text
//  * The text is centered in the balloon
//  */
// void draw_balloon_with_text(const char *text);

// /**
//  * Clears the dialogue balloon from the screen
//  */
// void hide_balloon(void);

#endif
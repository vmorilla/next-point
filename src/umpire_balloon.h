#ifndef _UMPIRE_BALLOON_H_
#define _UMPIRE_BALLOON_H_

#define BALLOON_TEXT_MAX_LENGTH 40

/**
 * Draws the dialogue balloon with the provided text
 * The text is centered in the balloon
 */
void draw_balloon_with_text(const char *text);

/**
 * Clears the dialogue balloon from the screen
 */
void hide_balloon(void);

#endif
#ifndef _PHYSICAL_COORDS_H_
#define _PHYSICAL_COORDS_H_

#include <stdint.h>

/**
 * Updates the screen by applying the position of the camera and the position of the sprites of the game
 * Before doing the actual update of sprites, it waits for the next frame to be ready.
 *
 * @param ticks The number of ticks to wait for. => param removed and forced to
 * @return The number of ticks elapsed since the last call.
 */
uint8_t update_screen(void);

/**
 * Necessary to avoid certain situations where ball appeared despite being hidden
 */
void hide_ball(void);

#endif
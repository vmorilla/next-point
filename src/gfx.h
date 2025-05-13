#ifndef _GFX_H_
#define _GFX_H_

#include <stdint.h>
#include <stdbool.h>

#define LAYER2_RAM_BANK 9
#define LAYER2_SHADOW_RAM_BANK 14

/**
 * Initializes the hardware of the Next: Turbo mode and disables RAM memory contention
 */
void init_hardware(void);

/**
 * Setups the layer2 graphics in 320x256 mode, with clipping area corresponding to the regular ZX Specutrum screen
 */
void init_layer2(void);

/**
 * Returns the video timing in Hertz: 50 or 60 Hz
 */
uint8_t video_timing(void);

/**
 * Enables or disables the sprites on the screen
 */
void set_sprites_visibility(bool enable);

#endif
#include "gfx.h"
#include "zxn.h"

#include <string.h>

#include "tilemap.h"

/* ==================================================================== */
/* Specific functions                                                  */
/* ==================================================================== */

/**
 * Initializes the hardware of the Next: Turbo mode and disables RAM memory contention
 */
void init_hardware(void)
{
    // Put Z80 in 28 MHz turbo mode.
    ZXN_NEXTREG(REG_TURBO_MODE, RTM_28MHZ);

    // Sets joystick mode to CURSOR for joystick 1 and Kempston for joystick 2
    // but keeping the 50/60 Hz value and scan doubler settings
    const uint8_t reg_periph_1 = ZXN_READ_REG(REG_PERIPHERAL_1) & 0b00000101;
    ZXN_NEXTREGA(REG_PERIPHERAL_1, reg_periph_1 | RP1_JOY1_CURSOR | RP1_JOY2_KEMPSTON_1);

    // Disables turbo and speed hotkeys. Enable AY mode
    // PRP2_PS2_PSG_MODE_AY seems to be wrongly defined to 0x11
    ZXN_NEXTREGA(REG_PERIPHERAL_2, RP2_PS2_KEYBOARD | 0x1);

    // Disable RAM memory contention.
    ZXN_NEXTREGA(REG_PERIPHERAL_3, ZXN_READ_REG(REG_PERIPHERAL_3) | RP3_DISABLE_CONTENTION);
}

/**
 * Setups the layer2 graphics in 320x256 mode, with clipping area corresponding to the regular ZX Specutrum screen
 */
void init_layer2(void)
{
    // Enables layer 2 display
    ZXN_NEXTREG(REG_DISPLAY_CONTROL, RDP_ENABLE_LAYER_2);
    // 320x256 8bpp mode
    ZXN_NEXTREG(REG_LAYER_2_CONTROL, RL2C_RESOLUTION_320x256);

    // Setups layer2 and layer2 shadow banks
    ZXN_NEXTREGA(REG_LAYER_2_RAM_BANK, LAYER2_RAM_BANK);
    // ZXN_NEXTREGA(REG_LAYER_2_SHADOW_RAM_BANK, LAYER2_SHADOW_RAM_BANK);

    clip_layer2_window(0, SCREEN_EXT_WIDTH / 2 - 1, 0, SCREEN_EXT_HEIGHT - 1);
    clip_sprites_window(0, SCREEN_EXT_WIDTH / 2 - 1, 0, SCREEN_EXT_HEIGHT - 1);
}

/**
 * Returns the video timing in Hertz: 50 or 60 Hz
 */
uint8_t video_timing(void)
{
    return (ZXN_READ_REG(REG_PERIPHERAL_1) & RP1_RATE_60) ? 60 : 50;
}

void set_sprites_visibility(bool enable)
{
    ZXN_NEXTREGA(REG_SPRITE_LAYER_SYSTEM, (enable ? RSLS_SPRITES_VISIBLE : 0) | RSLS_LAYER_PRIORITY_USL | RSLS_SPRITES_OVER_BORDER | RSLS_ENABLE_SPRITES_CLIPING_OVER_BORDER);
}
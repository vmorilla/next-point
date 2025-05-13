#include "zxn.h"

/* ====================================================================== */
/* Layer 2                                                                */
/* ====================================================================== */

/**
 * Sets the clip window for layer2 graphics to the specified coordinates.asm
 * Mind that the coordinates are inclusive and the x coordinates is multiplied by 2
 * in 256 and 320 modes, and by 4 in 640 mode.
 */
void clip_layer2_window(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2)
{
    ZXN_NEXTREGA(REG_CLIP_WINDOW_CONTROL, RCWC_RESET_LAYER_2_CLIP_INDEX); // | RCWC_RESET_SPRITE_CLIP_INDEX);
    ZXN_NEXTREGA(REG_CLIP_WINDOW_LAYER_2, x1);
    ZXN_NEXTREGA(REG_CLIP_WINDOW_LAYER_2, x2);
    ZXN_NEXTREGA(REG_CLIP_WINDOW_LAYER_2, y1);
    ZXN_NEXTREGA(REG_CLIP_WINDOW_LAYER_2, y2);
}

/**
 * Sets the offset for layer2 graphics
 */
void set_layer2_offset(uint16_t x, uint8_t y)
{
    ZXN_NEXTREGA(REG_LAYER_2_OFFSET_X_MSB, x >> 8);
    ZXN_NEXTREGA(REG_LAYER_2_OFFSET_X, x & 0xff);
    ZXN_NEXTREGA(REG_LAYER_2_OFFSET_Y, y);
}

/* ====================================================================== */
/* Tilemap                                                                */
/* ====================================================================== */

/**
 * Sets the clip window for tilemap graphics to the specified coordinates.asm
 * Mind that the coordinates are inclusive and the x coordinates is multiplied by 2
 * in 256 and 320 modes, and by 4 in 640 mode.
 */
void clip_tilemap_window(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2)
{
    ZXN_NEXTREG(REG_CLIP_WINDOW_CONTROL, RCWC_RESET_TILEMAP_CLIP_INDEX);
    ZXN_NEXTREGA(REG_CLIP_WINDOW_TILEMAP, x1);
    ZXN_NEXTREGA(REG_CLIP_WINDOW_TILEMAP, x2);
    ZXN_NEXTREGA(REG_CLIP_WINDOW_TILEMAP, y1);
    ZXN_NEXTREGA(REG_CLIP_WINDOW_TILEMAP, y2);
}

/**
 * Sets the offset for the tilemap layer
 */
void set_tilemap_offset(uint16_t x, uint8_t y)
{
    ZXN_NEXTREGA(REG_TILEMAP_OFFSET_X_MSB, x >> 8);
    ZXN_NEXTREGA(REG_TILEMAP_OFFSET_X, x & 0xff);
    ZXN_NEXTREGA(REG_TILEMAP_OFFSET_Y, y);
}

/* ====================================================================== */
/* Sprites                                                                */
/* ====================================================================== */

/**
 * Sets the index for sprite operations following this call
 **/
void set_sprite_index(uint8_t index)
{
    ZXN_NEXTREGA(REG_SPRITE_INDEX, index);
}

/**
 * Sets the attributes for the anchor character of a unified sprite
 */
void setup_unified_sprite(uint8_t pattern, uint16_t x, uint16_t y)
{
    ZXN_NEXTREGA(REG_SPRITE_X, x & 0xff);
    ZXN_NEXTREGA(REG_SPRITE_Y, y & 0xff);
    ZXN_NEXTREGA(REG_SPRITE_ATTR2, (x & 0x100) >> 8);
    ZXN_NEXTREGA(REG_SPRITE_ATTR3, RSA3_VISIBLE | RSA3_USE_BYTE_4 | pattern);
    ZXN_NEXTREGA(REG_SPRITE_ATTR4_INCREMENT, RSA4_ANCHOR_UNIFIED | (y & 0x100) >> 8); // Anchor with unified relatives, no scaling
}

/**
 * Sets the attributes for a relative sprite
 */
void setup_relative_sprite(uint8_t pattern, uint8_t x_offset, uint8_t y_offset)
{
    ZXN_NEXTREGA(REG_SPRITE_X, x_offset);
    ZXN_NEXTREGA(REG_SPRITE_Y, y_offset);
    ZXN_NEXTREGA(REG_SPRITE_ATTR2, RSA2_RELATIVE_ANCHOR_PALETTE);
    ZXN_NEXTREGA(REG_SPRITE_ATTR3, RSA3_VISIBLE | RSA3_USE_BYTE_4 | pattern);
    ZXN_NEXTREGA(REG_SPRITE_ATTR4_INCREMENT, RSA4_RELATIVE_SPRITE | RSA4_RELATIVE_ANCHOR_PATTERN); // Relative sprite, no scaling
}

void clip_sprites_window(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2)
{
    ZXN_NEXTREGA(REG_CLIP_WINDOW_CONTROL, RCWC_RESET_SPRITE_CLIP_INDEX);
    ZXN_NEXTREGA(REG_CLIP_WINDOW_SPRITES, x1);
    ZXN_NEXTREGA(REG_CLIP_WINDOW_SPRITES, x2);
    ZXN_NEXTREGA(REG_CLIP_WINDOW_SPRITES, y1);
    ZXN_NEXTREGA(REG_CLIP_WINDOW_SPRITES, y2);
}
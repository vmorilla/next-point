#ifndef _ZXN_H_
#define _ZXN_H_

#include <arch/zxn.h>
#include <stdint.h>

/* ====================================================================== */
/* Next registers                                                         */
/* ====================================================================== */

#define SCREEN_EXT_WIDTH 320
#define SCREEN_EXT_HEIGHT 256
#define SCREEN_DOUBLE_WIDTH 640
#define SCREEN_MARGIN 32

#define REG_TILEMAP_OFFSET_X_MSB 0x2f
#define REG_TILEMAP_OFFSET_X 0x30
#define REG_TILEMAP_OFFSET_Y 0x31
#define REG_TILEMAP_TRANSPARENCY_INDEX 0x4C

// REG_CLIP_WINDOW_CONTROL 0x1C
#define RCWC_RESET_TILEMAP_CLIP_INDEX 0x08

#define REG_CLIP_WINDOW_TILEMAP 0x1B

// REG_PALETTE_CONTROL
#define RPC_SELECT_TILEMAP_PALETTE_0 0x30
#define RPC_SELECT_TILEMAP_PALETTE_1 0x70

// SPRITE REGISTERS
#define REG_SPRITE_INDEX 0x34

#define REG_SPRITE_X 0x35
#define REG_SPRITE_X_INCREMENT 0x75

#define REG_SPRITE_Y 0x36
#define REG_SPRITE_Y_INCREMENT 0x76

#define REG_SPRITE_ATTR2 0x37
#define REG_SPRITE_ATTR2_INCREMENT 0x77
#define RSA2_ENABLE_X_MIRRORING 0x08
#define RSA2_ENABLE_Y_MIRRORING 0x04
#define RSA2_ROTATE_CLOCKWISE 0x02
#define RSA2_RELATIVE_ANCHOR_PALETTE 0x01

#define REG_SPRITE_ATTR3 0x38
#define REG_SPRITE_ATTR3_INCREMENT 0x78
#define RSA3_VISIBLE 0x80
#define RSA3_USE_BYTE_4 0x40

#define REG_SPRITE_ATTR4 0x39
#define REG_SPRITE_ATTR4_INCREMENT 0x79
#define RSA4_ANCHOR_SPRITE_8BIT 0b00000000
#define RSA4_RELATIVE_SPRITE 0b01000000
#define RSA4_ANCHOR_SPRITE_4BIT_0_127_PATTERN 0b10000000
#define RSA4_ANCHOR_SPRITE_4BIT_128_255_PATTERN 0b11000000
#define RSA4_ANCHOR_UNIFIED 0b00100000
#define RSA4_ANCHOR_COMPOSITE 0b00000000
#define RSA4_RELATIVE_ANCHOR_PATTERN 0b00000001
#define RSA4_ANCHOR_X_SCALE_2X 0b00001000
#define RSA4_ANCHOR_X_SCALE_4X 0b00010000
#define RSA4_ANCHOR_X_SCALE_8X 0b00011000
#define RSA4_ANCHOR_Y_SCALE_2X 0b00000100
#define RSA4_ANCHOR_Y_SCALE_4X 0b00000010
#define RSA4_ANCHOR_Y_SCALE_8X 0b00000110

#define RSLS_ENABLE_SPRITES_CLIPING_OVER_BORDER 0x20

// ULA REGISTERS
#define REG_ULA_CONTROL 0x68
#define RUC_DISABLE_ULA_OUTPUT 0x80
#define RUC_CANCEL_8x5_FOR_EXTENDED_KEYS 0x10
#define RUC_ULA_AS_BLEND_COLOR 0x00
#define RUC_NO_BLENDING 0b00100000
#define RUC_ULA_TILEMAP_AS_BLEND_COLOR 0b01000000
#define RUC_TILEMAP_AS_BLEND_COLOR 0b01100000

#define REG_DISPLAY_CONTROL 0x69
#define RDP_ENABLE_LAYER_2 0x80
#define RDP_ENABLE_ULA_SHADOW_DISPLAY 0x40

#define REG_TILEMAP_CONTROL 0x6b
#define RTC_ENABLE_TILEMAP 0x80
#define RTC_80x32_MODE 0x40
#define RTC_ELIMINATE_ATTR_BYTE 0x20
#define RTC_SECOND_TILEMAP 0x10
#define RTC_ACTIVATE_TEXT_MODE 0x08
#define RTC_ACTIVATE_512_MODE 0x02
#define RTC_FORCE_TILEMAP_OVER_ULA 0x01

#define REG_DEFAULT_TILEMAP_ATTRIBUTE 0x6c
#define RDTA_TILEINDEX_BIT_8 0x01

#define REG_TILEMAP_BASE_ADDRESS 0x6e
#define REG_TILE_DEFINITIONS_BASE_ADDRESS 0x6f

#define REG_LAYER_2_CONTROL 0x70
#define RL2C_RESOLUTION_256x192 0x00
#define RL2C_RESOLUTION_320x256 0x10
#define RL2C_RESOLUTION_640x256 0x20

#define REG_LAYER_2_OFFSET_X_MSB 0x71

// REG_TURBO_MODE missing flags
#define RTM_28MHZ 0x03

/* ====================================================================== */
/* Interrupts                                                             */
/* ====================================================================== */

#define REG_INTERRUPT_CONTROL 0xC0

#define REG_INTERRUP_ENABLE_0 0xC4
#define REG_INTERRUP_ENABLE_1 0xC5
#define REG_INTERRUP_ENABLE_2 0xC6

#define REG_INTERRUP_STATUS_0 0xC8
#define REG_INTERRUP_STATUS_1 0xC9
#define REG_INTERRUP_STATUS_2 0xCA

#define REG_DMA_INTERRUPT_ENABLE_0 0xCC
#define REG_DMA_INTERRUPT_ENABLE_1 0xCD
#define REG_DMA_INTERRUPT_ENABLE_2 0xCE

/* ====================================================================== */
/* KEYBOARD                                                               */
/* ====================================================================== */

#define REG_EXTENDED_KEYS_0 0xB0
#define REG_EXTENDED_KEYS_1 0xB1
#define REK1_BREAK 0x20
/* ====================================================================== */
/* CTC                                                                    */
/* ====================================================================== */

#define CTC0 0x183B // CTC channel 0 port
#define CTC1 0x193B // CTC channel 1 port
#define CTC2 0x1A3B // CTC channel 2 port
#define CTC3 0x1B3B // CTC channel 3 port

/* ====================================================================== */
/* Generic operations                                                     */
/* ====================================================================== */

/**
 * Updates a next register with the specified value applying a mask to the existing value
 * Mask it is uint16_t because of the problem with calls with odd number of octects
 */
void ZXN_NEXTREG_UPDATE(uint8_t reg, uint8_t val, uint8_t mask);

/* ====================================================================== */
/* Layer 2                                                                */
/* ====================================================================== */

/**
 * Sets the clip window for layer2 graphics to the specified coordinates.asm
 * Mind that the coordinates are inclusive and the x coordinates is multiplied by 2
 * in 256 and 320 modes, and by 4 in 640 mode.
 */
void clip_layer2_window(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2);

/**
 * Sets the offset for layer2 graphics
 */
void set_layer2_offset(uint16_t x, uint8_t y);

/* ====================================================================== */
/* Tilemap                                                                */
/* ====================================================================== */

/**
 * Sets the clip window for tilemap graphics to the specified coordinates.asm
 * Mind that the coordinates are inclusive and the x coordinates is multiplied by 2
 * in 256 and 320 modes, and by 4 in 640 mode.
 */
void clip_tilemap_window(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2);

/**
 * Sets the offset for the tilemap layer
 */
void set_tilemap_offset(uint16_t x, uint8_t y);

/* ====================================================================== */
/* Sprites                                                                */
/* ====================================================================== */

struct SpriteRecord
{
    uint8_t attr0;
    uint8_t attr1;
    uint8_t attr2;
    uint8_t attr3;
    uint8_t attr4;
};

/**
 * Sets the index for sprite operations following this call
 **/
void set_sprite_index(uint8_t index);

/**
 * Sets the attributes for the anchor character of a unified sprite
 */
void setup_unified_sprite(uint8_t pattern, uint16_t x, uint16_t y);

/**
 * Sets the attributes for a relative sprite
 */
void setup_relative_sprite(uint8_t pattern, uint8_t x_offset, uint8_t y_offset);

/**
 * Sets the clip window for sprites to the specified coordinates.asm
 * Mind that the coordinates are inclusive and the x coordinates is multiplied by 2
 * in 256 and 320 modes, and by 4 in 640 mode.
 */
void clip_sprites_window(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2);

#endif
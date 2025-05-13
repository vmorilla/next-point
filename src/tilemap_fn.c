#include <stdint.h>
#include <string.h>
#include <intrinsic.h>
#include <compress/zx7.h>
#include "tilemap.h"
#include <intrinsic.h>
#include "zxn.h"

// Note: ULA Shadow does not seem to work for tilemaps
#define FONTS_PAGE 99

#define TILEMAP_BASE_ADDRESS 0x4000
#define TILEMAP_SIZE (80 * 32 * 2)
#define TILE_DEFINITIONS_BASE_ADDRESS (TILEMAP_BASE_ADDRESS + TILEMAP_SIZE)

void load_palette(uint8_t *palette, uint8_t palette_length)
{
    // Loads palettes
    ZXN_NEXTREG(REG_PALETTE_CONTROL, RPC_SELECT_TILEMAP_PALETTE_0);
    ZXN_NEXTREG(REG_PALETTE_INDEX, 0);
    for (uint8_t i = 0; i < palette_length; i++)
    {
        ZXN_NEXTREGA(REG_PALETTE_VALUE_8, palette[i]);
    }
}

void select_tilemap(TileMapSelection tilemap_selection)
{
    const uint8_t current_page = ZXN_READ_MMU6();
    ZXN_WRITE_MMU6(FONTS_PAGE);

    // Disables ULA and enables Tilemap of double resolution.
    ZXN_NEXTREGA(REG_ULA_CONTROL, RUC_DISABLE_ULA_OUTPUT); // To disable extended keys mapping to 8x5 matrix: RUC_CANCEL_8x5_FOR_EXTENDED_KEYS);
    ZXN_NEXTREGA(REG_TILEMAP_CONTROL, RTC_ENABLE_TILEMAP | RTC_80x32_MODE);

    // No need to set the default tilemap attribute
    // ZXN_NEXTREG(REG_DEFAULT_TILEMAP_ATTRIBUTE, 0x00);

    // Sets the base address (relative to 0x4000) of the tilemap
    ZXN_NEXTREGA(REG_TILEMAP_BASE_ADDRESS, (TILEMAP_BASE_ADDRESS - 0x4000) >> 8);

    // Sets the base address (relative to 0x4000) of the tile definitions
    ZXN_NEXTREGA(REG_TILE_DEFINITIONS_BASE_ADDRESS, TILEMAP_SIZE >> 8);

    // Sets the transparency index
    ZXN_NEXTREG(REG_TILEMAP_TRANSPARENCY_INDEX, 0x00);

    // Cleans the tilemap
    memset((void *)TILEMAP_BASE_ADDRESS, 0x00, TILEMAP_SIZE);

    if (tilemap_selection == TILEMAP_GAME)
    {
        dzx7_standard(game_patterns, (void *)TILE_DEFINITIONS_BASE_ADDRESS);
        load_palette(game_palette, game_palette_length);
    }
    else
    {
        dzx7_standard(menu_patterns, (void *)TILE_DEFINITIONS_BASE_ADDRESS);
        load_palette(menu_palette, menu_palette_length); // menu_palette_length);
    }

    ZXN_WRITE_MMU6(current_page);
}

void clear_tilemap(void)
{
    // TODO: this does not work any more with CTC interruptions...
    // Reduce flickering
    intrinsic_halt();
    memset((void *)tilemap, 0, TILEMAP_HEIGHT * TILEMAP_WIDTH * sizeof(TileMapEntry));
}
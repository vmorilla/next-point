#ifndef _TILEMAP_H_
#define _TILEMAP_H_

#include <stdint.h>
#include <stdbool.h>

// -----------------------------------------------------------
// Tilemap constants
// -----------------------------------------------------------

#define TILEMAP_WIDTH 80
#define TILEMAP_HEIGHT 32

// -----------------------------------------------------------
// Tilemap special characters
// -----------------------------------------------------------
#define CHAR_SERVICE_BALL 0x05

// --------------------------------------------------------------
// Tilemap palettes
// --------------------------------------------------------------

#define TILEMAP_DEFAULT_PALETTE 0x00
#define TILEMAP_GAMES_PALETTE 0x01

// -----------------------------------------------------------
// Balloon tilemap dimensions
// -----------------------------------------------------------

#define BALLOON_WIDTH 13
#define BALLOON_HEIGHT 4

// -----------------------------------------------------------
// Tilemap data structures
// -----------------------------------------------------------

typedef struct
{
    char character;
    union
    {
        uint8_t attr;
        struct
        {
            bool tile_on_top : 1;
            bool rotate : 1;
            bool y_mirror : 1;
            bool x_mirror : 1;
            uint8_t palette : 4;
        };
    };

} TileMapEntry;

// ------------------------------------------------------------
// Tilemap selection
// ------------------------------------------------------------

typedef enum
{
    TILEMAP_GAME,
    TILEMAP_MENU,
} TileMapSelection;

typedef enum
{
    TILEMAP_PALETTE_SCOREBOARD,
    TILEMAP_PALETTE_GAMES,
    TILEMAP_PALETTE_BALLOON,
    TILEMAP_PALETTE_PLAYER_CHOICE,
} TileMapPalette;

/**
 * Initialized the tilemap in 80x32 mode with attributes and selects a font
 */
void select_tilemap(TileMapSelection tilemap_selection);

/**
 * Clears the tilemap
 */
void clear_tilemap(void);

// -----------------------------------------------------------
// Tilemap data
// -----------------------------------------------------------

extern uint8_t tiles[];
extern uint8_t game_patterns[];
extern uint8_t game_palette[];
extern uint8_t game_palette_end[];
#define game_palette_length ((uint8_t)(game_palette_end - game_palette))

extern uint8_t menu_patterns[];
extern uint8_t menu_palette[];
extern uint8_t menu_palette_end[];
#define menu_palette_length ((uint8_t)(menu_palette_end - menu_palette))

// The balloon array is arranged by rows, to facilitate extending the balloon in the x axis
extern TileMapEntry balloon_tilemap[BALLOON_WIDTH][BALLOON_HEIGHT];

extern TileMapEntry tilemap[TILEMAP_HEIGHT][TILEMAP_WIDTH];

#endif

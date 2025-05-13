#ifndef _UNIFIED_SPRITES_H_
#define _UNIFIED_SPRITES_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * A frame defintion is a structure that includes all the information of a sprite frame
 * (a Cel in aseprite terms). It includes a number of tiles (16x16 sprites) that compose
 * the frame, the number of patterns needed to draw the frame, the offset of the frame with
 * respect to the reference point, and the raw content (attributes followed by patterns).
 *
 * Frame definitions are generated from aseprite files and stored in memory pages.
 * Different frame definitions might require being stored in different memory pages.
 */

typedef struct
{
    // Number of patterns needed to draw this frame ( <= nTiles and <= max numer of patterns of the skin))
    const uint8_t nPatterns;

    // The coordinate system draws sprites so that the bottom middle point is at the reference point
    // The offset is used to adjust the position of the sprite to the coordinate system
    const int8_t offsetX;
    const int8_t offsetY;

    // Byte array with the sprite attributes (5 x nTiles) followed by the sprite patterns ( 16 x 16 x nPatterns)
    // The byte 3 of the content is modified to point to the pattern index in the pattern memory once the sprites
    // have been allocated
    // The number of tiles is equal to the maximum number of sprites that can be used in the skin. So, if the frame
    // has fewer tiles, the rest of the attributes are filled with empty ones (attribute 3 = 0x80)
    uint8_t content[];

} FrameDefinition;

/**
 * A frame address is a structure that includes points to a frame definition, stored in a certain
 * memory page. Frame addresses are generated from aseprite files
 */
typedef struct
{
    const uint8_t memoryPage;
    const FrameDefinition *frameDefinition;
} FrameAddress;

/**
 * A skin definition is a structure that points to the set of frame definitions that compose the
 * skin of a sprite (the different positions of the same character). For instance:
 * - The ball is a skin with a single frame of a single 16x16 sprite
 * - The ball shadow is another skin with a single frame (also 16x16 sprite)
 * - The net is a skin with a single frame but composed by multiple 16x16 sprites
 * - A character of the top player is a skin with multiple frames (different positions). Sarah Pova
 *   is a different skin than Roger Federer
 *
 * Skin definitions are generated from aseprite and include also, precalculated the maximum
 * number of attributes and patterns that the skin might use (the maximum of each frame).
 *
 * All skin definitions will be generated from aseprite files in a single file and memory page
 * The will be stored in variables generated from aseprite labels
 */
typedef struct
{
    // Points to the pattern index in the sprite pattern memory that will be used for this skin
    uint8_t currentPatternIndex;
    // Maximum number of individual sprites used by the unified sprite of this skin
    const uint8_t nSprites;
    // Maximum number of patterns used by any of the frames in this skin
    const uint8_t maxPatterns;
    // Number of frames in this skin
    const uint8_t nFrames;
    // Addresses of the definition of each frame
    const FrameAddress frameAddresses[];
} SkinDefinition;

/**
 * The unified sprite slot includes information about each sprite slot used in a momemnt of the game.alignas
 * Once the set of skins is known, the set of sprite slots is initialized, included the offset of attributes
 * and the offset of patterns in memory.alignas
 *
 * The slots can be initialized provided the list of skins to be assigned to each slot.
 */
typedef struct
{
    uint8_t currentFrame;

    // Copy of the x and y offset of the current frame to avoid page switching if not needed
    int8_t offsetX;
    int8_t offsetY;
    // TODO: rename to index
    uint8_t attributesIndex;
    // Pattern index specific to the skin (in advance of the scenario of using two frames of the same skin in different slots)
    // uint8_t patternIndex;
    SkinDefinition *skinDefinition;
} UnifiedSpriteSlot;

/**
 * Initialize a number of sprite slots with a list of skin definitions
 */
void init_sprite_slots(UnifiedSpriteSlot (*spriteSlots)[], SkinDefinition *(*skinDefinitions)[], uint8_t nSlots);

/**
 * Loads a sprite frame:
 *   - Loads attributes in the corresponding slot
 *   - Updates the x and y offset in the slot
 *   - Loads the sprite patterns, considering the patternOffset
 */
void loadSpriteFrame(UnifiedSpriteSlot *slot, FrameDefinition *frame, uint8_t memoryPage, uint16_t x, uint16_t y, bool visible);

/**
 * Loads the attributes and patterns of a frame in a slot
 *
 * @content: Pointer to the content of the frame, which contains the attributes (covering the max number of sprites of the skin),
 *         followed by the patterns (16x16) of the frame (which might be less than the max number of patterns of the skin)
 * @attrOffset: index of the attributes in the sprite memory used to paint the sprite
 * @nSprites: number of sprites used in the skin (the number of attributes)
 * @patternIndex: index of the first pattern in the sprite memory reserved for this skin.
 *                Bit 7 is the visibility bit: (1 = visible | 0 = hidden )
 * @nPatterns: number of patterns used in the frame
 */
void loadAttrsAndPatterns(uint8_t *content, uint8_t attrOffset, uint8_t nSprites, uint8_t patternIndex_visibility, uint8_t nPatterns);

/**
 * Moves a sprite to a new position. If the frame is different from the current frame,
 * the frame information is loaded
 */
void move_sprite(UnifiedSpriteSlot *slot, uint16_t x, uint16_t y, uint8_t frame);

/**
 * Hides a sprite by setting the attribute 3
 */
void hide_sprite(UnifiedSpriteSlot *slot);

/**
 * Shows a sprite by setting the attribute 3
 */
void show_sprite(UnifiedSpriteSlot *slot);

/**
 * Changes the visibility of a sprite (convenience function unifyng hide and show)
 */
void change_sprite_visibility(UnifiedSpriteSlot *slot, bool visible);

#endif
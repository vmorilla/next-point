#include "unified_sprites.h"
#include "zxn.h"
#include "debug.h"

#define UNDEFINED_PATTERN_INDEX 0xff
#define UNDEFINED_FRAME_INDEX 0xff

/**
 * Initialize a number of sprite slots with a list of skin definitions
 */
void init_sprite_slots(UnifiedSpriteSlot (*spriteSlots)[], SkinDefinition *(*skinDefinitions)[], uint8_t nSlots)
{
    // Resets the current pattern index for all skins, it will be assigned in the
    // next loop with the first slot that uses it
    for (char i = 0; i < nSlots; i++)
        (*skinDefinitions)[i]->currentPatternIndex = UNDEFINED_PATTERN_INDEX;

    uint8_t attributeOffset = 0;
    uint8_t patternIndex = 0;

    for (char i = 0; i < nSlots; i++)
    {
        UnifiedSpriteSlot *slot = &(*spriteSlots)[i];
        SkinDefinition *skin = (*skinDefinitions)[i];
        slot->skinDefinition = skin;
        slot->attributesIndex = attributeOffset;
        // This will force the pattern loading when setting the correct frame later
        slot->currentFrame = UNDEFINED_FRAME_INDEX;
        attributeOffset += skin->nSprites;
        // The pattern index is specfic to the skin
        if (skin->currentPatternIndex == UNDEFINED_PATTERN_INDEX)
        {
            skin->currentPatternIndex = patternIndex;
            patternIndex += skin->maxPatterns;
        }
    }
}

void move_sprite(UnifiedSpriteSlot *slot, uint16_t x, uint16_t y, uint8_t frame)
{
    if (slot->currentFrame != frame)
    {
        slot->currentFrame = frame;
        const FrameAddress *frameAddress = &(slot->skinDefinition->frameAddresses)[frame];
        loadSpriteFrame(slot, frameAddress->frameDefinition, frameAddress->memoryPage, x, y, true);
    }
    else
    {
        x += slot->offsetX;
        y += slot->offsetY;
        ZXN_NEXTREGA(REG_SPRITE_INDEX, slot->attributesIndex);
        ZXN_NEXTREGA(REG_SPRITE_X, x & 0xff);
        ZXN_NEXTREGA(REG_SPRITE_Y, y & 0xff);
        ZXN_NEXTREGA(REG_SPRITE_ATTR2, (x & 0x100) >> 8); // Should be fine unless there is a rotation or mirroring
        // Attribute 3 does not need to be modified
        ZXN_NEXTREGA(REG_SPRITE_ATTR4, (y & 0x100) >> 8); // Should be fine unless there scaling
    }
}

/**
 * Loads a sprite frame:
 *   - Loads attributes in the corresponding slot
 *   - Updates the x and y offset in the slot
 *   - Loads the sprite patterns, considering the patternOffset
 */
void loadSpriteFrame(UnifiedSpriteSlot *slot, FrameDefinition *frame, uint8_t memoryPage, uint16_t x, uint16_t y, bool visible)
{
    assert(memoryPage >= 38 && memoryPage <= 47, "Invalid memory page");
    assert(frame >= (void *)0xC000 && frame < (void *)0xE000, "Invalid frame address");
    // Page loaded in memory 8k page at 0xC000
    uint8_t currentMemoryPage = ZXN_READ_MMU6();
    ZXN_WRITE_MMU6(memoryPage);

    slot->offsetX = frame->offsetX;
    slot->offsetY = frame->offsetY;
    // Modifies attribute 3 to point to the pattern index saved in the skin definition
    frame->content[0] = (x + frame->offsetX) & 0xff;
    frame->content[1] = (y + frame->offsetY) & 0xff;
    frame->content[2] = (x & 0x100) >> 8;                                                  // Should be fine unless there is a rotation or mirroring
    frame->content[3] = slot->skinDefinition->currentPatternIndex | 0x40 | (visible << 7); // 0x40 = use byte 4, 0x80 = visible
    // Attribute 3 does not need to be modified
    frame->content[4] = (y & 0x100) >> 8; // Should be fine unless there scaling
    loadAttrsAndPatterns(frame->content, slot->attributesIndex, slot->skinDefinition->nSprites, slot->skinDefinition->currentPatternIndex, frame->nPatterns);

    ZXN_WRITE_MMU6(currentMemoryPage);
}

void hide_sprite(UnifiedSpriteSlot *slot)
{
    ZXN_NEXTREGA(REG_SPRITE_INDEX, slot->attributesIndex);
    ZXN_NEXTREGA(REG_SPRITE_ATTR3, 0b01000000);
}

void show_sprite(UnifiedSpriteSlot *slot)
{
    ZXN_NEXTREGA(REG_SPRITE_INDEX, slot->attributesIndex);
    ZXN_NEXTREGA(REG_SPRITE_ATTR3, slot->skinDefinition->currentPatternIndex | 0xc0);
}

void change_sprite_visibility(UnifiedSpriteSlot *slot, bool visible)
{
    if (visible)
        show_sprite(slot);
    else
        hide_sprite(slot);
}

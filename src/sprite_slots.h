#ifndef _SPRITE_SLOTS_
#define _SPRITE_SLOTS_

#include <stdint.h>
#include "unified_sprites.h"

// Redundante con physical_coords.c Slot ??
typedef enum
{
    ENTITY_BALL_SHADOW = 0,
    ENTITY_BALL_TOP,
    ENTITY_PLAYER_TOP,
    ENTITY_BALL_MIDDLE_TOP,
    ENTITY_NET,
    ENTITY_UMPIRE_CHAIR,
    ENTITY_UMPIRE_HEAD,
    ENTITY_BALL_MIDDLE_BOTTOM,
    ENTITY_PLAYER_BOTTOM,
    ENTITY_BALL_BOTTOM,
    ENTITY_COUNT
} Entity;

extern UnifiedSpriteSlot spriteSlots[ENTITY_COUNT];

void prepare_sprite_slots(void);

#endif
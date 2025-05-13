#include "sprite_slots.h"
#include "skins.h"
#include "zxn.h"

UnifiedSpriteSlot spriteSlots[ENTITY_COUNT];
SkinDefinition *skins[ENTITY_COUNT] = {
    [ENTITY_BALL_SHADOW] = &skin_ball_shadow,
    [ENTITY_BALL_TOP] = &skin_ball,
    [ENTITY_PLAYER_TOP] = &skin_player_front_a,
    [ENTITY_BALL_MIDDLE_TOP] = &skin_ball,
    [ENTITY_NET] = &skin_net,
    [ENTITY_UMPIRE_CHAIR] = &skin_umpire_chair,
    [ENTITY_UMPIRE_HEAD] = &skin_umpire_head,
    [ENTITY_BALL_MIDDLE_BOTTOM] = &skin_ball,
    [ENTITY_PLAYER_BOTTOM] = &skin_player_back_a,
    [ENTITY_BALL_BOTTOM] = &skin_ball,
};

static void preload_sprite(Entity entity)
{
    UnifiedSpriteSlot *slot = &spriteSlots[entity];
    slot->currentFrame = 0;
    FrameAddress *frameAddress = &(slot->skinDefinition->frameAddresses)[slot->currentFrame];
    loadSpriteFrame(slot, frameAddress->frameDefinition, frameAddress->memoryPage, 0, 0, false);
}

void prepare_sprite_slots(void)
{
    init_sprite_slots(&spriteSlots, &skins, ENTITY_COUNT);

    // Preloads and hides the ball for a start
    preload_sprite(ENTITY_BALL_SHADOW);
    preload_sprite(ENTITY_BALL_TOP);
    preload_sprite(ENTITY_BALL_MIDDLE_TOP);
    preload_sprite(ENTITY_BALL_MIDDLE_BOTTOM);
    preload_sprite(ENTITY_BALL_BOTTOM);
}


#include "fadeout_effect.h"
#include "tilemap.h"

#include <intrinsic.h>

extern volatile uint16_t remaining_effect_length;

void start_effect(uint16_t char_attr) __z88dk_fastcall;

void fadeout_effect(void)
{
    select_tilemap(TILEMAP_GAME);
#ifndef DISABLE_EFFECTS
    start_effect(0x3020);
#endif
}

void fadein_effect(void)
{
#ifndef DISABLE_EFFECTS
    start_effect(0x0);
#endif
}

void wait_for_effect(void)
{
#ifndef DISABLE_EFFECTS
    while (remaining_effect_length != 0)
    {
        intrinsic_halt();
    }
#endif
}

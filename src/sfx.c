#include "sfx.h"
#include "zxn.h"

#ifdef __INTELLISENSE__

#define IO_AY_REG (void)0;
#define IO_AY_DAT (void)0;

#endif

void sfx_play_sound(void *effect_address) __z88dk_fastcall;
extern void *sfx_sounds_table[];

void sfx_play_sync(SoundEffect effect)
{
    sfx_play_sound(sfx_sounds_table[effect]);
}

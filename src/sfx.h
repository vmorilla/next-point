#ifndef _SFX_H_
#define _SFX_H_

typedef enum
{
    SFX_CURSOR_MOVE,
    SFX_CURSOR_SELECT,
} SoundEffect;

void sfx_init(void);
void sfx_play_sync(SoundEffect effect);

#endif
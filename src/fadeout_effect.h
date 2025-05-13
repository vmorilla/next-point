#ifndef _FADEOUT_EFFECT_H_
#define _FADEOUT_EFFECT_H_

#include <stdint.h>

// The effects can be removed by using the flag DISABLE_EFFECTS

void init_effects(void);

void fadeout_effect(void);

void fadein_effect(void);

void wait_for_effect(void);

#endif
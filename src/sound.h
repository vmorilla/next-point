#ifndef _SOUND_H_
#define _SOUND_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * Initializes the sound system
 */
void init_sound(void);

/**
 * Sound interrupt handler
 */
void sound_interrupt_handler(void);

typedef enum
{
    VOICE_OUT,
    VOICE_FAULT,
    VOICE_DOUBLE_FAULT,
    VOICE_FIRST_SERVICE,
    VOICE_SECOND_SERVICE,
    VOICE_LET,
    VOICE_TOUCH,
    GAME_POINT,
    SET_POINT,
    MATCH_POINT,
    GAME_RECEIVER,
    GAME_SERVER,
    SET_RECEIVER,
    SET_SERVER,
    MATCH_RECEIVER,
    MATCH_SERVER,
    SOUND_BALL_BOUNCE,
    SOUND_BALL_HIT,
    SOUND_BALL_HIT_2,
    SOUND_BALL_HITS_NET,
    SCORE_0_15,
    SCORE_0_30,
    SCORE_0_40,
    SCORE_15_0,
    SCORE_15_15,
    SCORE_15_30,
    SCORE_15_40,
    SCORE_30_0,
    SCORE_30_15,
    SCORE_30_30,
    SCORE_30_40,
    SCORE_40_0,
    SCORE_40_15,
    SCORE_40_30,
    SCORE_40_40,
    SCORE_AD_RECEIVER,
    SCORE_AD_SERVER,
    SOUND_UMPF_DIEGO_1,
    SOUND_UMPF_DIEGO_2,
    SOUND_UMPF_VICTOR_1,
    SOUND_UMPF_VICTOR_2,
    SOUND_CROWD_CLAPPING,
    SOUND_TOUCH_HIT_MALE,

} SoundSampleEnum;

void play_sound(SoundSampleEnum sound) __z88dk_fastcall;
void play_sound_channel_b(SoundSampleEnum sound) __z88dk_fastcall;

bool is_sound_playing(void);
bool is_sound_playing_channel_b(void);

/**
 * Remaining length of the sound sample in channel B
 * Used to move the umpire mouth
 */
uint16_t channel_b_remaining_length(void);

#endif
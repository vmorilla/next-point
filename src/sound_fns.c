#include "sound.h"

extern volatile uint16_t current_sample_length;
extern volatile uint16_t current_sample_length_b;

bool is_sound_playing(void)
{
    return current_sample_length > 0;
}

bool is_sound_playing_channel_b(void)
{
    return current_sample_length_b > 0;
}

uint16_t channel_b_remaining_length(void)
{
    return current_sample_length_b;
}
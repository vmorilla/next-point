#ifndef _UMPIRE_QUEUE_H_
#define _UMPIRE_QUEUE_H_

#include <stdbool.h>
#include "sound.h"
#include "umpire_balloon.h"

typedef struct
{
    SoundSampleEnum sound;
    char text[BALLOON_TEXT_MAX_LENGTH];
} UmpireMessage;

bool add_message_to_queue(SoundSampleEnum sound, const char *text);

UmpireMessage *get_message_from_queue(void);

void clear_queue(void);

#endif

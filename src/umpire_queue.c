#include <stddef.h>
#include <string.h>
#include "umpire_queue.h"

#define UMPIRE_MESSAGE_QUEUE_SIZE 3

UmpireMessage umpire_message_queue[UMPIRE_MESSAGE_QUEUE_SIZE];
uint8_t umpire_message_queue_head = 0;
uint8_t umpire_message_queue_tail = 0;

bool add_message_to_queue(SoundSampleEnum sound, const char *text)
{
    if (umpire_message_queue_head == (umpire_message_queue_tail + 1) % UMPIRE_MESSAGE_QUEUE_SIZE)
    {
        // Queue is full
        return false;
    }
    else
    {
        UmpireMessage *message = &umpire_message_queue[umpire_message_queue_tail];
        message->sound = sound;
        strcpy(message->text, text);
        umpire_message_queue_tail = (umpire_message_queue_tail + 1) % UMPIRE_MESSAGE_QUEUE_SIZE;
        return true;
    }
}

UmpireMessage *get_message_from_queue(void)
{
    if (umpire_message_queue_head == umpire_message_queue_tail)
    {
        // Queue is empty
        return NULL;
    }
    else
    {
        UmpireMessage *message = &umpire_message_queue[umpire_message_queue_head];
        umpire_message_queue_head = (umpire_message_queue_head + 1) % UMPIRE_MESSAGE_QUEUE_SIZE;
        return message;
    }
}

// bool is_message_queue_empty(void)
// {
//     return umpire_message_queue_head == umpire_message_queue_tail;
// }

// bool is_message_queue_full(void)
// {
//     return umpire_message_queue_head == (umpire_message_queue_tail + 1) % UMPIRE_MESSAGE_QUEUE_SIZE;
// }

void clear_queue(void)
{
    umpire_message_queue_head = 0;
    umpire_message_queue_tail = 0;
}

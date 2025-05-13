#include <stdint.h>
#include <string.h>
#include "messages.h"
#include "tilemap.h"
#include "debug.h"

#pragma codeseg BANK_8

#define MESSAGE_LINE 7
#define MESSAGE_COL 2
#define MESSAGE_LENGTH 60
#define DEFAULT_COUNT_DOWN 250

uint16_t count_down = 0;

void clean_message_area(void)
{
    for (uint8_t i = 0; i < MESSAGE_LENGTH; i++)
    {
        tilemap[MESSAGE_LINE][MESSAGE_COL + i].character = 0x00;
    }
}

void send_message(const char *msg)
{
    clean_message_area();
    for (uint8_t i = 0; i < MESSAGE_LENGTH; i++)
    {
        if (msg[i] == '\0')
            break;

        tilemap[MESSAGE_LINE][MESSAGE_COL + i].character = msg[i];
        // debug_number("MessageAddress", (uint16_t)&tile_map[MESSAGE_LINE][MESSAGE_COL + i]);
    }
    count_down = DEFAULT_COUNT_DOWN;
}

void update_messages(void)
{
    if (count_down > 0)
    {
        count_down--;
        if (count_down == 0)
        {
            clean_message_area();
        }
    }
}

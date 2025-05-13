#include <string.h>
#include <stdbool.h>
#include <intrinsic.h>
#include "debug.h"
#include "tilemap.h"
#include "zxn.h"

#pragma codeseg BANK_8

#define DEBUG_CONSOLE_LINES 10
#define DEBUG_CONSOLE_LINE_LENGTH 32
#define DEBUG_CONSOLE_X_POSITION 80 - 1 - DEBUG_CONSOLE_LINE_LENGTH
#define DEBUG_CONSOLE_Y_POSITION 1
#define ASSERTION_MESSAGE_LENGTH 32

static char assertion_message[ASSERTION_MESSAGE_LENGTH];
uint8_t debug_console_n_lines = 0;
bool halt_next_frame = false;

void scroll_debug_console(void)
{
    // Scroll the console up
    for (uint8_t i = 0; i < DEBUG_CONSOLE_LINES - 1; i++)
    {
        memcpy(&tilemap[DEBUG_CONSOLE_Y_POSITION + i][DEBUG_CONSOLE_X_POSITION],
               &tilemap[DEBUG_CONSOLE_Y_POSITION + i + 1][DEBUG_CONSOLE_X_POSITION],
               DEBUG_CONSOLE_LINE_LENGTH * 2);
    }
}

char *new_console_line(void)
{
    if (debug_console_n_lines >= DEBUG_CONSOLE_LINES)
    {
        scroll_debug_console();
    }
    else
    {
        debug_console_n_lines++;
    }
    return (char *)&tilemap[DEBUG_CONSOLE_Y_POSITION + debug_console_n_lines - 1][DEBUG_CONSOLE_X_POSITION];
}

void end_console_line(char *address)
{
    char *end = (char *)&tilemap[DEBUG_CONSOLE_Y_POSITION + debug_console_n_lines][DEBUG_CONSOLE_X_POSITION + DEBUG_CONSOLE_LINE_LENGTH];
    // Fill the rest of the line with spaces
    while (address < end)
    {
        *address++ = 0;
    }
}

char *float_to_string(float value, char *buffer)
{
    uint8_t decimals = 2; // Number of decimal places to display
    // Handle negative numbers
    if (value < 0)
    {
        *buffer++ = '-';
        buffer++;
        value = -value;
    }

    // Get the integer part
    int integer_part = (int)value;
    float decimal_part = value - integer_part;

    // Convert integer part
    int i = 0;
    char int_buffer[16]; // Temporary buffer for integer part

    if (integer_part == 0)
    {
        int_buffer[i++] = '0';
    }
    else
    {
        while (integer_part > 0)
        {
            int_buffer[i++] = '0' + integer_part % 10;
            integer_part /= 10;
        }
    }

    // Reverse the integer part
    while (i > 0)
    {
        *buffer++ = int_buffer[--i];
        buffer++;
    }

    // Add decimal part if needed
    if (decimals > 0)
    {
        *buffer++ = '.';
        buffer++;

        // Convert decimal part
        while (decimals-- > 0)
        {
            decimal_part *= 10;
            int digit = (int)decimal_part;
            *buffer++ = '0' + digit;
            buffer++;
            decimal_part -= digit;
        }
    }

    return buffer;
}

void _debug_message(const char *message)
{
    char *address = new_console_line();
    while (*message != 0)
    {
        *address++ = *message++;
        address++;
    }
    end_console_line(address);
}

void _debug_number(const char *label, float value)
{
    char *address = new_console_line();
    while (*label != 0)
    {

        *address++ = *label++;
        address++;
    }
    *address++ = ':';
    address++;
    *address++ = ' ';
    address++;

    // Converts the float to a string
    address = float_to_string(value, address);

    // Sends the message
    end_console_line(address);
}

void _debug_point(const char *label, float x, float y)
{
    char *address = new_console_line();
    while (*label != 0)
    {
        *address++ = *label++;
        address++;
    }
    *address++ = ':';
    address++;
    *address++ = ' ';
    address++;

    // Converts the float to a string
    address = float_to_string(x, address);
    *address++ = ',';
    address++;
    address = float_to_string(y, address);

    // Sends the message
    end_console_line(address);
}

void _debug_draw_point(uint16_t x, uint8_t y, uint8_t color)
{
    if (x >= SCREEN_WIDTH)
        return;
    uint16_t pixel = x * 256 + y;
    uint8_t page = pixel >> 13;
    char *address = (char *)((pixel & 0x1FFF) | 0xC000);

    uint8_t saved_page = ZXN_READ_MMU6();
    ZXN_WRITE_MMU6(18 + page);

    *address = (char *)color;

    ZXN_WRITE_MMU6(saved_page);
}

void _debug_halt_next_frame(void)
{
    halt_next_frame = true;
}

/**
 * In the break of an assertion, the value of ix+5 points to the address with the message
 */
void _assert(bool condition, const char *message)
{
    if (!condition)
    {
        strcpy(assertion_message, message);
        debug_break();
    }
}
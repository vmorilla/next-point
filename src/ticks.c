#include "ticks.h"
#include "ula_interrupt.h"
#include "intrinsic.h"

#ifdef DEBUG
// Used to check if there are many game iterations running for longer than 1 frame
uint16_t runoff_ticks = 0;
#endif

uint8_t wait_for_ticks(uint8_t ticks)
{
    // Waits for a minimum number of ticks to pass since last call to this function.
    // This function is used to create a delay in the program execution.
    // The function returns the actual number of ticks elapsed since last wait_for_ticks call.

#ifdef DEBUG
    if (frame_counter > 0)
        runoff_ticks += frame_counter;
#endif
    while (frame_counter < ticks)
    {
        intrinsic_halt();
    }

    const uint8_t current_frame_counter = frame_counter;
    frame_counter = 0;
    return current_frame_counter;
}

void reset_ticks(void)
{
    // Resets the ticks counter to 0.
    frame_counter = 0;
}
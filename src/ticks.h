#ifndef _TICKS_H_
#define _TICKS_H_

#include <stdint.h>

/**
 * Waits for a minimum number of ticks to pass since last call to this function.
 * This function is used to create a delay in the program execution.
 *
 * @param ticks The number of ticks to wait for.
 * @return Actual number of ticks elapsed since last wait_for_ticks call.
 */
uint8_t wait_for_ticks(uint8_t ticks);

void reset_ticks(void);

#endif
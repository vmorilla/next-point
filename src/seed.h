#ifndef _SEED_H_
#define _SEED_H_

#include <stdint.h>

/**
 * Generates a random seed based on the value of the R register
 */
uint16_t random_seed(void) __z88dk_fastcall;

#endif
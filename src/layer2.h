#ifndef _COMPRESSED_COURTS_H_
#define _COMPRESSED_COURTS_H_

#include <stdint.h>
#include <stdbool.h>
/**
 * Loads a layer2 screen from compressed data in a page of memory
 * The page must start with 5 blocks of a data structure for each of the 5 banks of the layer2
 * Each block contains a page number and a ponter to the compressed data of each bank
 * Data must be compressed with the zx7 algorithm and it is assumed that each bank is compressed
 * within a single page (several banks can be compressed in the same page)
 *
 * @page: The page number to load the layer2 screen from
 */
void load_layer2_screen(uint8_t page, const void *address);

void load_layer2_shadow_screen(uint8_t page, const void *address);

void swap_layer2_screen(void);

#endif
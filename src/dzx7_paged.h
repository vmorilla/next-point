#ifndef _DZX7_PAGED_H_
#define _DZX7_PAGED_H_

/**
 * Decompresses the compressed block at address src to address dst.
 * Address src is assumed to be in the slot 6 page. If the pointer
 * exceeds the page, it will switch to the next bank using the
 * paging calls.
 *
 * The function does not restore the slot 6 page and returns the
 * next source address. This way, it can be used to chain multiple
 * calls to decompress blocks, for instance, to fill in the layer2
 */
const void *dzx7_paged(const void *src, void *dst);

#endif
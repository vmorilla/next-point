#include "keyboard.h"
#include <input.h>
#include "zxn.h"

bool break_key_pressed(void)
{
// Check if the break key is pressed b1
#ifdef DEBUG
    // Cannot find a way to isse a break key in CSpect
    return (ZXN_READ_REG(REG_EXTENDED_KEYS_1) & REK1_BREAK || in_key_pressed(IN_KEY_SCANCODE_ENTER)) != 0;
#else
    // This seems redundant, but it looks like the compiler is assuming booleans to be 1 or
    return ZXN_READ_REG(REG_EXTENDED_KEYS_1) & REK1_BREAK ? true : false;
    // return ZXN_READ_REG(REG_EXTENDED_KEYS_1) & REK1_BREAK;
#endif
}
#include "opponents.h"
#include <stdint.h>
#include "zxn.h"

#define DISPLAY_PROFILE_PAGE 97
extern void _diplay_profile(Opponent opponent);

void diplay_profile(Opponent opponent)
{
    uint8_t saved_page = ZXN_READ_MMU6();
    ZXN_WRITE_MMU6(DISPLAY_PROFILE_PAGE);

    _diplay_profile(opponent);

    ZXN_WRITE_MMU6(saved_page);
}

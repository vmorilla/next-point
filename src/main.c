
#include <input.h>
#include <stdlib.h>

#include "game_state.h"
#include "zxn.h"
#include "physical_coords.h"
#include "sprite_slots.h"
#include "gfx.h"
#include "menu_main.h"
#include "messages.h"
#include "court.h"
#include "controls.h"
#include "ball.h"
#include "sound.h"
#include "tests.h"
#include "interrupts.h"
#include "ticks.h"
#include "layer2.h"
#include "layer2_screens.h"
#include "tilemap.h"
#include "seed.h"
#include "umpire.h"
#include "large_font.h"
#include "computer_controls.h"
#include "game_loop.h"
#include "debug.h"
#include "fadeout_effect.h"
#include "sfx.h"

// #pragma output CRT_ORG_CODE = 0x8184
// #pragma output REGISTER_SP = 0xFF58
// #pragma output CLIB_MALLOC_HEAP_SIZE = 0
// #pragma output CLIB_STDIO_HEAP_SIZE = 0
// #pragma output CLIB_FOPEN_MAX = -1

void init(void)
{

    init_hardware();
    hardware_interrupt_mode();
    // slots 0 and 1 assigned to additional RAM
    ZXN_WRITE_MMU0(16);
    ZXN_WRITE_MMU1(17);

    set_sprites_visibility(false);

    init_effects();
    init_sound();
    sfx_init();

    init_layer2();

    prepare_sprite_slots();

    // Initializes the random number generator
    srand(random_seed());
}

int main(void)
{
    init();
    console_message("Hope it works");

    while (true)
    {
        set_sprites_visibility(false);
        select_tilemap(TILEMAP_GAME);

        start_main_menu();

        setup_court();

        game_loop();
    }
}

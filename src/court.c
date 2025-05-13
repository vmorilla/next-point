#include "court.h"

#include <stdbool.h>
#include <stdlib.h>

#include "layer2.h"
#include "layer2_screens.h"
#include "game_settings.h"
#include "gfx.h"
#include "tilemap.h"
#include "fadeout_effect.h"
#include "sound.h"

typedef struct
{
    uint8_t page;
    const void *address;
} Layer2Screen;

static const Layer2Screen surface_l2screens[] = {
    {LY2_COURT_LONDON_PAGE, LY2_COURT_LONDON_ADDRESS},
    {LY2_COURT_MADRID_PAGE, LY2_COURT_MADRID_ADDRESS},
    {LY2_COURT_SIDNEY_PAGE, LY2_COURT_SIDNEY_ADDRESS}};

void setup_court(void)
{
    fadeout_effect();
    uint8_t random_choice = rand();
    game_settings.actual_surface = game_settings.surface_choice == SURFACE_RANDOM
                                       ? random_choice % 3
                                       : game_settings.surface_choice;
    const Layer2Screen *court = &surface_l2screens[game_settings.actual_surface];
    load_layer2_shadow_screen(court->page, court->address);
    wait_for_effect();
    play_sound_channel_b(SOUND_CROWD_CLAPPING);
    swap_layer2_screen();
    fadein_effect();
    wait_for_effect();
    set_sprites_visibility(true);
}
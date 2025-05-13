#include <stdint.h>
#include "dzx7_paged.h"
#include "zxn.h"
#include "layer2.h"
#include "gfx.h"

#define LAYER2_PAGE (LAYER2_RAM_BANK * 2)
#define LAYER2_SHADOW_PAGE (LAYER2_SHADOW_RAM_BANK * 2)

static bool shadow_layer2_is_active = false;

typedef struct
{
    uint8_t page;
    void *address;
} Layer2Block;

typedef Layer2Block Layer2Screen[5];

extern const Layer2Screen layer2_screen;

void load_layer2_screen(uint8_t page, const void *address)
{
    load_layer2_shadow_screen(page, address);
    swap_layer2_screen();
}

void load_layer2_shadow_screen(uint8_t page, const void *address)
{
    uint8_t target_page = shadow_layer2_is_active ? LAYER2_PAGE : LAYER2_SHADOW_PAGE;
    // Saves MMU slots
    uint8_t slot0 = ZXN_READ_MMU0();
    uint8_t slot1 = ZXN_READ_MMU1();
    uint8_t slot6 = ZXN_READ_MMU6();

    ZXN_WRITE_MMU6(page);

    for (uint8_t i = 0; i < 5; i++)
    {
        ZXN_WRITE_MMU0(target_page + i * 2);
        ZXN_WRITE_MMU1(target_page + i * 2 + 1);
        // Load the court data into the memory
        address = dzx7_paged(address, (void *)0x0000);
    }

    // Restore MMU slots
    ZXN_WRITE_MMU0(slot0);
    ZXN_WRITE_MMU1(slot1);
    ZXN_WRITE_MMU6(slot6);
}

void swap_layer2_screen(void)
{
    // Swap active page
    shadow_layer2_is_active = shadow_layer2_is_active ^ 1;
    ZXN_NEXTREGA(REG_LAYER_2_RAM_BANK, shadow_layer2_is_active ? LAYER2_SHADOW_RAM_BANK : LAYER2_RAM_BANK);
}
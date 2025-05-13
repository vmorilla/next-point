#ifndef _MENU_H_
#define _MENU_H_

#include <stdbool.h>
#include <stdint.h>

#define MENU_FIRST_ROW 12
#define MENU_ROW_SEPARATION 3
#define MENU_NOTES_ROWS 29

typedef int8_t (*MenuAction)(int8_t);
typedef void (*MenuSuffixFn)(int8_t);

typedef struct
{
    const char *label;
    MenuAction action;
    MenuSuffixFn suffix_fn;
} MenuEntry;

typedef enum
{
    MENU_KEY_NONE = 0,
    MENU_KEY_UP,
    MENU_KEY_DOWN,
    MENU_KEY_LEFT,
    MENU_KEY_RIGHT,
    MENU_KEY_BACK,
    MENU_KEY_ENTER,
} MenuSelectionKey;

MenuSelectionKey read_menu_input(void);

int8_t handle_std_menu(MenuEntry (*menu_entries)[], uint8_t n_entries, uint8_t left_column, bool top_menu);

int8_t menu_go_back(int8_t entry);

#endif
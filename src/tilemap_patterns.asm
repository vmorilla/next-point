PUBLIC _game_patterns, _game_palette, _game_palette_end
PUBLIC _menu_patterns, _menu_palette, _menu_palette_end

; TODO: find a better solution more tidy than this and find a an alternative location for the balloon tilemap

SECTION PAGE_99

; -------------------------------------------------------------
; Game fonts
; -------------------------------------------------------------

_game_patterns:
    binary "../build/fonts/tiledefs.bin.zx7"
_game_palette:
    binary "../build/fonts/palettes.bin"
_game_palette_end:

; -------------------------------------------------------------
; Menu fonts
; -------------------------------------------------------------

_menu_patterns:
    binary "../build/fonts/menu-tiledefs.bin.zx7"
_menu_palette:
    binary "../build/fonts/menu-palettes.bin"
_menu_palette_end:




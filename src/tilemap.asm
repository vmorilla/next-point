PUBLIC _tilemap, _balloon_tilemap, _balloon_tilemap_end

SECTION BANK_5

    ORG $4000
_tilemap:
    ; Maybe better initialize in black before ULA to avoid screen noise
    defs 80*32*2, 0   ; Tilemap initialized with zeros 

    defs 8192 + 48, 0 ; A bit contrived... we find the end of the game tilemap data to put the ballon_tilemap


_balloon_tilemap:
    binary "../build/fonts/balloon.map"
_balloon_tilemap_end:


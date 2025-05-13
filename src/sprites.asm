SECTION code_user

PUBLIC _upload_sprite_attrs, _hide_sprites

    ; void upload_sprite_attrs(uint8_t index, uint8_t length, SpritePattern pattern[]);
_upload_sprite_attrs:
    pop hl
    pop de ; e = index, d = length
    ex (sp), hl ; hl = pattern address

    ; Selects the index of the sprite to upload in the port
    ld bc, 0x303B
    di
    out (c), e

    ; Upload port
    ld c, 0x57
    ; B set to 5 (size of sprite attrs), C to the sprite attribute upload port
next_sprite:
    ld b, 5
    otir
    dec d
    jr nz, next_sprite
    ei
    ret

_hide_sprites:
    pop hl
    ex (sp), hl
    ; h = size, l = attr index
    ld b, h
    ld a, l
    NEXTREG 0x34, a
    ld a, 0b01000000
next:
    NEXTREG 0x78, a
    djnz next
    ret

SECTION code_user

PUBLIC _loadAttrsAndPatterns


    
    defc SPRITE_SLOT_SELECT_PORT = 0x303B
    defc SPRITE_ATTRIBUTE_UPLOAD_PORT = 0x57

    ;void loadAttrsAndPatterns(uint8_t *content, uint8_t attrOffset, uint8_t nSprites, uint8_t patternIndex, uint8_t nPatterns);
    ; ix + 0: content address
    ; ix + 2: attrOffset 
    ; ix + 3: nSprites of the skin
    ; ix + 4: patternIndex
    ; ix + 5: nPatterns of the frame

_loadAttrsAndPatterns:
    push ix
    ld ix, 4
    add ix, sp

    ; Upload sprite attributes
    ld l, (ix + 0)
    ld h, (ix + 1)  ; content address
    ld bc, SPRITE_SLOT_SELECT_PORT ; prepare port for sprite slot index
    ld e, (ix + 2) ; attr index
    di
    out (c), e     ; sets the sprite attribute index
    ; Upload port
    ld c, SPRITE_ATTRIBUTE_UPLOAD_PORT
    ld e, (ix + 3) ; nSprites  

    ld d, 5
    mul d, e ; nSprites * 5
    ld b, e
    otir

    ; TODO: hide sprites not used (nSprites to nMaxSprites)

    ; Upload sprite patterns
    ld (dmaSource), hl ; Copy sprite sheet address from HL
    ; The pattern index 
    ld h, (ix + 5) ; nPatterns
    ld l, 0
    ld (dmaLength), hl ; Copy length in bytes from BC

    ld bc, $303b ; prepare port for sprite index
    ld a, (ix + 4) ; pattern index
    out (c), a 
    ld hl, dmaProgram ; setup source for otir
    ld b, dmaProgramLength ; setup length for otir
    ld c, $6b ; setup dma port
    otir ; invoke dma code
    ei
    pop ix
    ret
dmaProgram:
    db %10000011 ; R6 - Disable DMA
    db %01111101 ; R0 - append length + port A address, A->B
dmaSource:
    dw 0         ; R0 par 1&2 - port A start address
dmaLength:
    dw 0 ; R0 par 3&4 - transfer length
    db %01010100 ; R1 - A incr., A=memory
    db %00000010 ; R1-Cycle length port A
    db %01101000 ; R2 - B fixed, B=I/O
    db %00000010 ; R2-Cycle length port B
    db %10101101 ; R4 - continuous, append port B address
    dw $005B     ; R4 par 1&2 - port B address
    db %10000010 ; R5 - stop on end of block, CE only
    db %11001111 ; R6 - load addresses into DMA counters
    db %10000111 ; R6 - enable DMA
dmaProgramLength = $ - dmaProgram
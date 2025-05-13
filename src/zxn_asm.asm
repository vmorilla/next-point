    SECTION code_user

    PUBLIC _ZXN_NEXTREG_UPDATE

    ; void ZXN_NEXTREG_UPDATE(uint8_t reg, uint8_t val, uint8_t mask);
_ZXN_NEXTREG_UPDATE:
    push ix
    ld ix, 2
    add ix, sp
    ld bc, $243b ; Port for next registers
    ld a, (ix + 0) ; register
    out (c), a
    inc b
    in a, (c)    ; Value of register in A
    and a, (ix + 2) ; Mask
    or a, (ix + 1) ; register
    out (c), a
    pop ix
    ret

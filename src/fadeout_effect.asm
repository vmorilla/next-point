
SECTION code_user

include "macros.inc"

PUBLIC _effects_handler, _remaining_effect_length, _start_effect, _init_effects


defc FADEOUT_EFFECT_DATA_PAGE = 98

_effects_handler:
    nextreg INTERRUPT_STATUS_CTC, 2
    push af
    push hl

    ld hl, (_remaining_effect_length)
    ld a, h
    or l
    call nz, _effects_step
    pop hl
    pop af
    ei
    reti

_effects_step:

    ; HL receives the remaining length, assumed to be different than 0
    ; Any register, other than HL and A should be saved
    ; Saves the current page in MMU 6
    push de
    dec hl
    dec hl
    ld (_remaining_effect_length), hl

    ; Bank switch
    READ_NEXTREG(REG_MMU6)
    push af
    nextreg REG_MMU6, FADEOUT_EFFECT_DATA_PAGE

    ld de, effects_address
    add hl, de
    ld e, (hl)
    inc hl
    ld d, (hl)

    ld a, (_effect_char_attr)
    ld (de), a
    inc de
    ld a, (_effect_char_attr + 1)
    ld (de), a

    ; Restores bank
    pop af
    nextreg REG_MMU6, a

    pop de
    ret

    ; HL received the character (L) and attribute (H) to be used in the effect
_start_effect:
    ld (_effect_char_attr), hl
    ld hl, effects_length
    ld (_remaining_effect_length), hl
    ret

_remaining_effect_length:
    defw 0
_effect_char_attr:
    defw 0


_init_effects:
; ifndef does not seem to work... DISABLE_EFFECTS does not seem to be defined in the assembler
#ifndef DISABLE_EFFECTS

    ; Clock frequency = CPU frequency / ( 16 * ( 1 + time constant ) )
    ld h, 10100101b  ; No interrupt follows vector
                    ; 7: Enable (1) / disable (0) interrupt
                    ; 6: Counter mode (1) / timer mode (0)
                    ; 5: Prescaler 256 (1) / 16 (0)
                    ; 4: Edge selection: 0 = falling, 1 = rising
                    ; 3: Timer trigger: 0 = automatic trigger when time constant is loaded, 1 = clk starts timer
                    ; 2: Time constant: 0 = no time constant follows, 1 = time constant follows
                    ; 1: Reset: 0 = continue operation / 1 = software reset 
                    ; 0: Vector (0) / control word (1)
    ;ld l, 218      ; 217 : 8kHz = 28MHz / (16 * (1 + 218))
    
    ld l, 50     ; 16kHz = 28MHz / (16 * (1 + 109))
    ld	bc,CTC1		; Channel 0 port
	out	(c),h		; Control word
	out	(c),l	
#endif
    ret




SECTION PAGE_98

    ORG $C000

effects_address:
    incbin "../assets/fade_pixel_order.bin"
defc effects_length = $ - effects_address


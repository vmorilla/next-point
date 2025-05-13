SECTION code_user

PUBLIC _ula_interrupt_handler, _frame_counter

EXTERN _remaining_effect_length, _effects_handler, _sfx_effects_handler


_ula_interrupt_handler:
    ; Does not seem to be needed
	NEXTREG $C8, %00000001

    ; interruptions should be enabled asap to enable CTC interruptions in the middle 
    ei
    push af
    push hl
    ld hl, _frame_counter
    inc (hl)

    call _sfx_effects_handler

    pop hl
    pop af
    reti

_frame_counter:
    defb 0
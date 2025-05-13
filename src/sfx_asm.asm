SECTION code_user

include "macros.inc"

PUBLIC _sfx_effects_handler, _sfx_play_sound, _sfx_sounds_table, _sfx_init

; #define AY_WRITE(register) \
;     ld d, register : \
;     ld bc, AY_REG : \
;     out (c), d : \
;     ld b, AY_DATA >> 8 : \
;     out (c), a

; 	•	Bits 0–3: Volume level (0–15)
; 	•	Bit 4: Disable Tone (T) flag
; 	•	Bit 5: Change Tone Period
; 	•	Bit 6: Change Noise Period
; 	•	Bit 7: Disable Noise (N) flag ￼

; If Bit 5 is set, two bytes representing the new tone period (in little-endian format) follow. ￼

; If Bit 6 is set, one byte representing the new noise period follows.

; The end of an effect is indicated by the byte sequence 0xD0, 0x20. The player detects this sequence by checking for a noise period value equal to 0x20, signaling the end of the effect. ￼


   ; Mute all channels first
    ; ay_write(7, 0x3F);      // R7 mixer: 0x3F = disable all tone/noise (silence) [oai_citation:21‡chuntey.wordpress.com](https://chuntey.wordpress.com/tag/ay-chip/#:~:text=paired%20in%20the%20little,volume%20for%20that%20channel%20directly)

    ; // Set channel A tone frequency (e.g., period = 0x0100)
    ; ay_write(0, 0x00);      // R0: fine tune A (low 8 bits)
    ; ay_write(1, 0x01);      // R1: coarse tune A (high 4 bits, here 0x1<<8 -> period=0x0100)

    ; // Enable channel A tone, disable noise on A (and keep B,C disabled)
    ; ay_write(7, 0x3E);      // R7 mixer: 0x3E = 0b111110: enable tone A, no noise A, mute B/C [oai_citation:22‡chuntey.wordpress.com](https://chuntey.wordpress.com/tag/ay-chip/#:~:text=paired%20in%20the%20little,volume%20for%20that%20channel%20directly)

    ; // Set volume of channel A
    ; ay_write(8, 0x0F);      // R8: volume A = 15 (maximum volume, no envelope) [oai_citation:23‡chuntey.wordpress.com](https://chuntey.wordpress.com/tag/ay-chip/#:~:text=from%20all%20three%20channels%2C%2063,volume%20for%20that%20channel%20directly)

    ; // (Optional) brief delay loop to let tone play for some time
    ; for (volatile unsigned long i = 0; i < 100000UL; ++i) { /* simple delay */ }

    ; // Turn off sound (mute channel A)
    ; ay_write(8, 0x00);      // R8: volume A = 0 (silent)
    ; ay_write(7, 0x3F);      // R7: mute all channels again [oai_citation:24‡chuntey.wordpress.com](https://chuntey.wordpress.com/tag/ay-chip/#:~:text=paired%20in%20the%20little,volume%20for%20that%20channel%20directly)


; Modifies af, hl
_sfx_effects_handler:
    ld hl, (current_sound_address)
    ld a, h
    and a
    ret z           ; no sound effect

    push bc
    push de

    call sfx_play_frame
    ld (current_sound_address), hl

    pop de
    pop bc
    ret

    ; HL has the address of the sound effect pointer
sfx_play_frame:
    ld e, (hl)
    inc hl
    ld a, 0xd0
    cp e
    jr nz, no_end
    ld a, 0x20
    cp (hl)
    jr z, end
no_end:
    ld a, e
    and 0xf
    ld d, AY_VOLUME_C      ; Channel C volume
    call ay_write
    ld a, e
    and %10010000  ; Tone and noise channel C enable disable 
    rra
    rra
    or %00011011
    ld d, AY_MIXER
    call ay_write
    bit 5, e
    jr z, no_tone

    ; Writes the tone period (lo part)
    ld a, (hl)
    inc hl
    ld d, AY_TONE_C_LO
    call ay_write
    ; Writes the tone period (hi part)
    ld a, (hl)
    inc hl
    ld d, AY_TONE_C_HI
    call ay_write

no_tone:
    bit 6, e
    ret z 
    ld a, (hl)
    inc hl 
    ld d, AY_NOISE
    jp ay_write
end:
    ld a, 0x3f ; R7 disable all channels
    ld d, AY_MIXER
    call ay_write
    ld hl, 0 ; We end the sound effect

    ret

; ---------------------------------------------------------
; sfx_play_sound(sound_address) __z88dk_fastcall;
; hl = sound_address
; ---------------------------------------------------------
_sfx_play_sound:

    ld d, 0x7  ; R7 mixer
    ld a, 0x3f  ; R7 disable all channels
    call ay_write
    ld (current_sound_address), hl

    ret

current_sound_address:
    defw 0


; d = register
; a = value
ay_write:
    ld bc, AY_REG
    out (c), d
    ld b, AY_DATA >> 8
    out (c), a
    ret

; ------------------------------------------------------------
; TurboSound init
; ------------------------------------------------------------
_sfx_init:
    ld bc, AY_REG
    ld a, %11111111  ; ; Enable left+right audio, select AY1
    out (c), a

    ; Setup mapping of chip channels to stereo channels
    nextreg $08, %00011010 ; Use ABC, enable internal speaker $turbosound, enable DAC
    nextreg $09, %11100000 ; Enable mono for AY1-3

    ; Resets of AY-3 registers
    xor a
    ld d,0x0f
_sfx_init_loop:
    dec d
    call ay_write
    jr nz, _sfx_init_loop

    ret

;------------------------------------------------------------
; Sound effects table and data
; ------------------------------------------------------------

_sfx_sounds_table:
    dw _cursor_sound
    dw _select_sound


_cursor_sound:
    incbin "../assets/sfx/cursor.afx"
_select_sound:
    incbin "../assets/sfx/select.afx"

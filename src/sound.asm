
SECTION code_user

include "macros.inc"

PUBLIC _sound_interrupt_handler
PUBLIC _init_sound
PUBLIC _current_sample_pointer, _current_sample_length, _current_sample_page
PUBLIC _current_sample_pointer_b, _current_sample_length_b, _current_sample_page_b
PUBLIC _play_sound, _play_sound_channel_b

#define SOUNDS_TABLE_PAGE 100

_sound_interrupt_handler:
    ;NEXTREG INTERRUPT_STATUS_CTC, 1
    push af
    push bc
    push hl
    push ix

    ; Saves the current page in MMU 6
    READ_NEXTREG(REG_MMU6)
    push af

    ld ix, _current_sample_page
    ld bc, IO_DAC_M0
    call _sound_interrupt_handler_channel

    ld c, IO_DAC_L1
    ld ix, _current_sample_page_b
    call _sound_interrupt_handler_channel

    ; Restores the currengt page in MMU 6
    pop af
    nextreg REG_MMU6, a

    pop ix
    pop hl
    pop bc
    pop af
    NEXTREG INTERRUPT_STATUS_CTC, 1
    ei
    reti
; ---------------------------------------------------------------    

_sound_interrupt_handler_channel:
    ld hl, (ix + 3) ;  sample length
    ld a, h
    or l
    ret z
    dec hl
    ld (ix + 3), hl

    ld a, (ix + 0) ; sample page
    nextreg REG_MMU6, a
    ld hl, (ix + 1) ; sample pointer

    ld a, (hl)
    out (c), a
    inc hl
    ; Since using memory slot 6, the sample is in address 0xc000 to 0xdfff
    bit 5, h ; Runs after 0xdfff (new page)
    jr z, no_new_page

    ld h, $c0   ; sets h at the beginning of the page ($c000)
    inc (ix + 0) ; increases the sample page for next sample

no_new_page:
    ld (ix + 1), hl ; update sample pointer
    ret 
 
; TODO: use a structure

_current_sample_page:
    defb 0
_current_sample_pointer:
    defw 0
_current_sample_length:
    defw 0
_end_current_sample:

_current_sample_page_b:
    defb 0
_current_sample_pointer_b:
    defw 0
_current_sample_length_b:
    defw 0

; ---------------------------------------------------------------
; Structure size of the sound sample... used to calculate the offset of the sample
; and paste it in the current sample data
defc CURRENT_SAMPLE_SIZE = _end_current_sample - _current_sample_page 

_init_sound:
    ; Clock frequency = CPU frequency / ( 16 * ( 1 + time constant ) )
    ld h, 10000101b  ; No interrupt follows vector
                    ; 7: Enable (1) / disable (0) interrupt
                    ; 6: Counter mode (1) / timer mode (0)
                    ; 5: Prescaler 256 (1) / 16 (0)
                    ; 4: Edge selection: 0 = falling, 1 = rising
                    ; 3: Timer trigger: 0 = automatic trigger when time constant is loaded, 1 = clk starts timer
                    ; 2: Time constant: 0 = no time constant follows, 1 = time constant follows
                    ; 1: Reset: 0 = continue operation / 1 = software reset 
                    ; 0: Vector (0) / control word (1)
    ;ld l, 218      ; 217 : 8kHz = 28MHz / (16 * (1 + 218))
    
    ld l,  109     ; 16kHz = 28MHz / (16 * (1 + 109))
    ld	bc,CTC0		; Channel 0 port
	out	(c),h		; Control word
	out	(c),l	
    ret

;---------------------------------------------------------------
; play_sound(SoundSampleEnum) _z88dk_fastcall
;    a: sound sample number
;    Modifies: A, DE, HL, BC 
;---------------------------------------------------------------

EXTERN _sounds_table

; -----------------------------------------------------------------
; Puts the data of a sound sample in L in the selected channel
; L = Sound sample number
; BC = Address of sound data of the selected channel
;     BC = _current_sound_page or _current_sound_page_b
; ------------------------------------------------------------------
_play_sound_selected_channel:
    ; Saves the page of slot 6
    push af
    ; Multiplies the sound sample number by 5, to locate the entry of the sound sample data in the table
    ld e, l
    ld d, CURRENT_SAMPLE_SIZE
    mul d, e


    ; Sets slot 6 to the sound sample table page
    nextreg REG_MMU6, SOUNDS_TABLE_PAGE
    
    ; Gets the sound sample address
    ld hl, _sounds_table
    add hl, de

    ; Copies the sound 
    ld de, bc
    ld bc, CURRENT_SAMPLE_SIZE
    di
    ldir
    ei

    ; Restores the memory slot 6 page
    pop af
    nextreg REG_MMU6, a

    ret

; -----------------------------------------------------------------
; Puts the data of a sound sample in L in the channel A
; L = Sound sample number  (__z88dk_fastcall)
; ------------------------------------------------------------------
_play_sound:
    READ_NEXTREG(REG_MMU6)  ; Modifies BC!
    ld bc, _current_sample_page
    jr _play_sound_selected_channel

; -----------------------------------------------------------------
; Puts the data of a sound sample in L in the channel A
; L = Sound sample number (__z88dk_fastcall)
; ------------------------------------------------------------------
_play_sound_channel_b:
    READ_NEXTREG(REG_MMU6)  ; Modifies BC!
    ld bc, _current_sample_page_b
    jr _play_sound_selected_channel
; -----------------------------------------------------------------
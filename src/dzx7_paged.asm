
; ===============================================================
; Based on the code by Einar Saukas, Antonio Villena & Metalbrain
; ===============================================================
; 
; void* dzx7_paged(void *src, void *dst)
;
; Decompress the compressed block at address src to address dst.
; Address src is assumed to be in the slot 6 page. If the pointer
; exceeds the page, it will switch to the next bank using the
; paging calls.
; The function does not restore the slot 6 page and returns the
; next source address. This way, it can be used to chain multiple
; calls to decompress blocks, for instance, to fill in the layer2
; ===============================================================

include "macros.inc"

SECTION code_user

PUBLIC _dzx7_paged

_dzx7_paged:
    ; This function decompresses a block of data using the DZX7
    ; algorithm. The compressed data is read from the source address
    ; and the decompressed data is written to the destination address.
    ;
    ; entry : parameters stored in stack from right to left
    ; exit  : hl = following uncompressed block
    ;
    ; uses  : af, bc, de, hl

   ; enter : hl = void *src
   ;         de = void *dst
   ;
   ; exit  : hl = address following uncompressed block + 2 bytes
   ;
   ; uses  : af, bc, de, hl
    push ix
    ld ix, 4
    add ix, sp
    ld hl, (ix)                ; hl = src
    ld de, (ix + 2)            ; de = dst

    ld a, $80
       
dzx7p_copy_byte_loop:

    ldi                              ; copy literal byte
    call check_page
        
dzx7p_main_loop:

    call    dzx7p_next_bit
    jr      nc, dzx7p_copy_byte_loop ; next bit indicates either literal or sequence

; determine number of bits used for length (Elias gamma coding)

    push    de            ; stores destination in (sp)
    ld      bc, 0
    ld      d, b
        
dzx7p_len_size_loop:

    inc     d
    call    dzx7p_next_bit
    jr      nc, dzx7p_len_size_loop

; determine length

dzx7p_len_value_loop:

    call    nc, dzx7p_next_bit
    rl      c
    rl      b
    jr      c, dzx7p_exit         
    dec     d
    jr      nz, dzx7p_len_value_loop
    inc     bc                          ; adjust length

; determine offset

    ld e, (hl)                 ; load offset flag (1 bit) + offset value (7 bits)
    inc hl
    call check_page
    sla e
    inc e
    jr      nc, dzx7p_offset_end    ; if offset flag is set, load 4 extra bits
    ld      d, $10                  ; bit marker to load 4 bits
        
dzx7p_rld_next_bit:
    call    dzx7p_next_bit
    rl      d                       ; insert next bit into D
    jr      nc, dzx7p_rld_next_bit  ; repeat 4 times, until bit marker is out
    inc     d                       ; add 128 to DE
    srl	d			; retrieve fourth bit from D
        
dzx7p_offset_end:
    rr      e                       ; insert fourth bit into E

; copy previous sequence

    ex      (sp), hl                ; store source, restore destination
    push    hl                      ; store destination
    sbc     hl, de                  ; HL = destination - offset - 1
    pop     de                      ; DE = destination
    ldir
    pop hl                      ; restore source address (compressed data)
    jr nc, dzx7p_main_loop
        
dzx7p_next_bit:
    add a, a                    ; check next bit
    ret nz                      ; no more bits left?
    ld a, (hl)                 ; load another group of 8 bits
    inc hl
    call check_page
    rla
    ret

dzx7p_exit:
    pop de          ; restores the stack and returns
    pop ix          ; restores ix
    ret

    ; Checks if hl exceeds the slot 6 page. If so, it 
    ; moves to the next page.
check_page:
    bit 5, h
    ret z
    push af
    ld h, 0xc0          ; start of slot 6
    READ_NEXTREG(REG_MMU6)
    inc a
    nextreg REG_MMU6, a
    pop af
    ret
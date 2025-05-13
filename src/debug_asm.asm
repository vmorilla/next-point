SECTION code_user

PUBLIC __console_message

; --------------------------------------------------------------
; void _console_message(const char *message) __z88dk_fastcall;
; hl = pointer to message
; --------------------------------------------------------------
__console_message:
    ld bc, $8080
__console_message_loop:
    ld a, (hl)
    out (c), a
    and a
    ret z
    inc hl
    jr __console_message_loop
; --------------------------------------------------------------

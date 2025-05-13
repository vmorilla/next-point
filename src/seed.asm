SECTION code_user

PUBLIC _random_seed

    ; uint16_t random_seed(void) __z88dk_fastcall;
_random_seed:
    ld a,r
    ld l,a
    halt
    ld a,r
    ld h,a
    ret

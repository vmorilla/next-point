    SECTION code_crt_interrupt_vector_table

    EXTERN _default_interrupt_handler, _sound_interrupt_handler, _ula_interrupt_handler, _effects_handler

    PUBLIC _interrupt_vector_table

    ORG $FC00
_interrupt_vector_table:
    defw _default_interrupt_handler ; 0: line interrupt
    defw _default_interrupt_handler ; 1: UART0 RX
    defw _default_interrupt_handler ; 2: UART1 RX
    defw _sound_interrupt_handler   ; 3: CTC channel 0
    defw _effects_handler ; 4: CTC channel 1
    defw _default_interrupt_handler ; 5: CTC channel 2
    defw _default_interrupt_handler ; 6: CTC channel 3
    defw _default_interrupt_handler ; 7: CTC channel 4
    defw _default_interrupt_handler ; 8: CTC channel 5
    defw _default_interrupt_handler ; 9: CTC channel 6
    defw _default_interrupt_handler ; 10: CTC channel 7
    defw _ula_interrupt_handler     ; 11: ULA interrupt
    defw _default_interrupt_handler ; 12: UART0 TX
    defw _default_interrupt_handler ; 13: UART1 TX
    defw _default_interrupt_handler ; 14: Not documented
    defw _default_interrupt_handler ; 15: Not documented



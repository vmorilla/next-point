SECTION code_user

EXTERN _interrupt_vector_table

PUBLIC _hardware_interrupt_mode, _default_interrupt_handler

defc REG_INTERRUPT_CONTROL = 0xC0

defc REG_INTERRUPT_ENABLE_0 = 0xC4
defc REG_INTERRUPT_ENABLE_1 = 0xC5
defc REG_INTERRUPT_ENABLE_2 = 0xC6

defc REG_INTERRUPT_STATUS_0 = 0xC8
defc REG_INTERRUPT_STATUS_1 = 0xC9
defc REG_INTERRUPT_STATUS_2 = 0xCA

defc REG_DMA_INTERRUPT_ENABLE_0 = 0xCC
defc REG_DMA_INTERRUPT_ENABLE_1 = 0xCD
defc REG_DMA_INTERRUPT_ENABLE_2 = 0xCE

include "macros.inc"

; Important.... init sound must be called after setting the hardware interrupt mode...
; CTC interruptions are happending despite the corresponding register is set to 0

_hardware_interrupt_mode:
    di ; disable interrupts

    ; ** Quite important is that you should be changing nextreg 0xc0 with a read-modify-write operation.  
    ; That's because starting in nextzxos versions for core 3.01.10, nextzxos operates in the stackless nmi mode 
    ; which is controlled by bit 3. (see https://discord.com/channels/556228195767156758/692885312296190102/894284968614854749)

    READ_NEXTREG(REG_INTERRUPT_CONTROL)
    and %00011110
    or (_interrupt_vector_table & %11100000) | %00000001
    nextreg REG_INTERRUPT_CONTROL, a
    ;nextreg REG_INTERRUPT_CONTROL, (_interrupt_vector_table & %11100000) | %00000001
    ; nextreg REG_INTERRUPT_ENABLE_0, 1  ; Enable ULA interrupts
    nextreg REG_INTERRUPT_ENABLE_0, %10000001 ; Enable ULA and expansion bus interrupts
    nextreg REG_INTERRUPT_ENABLE_1, 0 ; enable CTC channel 0 interrupt
    nextreg REG_INTERRUPT_ENABLE_2, 0

	nextreg REG_INTERRUPT_STATUS_0, $FF ; 
	nextreg REG_INTERRUPT_STATUS_1, $FF ; Set status bits to clear
	nextreg REG_INTERRUPT_STATUS_2, $FF ; 

    nextreg REG_DMA_INTERRUPT_ENABLE_0,0 
    ; Enabling DMA interrupts does not seem to cause any harm
	nextreg REG_DMA_INTERRUPT_ENABLE_1,0
	;nextreg REG_DMA_INTERRUPT_ENABLE_1,0xff
	nextreg REG_DMA_INTERRUPT_ENABLE_2,0 

    ld a, _interrupt_vector_table >> 8
    ld i, a
    im 2 ; enable HW Interrupt Mode 2
    ei
    ret

_default_interrupt_handler:
    ei
    reti


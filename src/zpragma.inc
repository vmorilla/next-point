; This can be used to append the content of a mmap.inc file; however, it only admits newly defined sections.
; For redefining existing ones, we must use #pragma output CRT_ORG_PAGE_XX = 0xYYYY
; #pragma output CRT_APPEND_MMAP = 1

; Interrupt vector table
#pragma output CRT_ORG_code_crt_interrupt_vector_table = 0xF000

; Additional RAM for code in ROM area
#pragma output CRT_ORG_BANK_8 = 0x0000

; Layer 2 pages: 18 - 27, 28 - 37

; Redefines the default memory map for sprite pages
#pragma output CRT_ORG_PAGE_38 = 0xc000
#pragma output CRT_ORG_PAGE_39 = 0xc000
#pragma output CRT_ORG_PAGE_40 = 0xc000
#pragma output CRT_ORG_PAGE_41 = 0xc000
#pragma output CRT_ORG_PAGE_42 = 0xc000
#pragma output CRT_ORG_PAGE_43 = 0xc000
#pragma output CRT_ORG_PAGE_44 = 0xc000
#pragma output CRT_ORG_PAGE_45 = 0xc000
#pragma output CRT_ORG_PAGE_46 = 0xc000
#pragma output CRT_ORG_PAGE_47 = 0xc000
#pragma output CRT_ORG_PAGE_48 = 0xc000
#pragma output CRT_ORG_PAGE_49 = 0xc000

; Redefines the default memory map for layer 2 compressed data pages
#pragma output CRT_ORG_PAGE_70 = 0xc000
#pragma output CRT_ORG_PAGE_71 = 0xc000
#pragma output CRT_ORG_PAGE_72 = 0xc000
#pragma output CRT_ORG_PAGE_73 = 0xc000
#pragma output CRT_ORG_PAGE_74 = 0xc000
#pragma output CRT_ORG_PAGE_75 = 0xc000
#pragma output CRT_ORG_PAGE_76 = 0xc000
#pragma output CRT_ORG_PAGE_77 = 0xc000
#pragma output CRT_ORG_PAGE_78 = 0xc000
#pragma output CRT_ORG_PAGE_79 = 0xc000
#pragma output CRT_ORG_PAGE_80 = 0xc000
#pragma output CRT_ORG_PAGE_81 = 0xc000
#pragma output CRT_ORG_PAGE_82 = 0xc000
#pragma output CRT_ORG_PAGE_83 = 0xc000
#pragma output CRT_ORG_PAGE_84 = 0xc000
#pragma output CRT_ORG_PAGE_85 = 0xc000
#pragma output CRT_ORG_PAGE_86 = 0xc000
#pragma output CRT_ORG_PAGE_87 = 0xc000
#pragma output CRT_ORG_PAGE_88 = 0xc000
#pragma output CRT_ORG_PAGE_89 = 0xc000
#pragma output CRT_ORG_PAGE_90 = 0xc000

; Redefines the default memory map for opponent choice description and functions
#pragma output CRT_ORG_PAGE_97 = 0xc000

; Redefines the default memory map for the fadeout effect data
#pragma output CRT_ORG_PAGE_98 = 0xc000

; Redefines the default memory map for tilemap pages
#pragma output CRT_ORG_PAGE_99 = 0xc000

; Redefines the default memory map for sounds (PAGE 100 and onwards)
; are not necessary since explicitly defined in the corresponding .asm file

# Specify the compiler and flags
CC = zcc
AS = zcc
LD = zcc
ASERUN = ../../ase2next/dist/ase2next.js
SPRITES = ../assets/sprites

TARGET = +zxn -subtype=nex
VERBOSITY = -vn   # -v for verbose, -vn for not verbose
CRT=31	# CRT=31 is the most minimalistic c, CRT=0 to use stdio

DEBUG_FLAGS= #-DDEBUG # #-DDISABLE_EFFECTS #
C_OPT_FLAGS=-SO3 --max-allocs-per-node200000 --opt-code-size
CFLAGS = $(TARGET) $(VERBOSITY) --list -c --c-code-in-asm -clib=sdcc_iy -Cz"--clean" --math32 $(C_OPT_FLAGS) $(DEBUG_FLAGS)
ASFLAGS= $(TARGET) $(VERBOSITY) -c --list -m $(DEBUG_FLAGS)
# With -startup=31, we get the most minimalistic c run time for spectrum (see https://github.com/z88dk/z88dk/blob/master/doc/ZXSpectrumZSDCCnewlib_02_HelloWorld.md)
LDFLAGS= $(TARGET) $(VERBOSITY) --list -m -clib=sdcc_iy -startup=$(CRT) -pragma-include:zpragma.inc

LIBS = -lmath32

# Specify the source files
GENERATED_C = skins.c
GENERATED_H = skins.h frames_def.h
GENERATED_ASM = sprites_page_38.asm
GENERATED_SRCS = $(GENERATED_C) $(GENERATED_H) $(GENERATED_ASM)

SRCS_C = $(wildcard *.c) $(GENERATED_C)
SRCS_ASM = $(wildcard *.asm) $(GENERATED_ASM)
SPRITE_FILES := $(wildcard $(SPRITES)/*.aseprite)

# Specify the output file
OUTPUT_DIR = ../build
OUTPUT = $(OUTPUT_DIR)/next-point.nex

# Specify the object files in the build folder
OBJS = $(patsubst %.c,$(OUTPUT_DIR)/%.o,$(SRCS_C)) $(patsubst %.asm,$(OUTPUT_DIR)/%.o,$(SRCS_ASM))

all: $(OUTPUT) check_length

debug-var:
	@echo "SRCS_C = $(SRCS_C)"
	@echo "SRCS_ASM = $(SRCS_ASM)"
	@echo "GENERATED_SRCS = $(GENERATED_SRCS)"
	@echo "SPRITES = $(SPRITES)"
	@echo "OBJS = $(OBJS)"

# Rules
skins.c skins.h sprites_page_38.asm: $(SPRITE_FILES)
	$(ASERUN) -s ./ -a ../assets/ -b 38 $^

sprite_slots.c: skins.h

$(OUTPUT_DIR)/%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

$(OUTPUT_DIR)/%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

$(OUTPUT): $(OBJS)
	$(LD) $(LDFLAGS) $(LIBS) -o $@ $^ -create-app

# Rule to check file length
check_length:
	@FILE=$(OUTPUT_DIR)/next-point_CODE.bin; \
	MAX_SIZE=16384; \
	ACTUAL_SIZE=$$(stat -f%z "$$FILE"); \
	if [ $$ACTUAL_SIZE -gt $$MAX_SIZE ]; then \
		echo "Error: $$FILE exceeds the maximum allowed size of $$MAX_SIZE bytes (actual: $$ACTUAL_SIZE bytes)."; \
		exit 1; \
	else \
		echo "$$FILE is within the size limit (actual: $$ACTUAL_SIZE bytes)."; \
	fi

# Clean rule
clean:
	rm -f $(OUTPUT_DIR)/*
	rm -f *.o *.bin *.lis *.sym
	rm -f $(GENERATED_SRCS)
	rm -f ../assets/tiledefs.bin ../assets/palettes.bin

.PHONY: all clean fonts court check_length


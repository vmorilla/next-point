# Tools and paths
ASERUN = ../../ase2next/dist/ase2next.js
SPRITES = ../assets/sprites
LY2_ASSETS_DIR = ../assets/layer2
ZX7 = z88dk-zx7
FFMPEG = ffmpeg -v error
LY2_PROCESS = ../utils/layer2_screens.py


SOUND_OPTIONS = -ac 1 -ar 16000 -acodec pcm_u8 -af loudnorm=I=-16:LRA=11:TP=-1.5 -y -f u8 #-f wav
# Specify the source files
SPRITE_FILES := $(wildcard $(SPRITES)/*.aseprite)
#LAYER2_ASSETS := $(filter-out $(LY2_ASSETS_DIR)/default-layer2.aseprite, $(wildcard $(LY2_ASSETS_DIR)/*.aseprite))
LAYER2_ASSETS := $(wildcard $(LY2_ASSETS_DIR)/*.aseprite)
SOUND_FILES := $(wildcard sounds/*.wav)

# Source files generated from layer 2 assets
LY2_ASM = ../src/layer2_screens.asm
LY2_HEADER = ../src/layer2_screens.h

# Specify the output directory
OUTPUT_DIR = ../build
LAYER2_OUTPUT_DIR = $(OUTPUT_DIR)/layer2

# Layer 2 bank compressed files
BANK_ZX_FILES := $(foreach file,$(LAYER2_ASSETS),\
               $(foreach suffix,0 1 2 3 4,\
				$(LAYER2_OUTPUT_DIR)/$(notdir $(basename $(file))).bank_$(suffix).ly2.zx7))

# Layer 2 bank files (uncompressed)
BANK_LY2_FILES := $(foreach file,$(LAYER2_ASSETS),\
               $(foreach suffix,0 1 2 3 4,\
                $(OUTPUT_DIR)/$(notdir $(basename $(file))).bank_$(suffix).ly2))

SOUND_RAW_FILES := $(foreach file,$(SOUND_FILES),\
			   $(OUTPUT_DIR)/sounds/$(notdir $(basename $(file))).raw)

all: dirs layer2 fonts sounds

layer2: $(BANK_ZX_FILES) $(LY2_HEADER) dirs

sounds: $(SOUND_RAW_FILES) ../src/sound_samples.asm
	../utils/sound_table.py ../build > ../src/sound_samples.asm

# # Generate the layer 2 bank compressed files
# $(BANK_ZX_FILES): $(OUTPUT_DIR)/%.ly2
# 	$(ZX7) -f $< $@

# Rule to generate .ly2 bank files from .aseprite files
$(LAYER2_OUTPUT_DIR)/%.bank_0.ly2 $(LAYER2_OUTPUT_DIR)/%.bank_1.ly2 $(LAYER2_OUTPUT_DIR)/%.bank_2.ly2 $(LAYER2_OUTPUT_DIR)/%.bank_3.ly2 $(LAYER2_OUTPUT_DIR)/%.bank_4.ly2: $(LY2_ASSETS_DIR)/%.aseprite
	$(ASERUN) -l $(LAYER2_OUTPUT_DIR)/$*.bank_ $<

# Rule to compress .ly2 files to .ly2.zx7
$(LAYER2_OUTPUT_DIR)/%.ly2.zx7: $(LAYER2_OUTPUT_DIR)/%.ly2
	$(ZX7) -f $< $@

$(LY2_HEADER): $(BANK_ZX_FILES)
	$(LY2_PROCESS) $(LY2_HEADER) $(BANK_ZX_FILES) > $(LY2_ASM)

$(OUTPUT_DIR)/sounds/%.raw: ../assets/sounds/%.wav
	$(FFMPEG) -i $< $(SOUND_OPTIONS) $@

# $(OUTPUT_DIR)/sounds/%.raw: ../assets/sounds/%.m4a
# 	$(FFMPEG) -i $< $(SOUND_OPTIONS) $@

# $(OUTPUT_DIR)/sounds/%.raw: ../assets/sounds/%.mov
# 	$(FFMPEG) -i $< $(SOUND_OPTIONS) $@


fonts:
	$(ASERUN) -t $(OUTPUT_DIR)/fonts/tiledefs.bin -c $(OUTPUT_DIR)/fonts/palettes.bin -m $(OUTPUT_DIR)/fonts/balloon.map fonts/tiles.aseprite
	$(ZX7) -f $(OUTPUT_DIR)/fonts/tiledefs.bin
	$(ASERUN) -t $(OUTPUT_DIR)/fonts/menu-tiledefs.bin -c $(OUTPUT_DIR)/fonts/menu-palettes.bin fonts/menu.aseprite
	$(ZX7) -f $(OUTPUT_DIR)/fonts/menu-tiledefs.bin
	
dirs:
	mkdir -p $(OUTPUT_DIR)/voices
	mkdir -p $(OUTPUT_DIR)/sounds
	mkdir -p $(OUTPUT_DIR)/fonts
	mkdir -p $(LAYER2_OUTPUT_DIR)

# Clean rule
clean:
	rm -fR $(OUTPUT_DIR)/*
	rm -f $(LY2_ASM) $(LY2_HEADER)

.PHONY: all clean dirs fonts layer2 sounds

# Preserve files from being removed... more specifically ../build/.bank_0.ly2 files
.SECONDARY:
#!/bin/bash

# Convert sound file to raw format to use with the sound driver
# Usage: sound2raw.sh <sound file> <output file>

# if [ $# -ne 2 ]; then
#     echo "Usage: $0 <sound file> <output file>"
#     exit 1
# fi

#ffmpeg -i "$1" -ac 1 -ar 8000 -sample_fmt u8 -f s8 "$2"

# This one works, but very low volume
PARAMETERS="-ac 1 -ar 16000 -acodec pcm_u8 -af loudnorm=I=-16:LRA=11:TP=-1.5 -y"

# Iterates through all mp3 and mov files in the assets sound dir
for i in assets/sounds/*.mp3 assets/sounds/*.mov; do
    # Convert to wav format 
    ffmpeg -i "$i" $PARAMETERS -f wav "${i%.*}.wav"
done

for i in assets/sounds/*.wav; do
    # Convert to raw format
    echo "Converting $i..."
    ffmpeg -i "$i" $PARAMETERS -f u8 - | utils/remove_trailing_80.py > "${i%.*}.raw"
done

#ffmpeg -i "$1" -ac 1 -ar 8000 -acodec pcm_u8 -af loudnorm=I=-16:LRA=11:TP=-1.5 -f u8 "$2"

#ffmpeg -i "$1" -ac 1 -ar 8000 -acodec pcm_s8 -f s8 "$2"


#ffmpeg -i "$1" -ac 1 -ar 8000 -f wav -acodec pcm_u8 output.wav

#!/bin/bash
V2PLPATH="../../../framer"

VENV_PATH="${V2PLPATH}/.venv/bin"

source "${VENV_PATH}/activate"
 
V2PL="${V2PLPATH}/v2pl"
#YTDL="${HOME}/bin/yt-dlp_macos --recode-video mp4 --download-sections"
YTDL="yt-dlp --recode-video mp4 --download-sections"

SKELETONS_PATH="/Users/vmorilla/Library/Application Support/Aseprite/extensions/pixellab/skeleton_references"

# Videos


#${V2PL} -s 0 -e 92 -o 0 -f 0 43 56 64 69 74 -n service -d north -i 9 -t Serving ../videos/nadal-service.mp4
#${V2PL} -n service -d south -i 9 -t Serving ../videos/alcaraz-front-serve.mp4

# Servicio alcaraz frente
#${YTDL} "*00:00:25-00:00:30" "https://www.youtube.com/watch?v=tKgIBrKcr3Y" -o alcaraz-front-serve
#${V2PL} -s 28 -e 248 -o 0 -f 39 118 143 178 183 189 214 -n service -d south -i 9 -t Serving ../videos/alcaraz-front-serve.mp4

# Nadal mov lateral
# Does not work
#${YTDL} "*00:00:22-00:00:23" "https://www.youtube.com/watch?v=gkIsvlZDG-Y&t=110s" -o ../videos/nadal-lateral-move
#${V2PL} -n latmove -d south -i 10 -t "Lateral move" ../videos/nadal-lateral-move.webm



# Trainer drive south
#${V2PL} -s 0 -e 11 -o 0 -f 0 3 6 10 -n drive -d south -i 10 -t Drive ../videos/footwork1.webm

# Trainer lateral move south
# ${V2PL} -s 18 -e 32 -o 0 -f 5 8 10 13 -n latmove -d south -i 11 -t Lateral ../videos/footwork1.webm
# Djokovic backhand north
#${V2PL} -s 280 -e 640 -n backhand-north -d north -i 12 -t Backhand ../videos/two-handed-backhand.webm

# Djokovic drive
#${YTDL} "*01:05:34-01:05:37" "https://www.youtube.com/watch?v=tMmB10l_i44" -o ../videos/djok-back-drive

# Nadal - Federer
#${YTDL} "*00:05:34-00:05:37" "https://www.youtube.com/watch?v=tMmB10l_i44" -o ../videos/nadal-federer-test


# Djokovic - backhand drive - front view
# ${V2PL} -n backhand -d  south -i 10 -t "Backhand" "../videos/Djokovic Backhand.webm"
# ${V2PL} -s 11163 -e 11280 -o 1 -f 67 83 90 108 -n backhand -d south -i 12 -t Backhand "../videos/Djokovic Backhand.webm"

# Djokovic - backhand drive - back view
#${V2PL} -s 59359 -e 59388 -o 0 -f 6 16 22 26 29 -n backhand -d north -i 10 -t "Backhand" ../videos/nadal-djok.webm
#${V2PL} -n backhand -d north -i 10 -t Backhand ../videos/nadal-djok.webm



cp version.json "${SKELETONS_PATH}/"
cp service.json drive.json latmove.json backhand.json "${SKELETONS_PATH}/pixellab/normal/"
#!/usr/bin/env python3

import random
from pathlib import Path
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Screen dimensions (typical for 8-bit systems could be 160x192, 256x192, etc.)
WIDTH = 80
HEIGHT = 32

# Create a list of all pixels
pixels = [(x, y) for x in range(WIDTH) for y in range(HEIGHT)]


# Assign a probability weight based on x (left-to-right bias)
# Pixels on the left have lower x, so we give them higher weights initially
def pixel_weight(x, y):
    return np.exp(-x / (WIDTH / 5)) + random.uniform(0, 0.3)


# Shuffle based on weights
pixels.sort(key=lambda p: pixel_weight(p[0], p[1]))

# Visualization
screen = np.ones((HEIGHT, WIDTH))  # 1 for white, 0 for black

fig, ax = plt.subplots()
img = ax.imshow(screen, cmap="gray", vmin=0, vmax=1)
ax.axis("off")


def update(frame):
    if frame < len(pixels):
        x, y = pixels[frame]
        screen[y, x] = 0
        img.set_array(screen)
    return [img]


ani = animation.FuncAnimation(
    fig, update, frames=len(pixels), interval=1, blit=True, repeat=False
)

plt.show()

# Output the list of pixel coordinates for use in the 8-bit system

# Compute 16-bit addresses
addresses = [0x4000 + (x + y * WIDTH) * 2 for (x, y) in pixels]

# Save as 16-bit values to a binary file
output_path = Path("fade_pixel_order.bin")
with open(output_path, "wb") as f:
    for addr in addresses:
        f.write(addr.to_bytes(2, byteorder="little"))

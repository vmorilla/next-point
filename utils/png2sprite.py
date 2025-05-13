#!/usr/bin/env python

import argparse
from PIL import Image

TRANSPARENT_COLOR = 227


def rgb_to_gggrrrbb(r, g, b, a):
    # Convert 8-bit RGB to 3-bit G, 3-bit R, and 2-bit B
    if a == 0:
        return TRANSPARENT_COLOR
    else:
        g = (g >> 5) & 0b111
        r = (r >> 5) & 0b111
        b = (b >> 6) & 0b11
        return (r << 5) | (g << 2) | b


def export_next_sprite(file_path, output_file):
    try:
        # Open the image file
        with Image.open(file_path) as img:
            img = img.convert("RGBA")  # Ensure image is in RGBA format
            width, height = img.size
            pixels = img.load()

            # Create a list to store the converted pixel data
            pixel_data = []

            # Obtain the sprite width and height as multiples of 16
            sp_width = (width + 15) & ~15
            sp_height = (height + 15) & ~15

            # Iterate through sprite blocks of 16x16 pixels
            for y in range(0, sp_height, 16):
                for x in range(0, sp_width, 16):
                    for j in range(16):
                        for i in range(16):
                            if x + i < width and y + j < height:
                                r, g, b, a = pixels[x + i, y + j]
                                pixel_data.append(rgb_to_gggrrrbb(r, g, b, a))
                            else:
                                pixel_data.append(TRANSPARENT_COLOR)

            # Export the pixel data to a binary file
            with open(output_file, "wb") as f:
                f.write(bytearray(pixel_data))

            print(f"Exported {sp_width / 16}x{sp_height / 16} sprite to: {output_file}")

    except Exception as e:
        print(f"Error opening image: {e}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Convert a PNG file to a binary file with each pixel represented as a byte (GGGRRRBB)."
    )
    parser.add_argument("input_file", help="Path to the input PNG file")
    parser.add_argument("output_file", help="Path to the output binary file")

    args = parser.parse_args()

    export_next_sprite(args.input_file, args.output_file)

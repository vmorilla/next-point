"""
Helper functions for the NEXT file format
"""

from io import BufferedWriter
from PIL import Image


def img_to_ly2(img: Image, f: BufferedWriter):
    """
    Converts an image to a layer2 format file

    Args:
        img (Image): The image to convert
        f (BufferedWriter): The file to write the converted image to
    """
    pixels = img.load()
    for x in range(img.width):
        for y in range(img.height):
            r, g, b = pixels[x, y]
            r = (r >> 5) << 2  # 3 bits for red
            g = (g >> 5) << 5  # 3 bits for green
            b = b >> 6  # 2 bits for blue
            color = r | g | b
            f.write(bytes([color]))

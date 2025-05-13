#!/usr/bin/env python
import cv2
import argparse
import numpy as np
from collections import Counter


output_path = "grid_output.png"


def get_most_common_color(cell):
    # Reshape to a list of pixels
    pixels = cell.reshape(-1, 3)
    # Convert to tuple for hashing
    pixel_tuples = [tuple(p) for p in pixels]
    # Count colors and return the most common
    most_common = Counter(pixel_tuples).most_common(1)
    return (
        np.array(most_common[0][0], dtype=np.uint8)
        if most_common
        else np.array([0, 0, 0], dtype=np.uint8)
    )


def generate_grid_image(image, grid_size, offset_x, offset_y):
    height, width, _ = image.shape
    grid_rows = (height - offset_y) // int(grid_size)
    grid_cols = (width - offset_x) // int(grid_size)

    result_img = np.zeros((grid_rows, grid_cols, 3), dtype=np.uint8)

    for row in range(grid_rows):
        for col in range(grid_cols):
            y = int(offset_y + row * grid_size)
            x = int(offset_x + col * grid_size)
            cell = image[y : y + int(grid_size), x : x + int(grid_size)]
            color = get_most_common_color(cell)
            result_img[row, col] = color

    return result_img


def draw_grid(image, grid_size, offset_x, offset_y, scale=2):
    height, width = image.shape[:2]
    new_size = (width * scale, height * scale)

    # Resize image to larger canvas
    large_img = cv2.resize(image, new_size, interpolation=cv2.INTER_LINEAR)

    # Scale grid size and offsets
    grid_size_scaled = int(grid_size * scale)
    offset_x_scaled = int(offset_x * scale)
    offset_y_scaled = int(offset_y * scale)

    # Draw grid lines
    for x in range(offset_x_scaled, new_size[0], grid_size_scaled):
        cv2.line(
            large_img,
            (x, 0),
            (x, new_size[1]),
            (255, 255, 255),
            1,
            lineType=cv2.LINE_AA,
        )
    for y in range(offset_y_scaled, new_size[1], grid_size_scaled):
        cv2.line(
            large_img,
            (0, y),
            (new_size[0], y),
            (255, 255, 255),
            1,
            lineType=cv2.LINE_AA,
        )

    # Downscale to original size for display
    final_img = cv2.resize(large_img, (width, height), interpolation=cv2.INTER_AREA)
    return final_img


def interactive_grid_viewer(image_path, initial_grid_size=50.0):
    image = cv2.imread(image_path)
    if image is None:
        print("Error: Could not load image.")
        return

    grid_size = initial_grid_size
    offset_x = 0.0
    offset_y = 0.0
    step = 0.5  # Subpixel step size

    while True:
        img_with_grid = draw_grid(image, grid_size, offset_x, offset_y, scale=4)
        cv2.imshow("Grid Viewer", img_with_grid)
        key = cv2.waitKey(0) & 0xFF

        if key == ord("q"):
            break
        elif key == ord("+") or key == ord("="):
            grid_size += step
            offset_x = (offset_x - step) % grid_size
            offset_y = (offset_y - step) % grid_size
        elif key == ord("-") and grid_size > step:
            grid_size -= step
            offset_x = (offset_x + step) % grid_size
            offset_y = (offset_y + step) % grid_size
        elif key == ord("a"):
            offset_x = (offset_x - step) % grid_size
        elif key == ord("d"):
            offset_x = (offset_x + step) % grid_size
        elif key == ord("w"):
            offset_y = (offset_y - step) % grid_size
        elif key == ord("s"):
            offset_y = (offset_y + step) % grid_size
        elif key == 13:  # Enter key
            print("Generating grid image...")
            result = generate_grid_image(
                image, round(grid_size), round(offset_x), round(offset_y)
            )
            output_path = "grid_output.png"
            cv2.imwrite(output_path, result)
            print(f"✅ Saved grid image to {output_path}")

    cv2.destroyAllWindows()


def main():
    parser = argparse.ArgumentParser(
        description="Interactive image grid viewer with subpixel steps."
    )
    parser.add_argument("image_path", help="Path to the image file.")
    parser.add_argument(
        "--grid_size",
        type=float,
        default=50.0,
        help="Initial grid size (float, allows subpixel steps).",
    )
    args = parser.parse_args()

    interactive_grid_viewer(args.image_path, initial_grid_size=args.grid_size)


if __name__ == "__main__":
    main()

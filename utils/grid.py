#!/usr/bin/env python3
import argparse
import cv2
import numpy as np
from collections import Counter


def get_cell_color(cell, method="average"):
    # Reshape to a list of pixels
    pixels = cell.reshape(-1, 3)

    if method == "most_common":
        # Convert to tuple for hashing and get the most common color
        pixel_tuples = [tuple(p) for p in pixels]
        color = Counter(pixel_tuples).most_common(1)
        return (
            np.array(color[0][0], dtype=np.uint8)
            if color
            else np.array([0, 0, 0], dtype=np.uint8)
        )
    elif method == "average":
        # Compute the average color
        avg_color = np.mean(pixels, axis=0)
        return avg_color.astype(np.uint8)
    else:
        raise ValueError("Unknown method. Use 'most_common' or 'average'")


def generate_grid_image(image, horizontal_lines, vertical_lines):
    width = len(vertical_lines) - 1
    height = len(horizontal_lines) - 1
    result_img = np.zeros((height, width, 3), dtype=np.uint8)

    for row in range(height):
        for col in range(width):
            cell = image[
                horizontal_lines[row] + 1 : horizontal_lines[row + 1],
                vertical_lines[col] + 1 : vertical_lines[col + 1],
            ]
            color = get_cell_color(cell)
            result_img[row, col] = color

    return result_img


def compare_rows(image, row1, row2, method="euclidean"):
    """
    Compare two horizontal lines (rows) in an image and compute per-column differences.

    Args:
        image: Input image as a NumPy array (H, W, 3).
        row1, row2: Row indices to compare.
        method: 'euclidean', 'squared', or 'abs'

    Returns:
        A 1D array of differences for each column.
    """
    if row1 < 0 or row2 < 0 or row1 >= image.shape[0] or row2 >= image.shape[0]:
        raise ValueError("Row indices are out of bounds.")

    line1 = image[row1 : row1 + 1, :, :].astype(np.float32)  # Shape: (1, W, 3)
    line2 = image[row2 : row2 + 1, :, :].astype(np.float32)

    if method == "euclidean":
        diff = np.linalg.norm(line1 - line2, axis=2).flatten()
    elif method == "squared":
        diff = np.sum((line1 - line2) ** 2, axis=2).flatten()
    elif method == "abs":
        diff = np.mean(np.abs(line1 - line2), axis=2).flatten()
    else:
        raise ValueError("Unknown method. Use 'euclidean', 'squared', or 'abs'.")

    return np.mean(diff)


def compare_columns(image, col1, col2, method="euclidean"):
    """
    Compare two vertical lines (columns) in an image and compute per-row differences.

    Args:
        image: Input image as a NumPy array (H, W, 3).
        col1, col2: Column indices to compare.
        method: 'euclidean', 'squared', or 'abs'

    Returns:
        A 1D array of differences for each row.
    """
    if col1 < 0 or col2 < 0 or col1 >= image.shape[1] or col2 >= image.shape[1]:
        raise ValueError("Column indices are out of bounds.")

    line1 = image[:, col1 : col1 + 1, :].astype(np.float32)  # Shape: (H, 1, 3)
    line2 = image[:, col2 : col2 + 1, :].astype(np.float32)

    if method == "euclidean":
        diff = np.linalg.norm(line1 - line2, axis=2).flatten()
    elif method == "squared":
        diff = np.sum((line1 - line2) ** 2, axis=2).flatten()
    elif method == "abs":
        diff = np.mean(np.abs(line1 - line2), axis=2).flatten()
    else:
        raise ValueError("Unknown method. Use 'euclidean', 'squared', or 'abs'.")

    return np.mean(diff)


def compare_neightbour_rows(image, row, method="euclidean"):
    return compare_rows(image, row, row - 1, method) + compare_rows(
        image, row, row + 1, method
    )


def compare_neightbour_columns(image, col, method="euclidean"):
    return compare_columns(image, col, col - 1, method) + compare_columns(
        image, col, col + 1, method
    )


def compute_variance_diff(img, vertical: bool, min_spacing, max_spacing):
    height, width = img.shape[:2]
    lines = []
    max_pos = width if vertical else height

    pos = -min_spacing + 1
    while pos < max_pos - min_spacing:
        min_range = pos + min_spacing
        max_range = min(min_range + max_spacing + 1, max_pos - 2)
        if max_range - min_range >= min_spacing:
            test_range = range(min_range, max_range)
            variances = []
            if vertical:
                variances = [compare_neightbour_columns(img, p) for p in test_range]
            else:
                variances = [compare_neightbour_rows(img, p) for p in test_range]

            # Find the index with the maximum variance
            max_index = np.argmax(variances)
            pos = test_range[max_index]
            print(f"Found line at {pos} with variance {variances[max_index]}")
            lines.append(pos)
        else:
            break

    return lines


def draw_adaptive_grid(image, v_lines, h_lines):
    output = image.copy()
    for x in v_lines:
        cv2.line(output, (x, 0), (x, image.shape[0]), (0, 255, 0), 1)
    for y in h_lines:
        cv2.line(output, (0, y), (image.shape[1], y), (0, 255, 0), 1)
    return output


def main():
    parser = argparse.ArgumentParser(
        description="Adaptive grid based on color variance."
    )
    parser.add_argument("image_path", help="Path to image file")
    parser.add_argument(
        "--min_spacing", type=int, default=11, help="Minimum spacing between grid lines"
    )
    parser.add_argument(
        "--max_spacing",
        type=int,
        default=20,
        help="Maximum spacing between grid lines",
    )
    parser.add_argument(
        "--output", default="adaptive_grid.png", help="Path to save output image"
    )
    args = parser.parse_args()

    image = cv2.imread(args.image_path)
    if image is None:
        print("Error: Cannot load image.")
        return

    print("🔍 Scanning for vertical lines...")
    v_lines = compute_variance_diff(image, True, args.min_spacing, args.max_spacing)
    print(f"Found {len(v_lines)} vertical lines.")

    print("🔍 Scanning for horizontal lines...")
    h_lines = compute_variance_diff(image, False, args.min_spacing, args.max_spacing)
    print(f"Found {len(h_lines)} horizontal lines.")

    result = draw_adaptive_grid(image, v_lines, h_lines)
    cv2.imwrite(args.output, result)
    print(f"✅ Adaptive grid saved to {args.output}")

    cv2.imshow("Adaptive Grid", result)

    result = generate_grid_image(image, h_lines, v_lines)
    cv2.imwrite(args.output, result)
    print(f"✅ Saved grid image to {args.output}")

    cv2.waitKey(0)
    cv2.destroyAllWindows()


if __name__ == "__main__":
    main()

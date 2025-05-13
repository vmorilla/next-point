#!/usr/bin/env python3

import json
import re

# Tool to produce the colors.h file from the GIMP palette with NEXT 8 bit default palette


def parse_gimp_palette(file_path):
    """Parse a GIMP .gpl file and extract RGB values."""
    rgb_values = []
    with open(file_path, "r") as file:
        lines = file.readlines()
        # Skip header lines until we hit the color data
        for line in lines:
            # Match lines with RGB values (e.g., "255   0   0 Red" or "255   0   0")
            match = re.match(r"^\s*(\d+)\s+(\d+)\s+(\d+)", line)
            if match:
                r, g, b = map(int, match.groups())
                rgb_values.append((r, g, b))
    return rgb_values


def parse_json_css_colors(file_path):
    """Parse a JSON file containing CSS colors. The format of the file is "color": "#RRGGBB"."""
    with open(file_path, "r") as file:
        data = json.load(file)
        colors = {}
        for color_name, hex_value in data.items():
            # Convert hex to RGB
            r = int(hex_value[1:3], 16)
            g = int(hex_value[3:5], 16)
            b = int(hex_value[5:7], 16)
            colors[color_name] = (r, g, b)
    return colors


def find_closest_colors(next_palette, css_colors):
    """Find the closest color correspondences between the GIMP palette and CSS colors. Returns a dictionary of color names and gimp palette indexes.
    Neither the color name or the index are duplicated (only the best match is chosen).
    """

    def color_distance(color1, color2):
        """Calculate Euclidean distance between two RGB colors."""
        r1, g1, b1 = color1
        r2, g2, b2 = color2
        return ((r1 - r2) ** 2 + (g1 - g2) ** 2 + (b1 - b2) ** 2) ** 0.5

    # Create a list of all possible matches with their distances
    all_matches = []
    for idx, next_color in enumerate(next_palette):
        for css_name, css_color in css_colors.items():
            distance = color_distance(next_color, css_color)
            all_matches.append((distance, idx, css_name))

    # Sort by distance (closest first)
    all_matches.sort()

    # Assign matches, ensuring no duplicates for either palette index or color name
    used_indices = set()
    used_names = set()
    result = {}

    for distance, idx, css_name in all_matches:
        if idx not in used_indices and css_name not in used_names:
            result[css_name] = idx
            used_indices.add(idx)
            used_names.add(css_name)

            # If all palette colors have been assigned, we're done
            if len(used_indices) == len(next_palette):
                break

    return result


def main():
    # Replace with your GIMP palette file path
    gimp_file = "/Applications/Aseprite.app/Contents/Resources/data/extensions/arne-palettes/next-8bits.gpl"

    # Open the css color names (css_colors.json) in the current folder
    css_colors_file = "css_colors.json"
    css_colors = parse_json_css_colors(css_colors_file)
    next_palette = parse_gimp_palette(gimp_file)

    result = find_closest_colors(next_palette, css_colors)

    # Sort the result by index
    result = dict(sorted(result.items(), key=lambda item: item[1]))

    for css_name, index in result.items():
        print(f"#define {css_name.upper()} {index}\t // {next_palette[index]}")


if __name__ == "__main__":
    main()

#!/usr/bin/env python3
"""
Draws a tennis court with perspective using the PIL library.
"""
from typing import List, NewType, Tuple, Union
from PIL import Image, ImageDraw, ImageEnhance
from next import img_to_ly2

# Distance of the vanishing point to...
VANISHING_POINT = 2512 / 5

# Full area of the image
WIDTH = 320
HEIGHT = 256
# WIDTH = 1600
# HEIGHT = 1280

# Visible area
VISIBLE_WIDTH = 256
VISIBLE_HEIGHT = 192

# Double Tennis court dimensions
DT_LOW_WIDTH = 241
DT_TOP_WIDTH = 171
DT_HEIGHT = 128
DT_NET = 16

Y_OFFSET = 30

# Resize factor to apply to double tennis court dimensions
DT_PROP = 1200 / 5 / 241

# Define type for coordinates
Coord = NewType("Coord", Tuple[float, float])
CoordHeight = NewType("CoordHeight", Tuple[float, float, float])
CoordH = NewType("CoordH", Coord | CoordHeight)
Coords = NewType("Coords", List[CoordH])


# This function is simpler...
def perspective(point: CoordH) -> Coord:
    if len(point) == 2:
        (x, y) = point
        return _perspective(x, y, 0)
    else:
        (x, y, h) = point
        return _perspective(x, y, h)


def _perspective(x: float, y: float, h: float) -> Coord:
    dx = x - WIDTH / 2
    dy = HEIGHT - y
    px = dx * dy / VANISHING_POINT
    dh = h * dy / VANISHING_POINT

    return (x - px, y - (h - dh))


def persp(points: Coords) -> Coords:
    if isinstance(points, tuple):
        return perspective(points)
    else:
        return [perspective(p) for p in points]


def rect_points(left: float, top: float, width: float, height: float) -> List[Coord]:
    return [
        (left, top),
        (left + width, top),
        (left + width, top + height),
        (left, top + height),
    ]


def offset_points(points: Coords, offset: Coord) -> Coords:
    if isinstance(points, tuple):
        return (points[0] + offset[0], points[1] + offset[1] + Y_OFFSET)
    else:
        return [(p[0] + offset[0], p[1] + offset[1] + Y_OFFSET) for p in points]


def center(points: Coords) -> Coords:
    width = max([p[0] for p in points]) - min([p[0] for p in points])
    height = max([p[1] for p in points]) - min([p[1] for p in points])
    offset = ((WIDTH - width) / 2, (HEIGHT - height) / 2)
    return offset_points(points, offset)


def resize(image: Image, width: int, height: int) -> Image:

    image = image.resize([round(image.width * height / image.height), height])

    left = (image.width - width) / 2 + 25
    return image.crop([left, 0, left + width, image.height])


def draw_court() -> Image:
    image = Image.new("RGB", (WIDTH, HEIGHT), color="black")
    # image = Image.open(refImage)
    # brighter = ImageEnhance.Brightness(image)
    # image = brighter.enhance(0.5)

    image = resize(image, WIDTH, HEIGHT)

    draw = ImageDraw.Draw(image)
    draw.rectangle([0, 0, WIDTH, HEIGHT], fill="black")

    # Ground area
    GROUND_WIDTH = WIDTH + 200 / 5
    GROUND_TOP = 200 / 5 + Y_OFFSET
    GROUND_LEFT = (WIDTH - GROUND_WIDTH) / 2
    GROUND_RIGHT = (WIDTH + GROUND_WIDTH) / 2
    GROUND_BOTTOM = HEIGHT + Y_OFFSET

    draw.polygon(
        persp(rect_points(GROUND_LEFT, GROUND_TOP, GROUND_WIDTH, HEIGHT - GROUND_TOP)),
        fill="MediumBlue",
    )

    # Walls
    WALL_HEIGHT = 100 / 5
    WALL_INCL = 10
    draw.polygon(
        persp(
            [
                (GROUND_LEFT, GROUND_TOP, WALL_HEIGHT),
                (GROUND_RIGHT, GROUND_TOP, WALL_HEIGHT),
                (GROUND_RIGHT, GROUND_TOP, 0),
                (GROUND_LEFT, GROUND_TOP, 0),
            ]
        ),
        fill="gray",
        outline="black",
    )

    draw.polygon(
        persp(
            [
                (GROUND_LEFT - WALL_INCL, GROUND_BOTTOM, WALL_HEIGHT),
                (GROUND_LEFT, GROUND_BOTTOM, 0),
                (GROUND_LEFT, GROUND_TOP, 0),
                (GROUND_LEFT - WALL_INCL, GROUND_TOP, WALL_HEIGHT),
            ]
        ),
        fill="gray",
        outline="black",
    )

    draw.polygon(
        persp(
            [
                (GROUND_RIGHT + WALL_INCL, GROUND_BOTTOM, WALL_HEIGHT),
                (GROUND_RIGHT, GROUND_BOTTOM, 0),
                (GROUND_RIGHT, GROUND_TOP, 0),
                (GROUND_RIGHT + WALL_INCL, GROUND_TOP, WALL_HEIGHT),
            ]
        ),
        fill="gray",
        outline="black",
    )

    # Draws double tennis proportional play area
    PLAYAREA_WIDTH = 1000 / 5  # round(DT_PROP * DT_LOW_WIDTH)
    PLAYAREA_HEIGHT = 600 / 5  # round(DT_PROP * DT_HEIGHT)
    NET_HEIGHT = round(DT_PROP * DT_NET)

    # Draws doubles play area
    draw.polygon(
        persp(center(rect_points(0, 0, PLAYAREA_WIDTH * 36 / 27, PLAYAREA_HEIGHT))),
        width=2,
        outline="white",
        fill="DarkBlue",
    )

    # Draws singles play area
    draw.polygon(
        persp(center(rect_points(0, 0, PLAYAREA_WIDTH, PLAYAREA_HEIGHT))),
        width=2,
        outline="white",
    )

    hor_divider = [
        ((WIDTH - PLAYAREA_WIDTH) / 2, HEIGHT / 2),
        ((WIDTH + PLAYAREA_WIDTH) / 2, HEIGHT / 2),
    ]
    # Middle lines
    draw.line(
        persp(offset_points(hor_divider, (0, PLAYAREA_HEIGHT / 4))),
        width=2,
        fill="white",
    )
    draw.line(
        persp(offset_points(hor_divider, (0, -PLAYAREA_HEIGHT / 4))),
        width=2,
        fill="white",
    )

    vert_margin = (HEIGHT - PLAYAREA_HEIGHT) / 2

    draw.line(
        [
            (WIDTH / 2, vert_margin + PLAYAREA_HEIGHT / 4 + Y_OFFSET),
            (WIDTH / 2, vert_margin + PLAYAREA_HEIGHT * 3 / 4 + Y_OFFSET),
        ],
        width=2,
        fill="white",
    )

    # Service separation lines
    draw.line(
        [(WIDTH / 2, vert_margin + Y_OFFSET), (WIDTH / 2, vert_margin + 4 + Y_OFFSET)],
        width=2,
        fill="white",
    )
    draw.line(
        [
            (WIDTH / 2, HEIGHT - vert_margin + Y_OFFSET),
            (WIDTH / 2, HEIGHT - vert_margin - 4 + Y_OFFSET),
        ],
        width=2,
        fill="white",
    )

    # Draw visible area
    # draw.rectangle(
    #     [
    #         (WIDTH - VISIBLE_WIDTH) / 2,
    #         (HEIGHT - VISIBLE_HEIGHT) / 2,
    #         (WIDTH + VISIBLE_WIDTH) / 2,
    #         (HEIGHT + VISIBLE_HEIGHT) / 2,
    #     ],
    #     width=2,
    #     outline="cyan",
    # )

    return image


def rgb8(r: int, g: int, b: int) -> int:
    value = ((r >> 5) << 5) | ((g >> 5) << 2) | (b >> 6)
    print(f"R{r} G{g} B{b} => {value}")
    return value


def to_tile(image: Image):
    pixels = image.load()
    patterns: List[Tuple[int]] = []

    def block_pattern(block_x: int, block_y: int):
        return tuple(
            [
                16 * pixels[x + block_x, y + block_y]
                + pixels[x + 1 + block_x, y + block_y]
                for y in range(8)
                for x in range(0, 8, 2)
            ]
        )

    def pattern_index(block_pattern: Tuple[int]) -> int:
        try:
            return patterns.index(block_pattern)
        except ValueError:
            patterns.append(block_pattern)
            return len(patterns) - 1

    palette = image.getpalette()
    grb_palette = [
        rgb8(r, g, b) for r, g, b in zip(palette[0::3], palette[1::3], palette[2::3])
    ]

    tile_map = [
        pattern_index(block_pattern(blockX, blockY))
        for blockY in range(0, image.height, 8)
        for blockX in range(0, image.width, 8)
    ]

    flat_patterns = [p for pattern in patterns for p in pattern]

    return tile_map, flat_patterns, grb_palette


def reduce_palette(image: Image, colors: int) -> Image:
    # Reduce the number of bits to represent each color component to 3
    image = image.convert("RGB")
    for x in range(image.width):
        for y in range(image.height):
            r, g, b = image.getpixel((x, y))
            image.putpixel((x, y), (r & 0b11100000, g & 0b11100000, b & 0b11000000))

    image = image.convert("P", palette=Image.ADAPTIVE, colors=colors)
    return image


if __name__ == "__main__":
    court = draw_court()
    # court.show()
    court.save("graphics/court-base.png")
    assert court.size == (WIDTH, HEIGHT)
    # with open("assets/court.ly2", "wb") as f:
    #     img_to_ly2(court, f)

    # print(f"${_perspective(50, 256, 0)} - {_persp_point([50, 256])}")
    # print(f"${_perspective(50, 106, 0)} - {_persp_point([50, 106])}")
    # court = court.resize((640, 256))
    # court = reduce_palette(court, 256)

    # tiles, patterns, palette = to_tile(court)
    # with open("assets/melbourne.tiles", "wb") as f:
    #     f.write(bytearray(tiles))

    # with open("assets/melbourne.patterns", "wb") as f:
    #     f.write(bytearray(patterns))

    # with open("assets/melbourne.palette", "wb") as f:
    #     f.write(bytearray(palette))

    # court.show()

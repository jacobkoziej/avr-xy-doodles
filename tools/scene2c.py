#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
#
# scene2c.py -- generate c code from scenes
# Copyright (C) 2025  Jacob Koziej <jacobkoziej@gmail.com>

import sys

import yaml

from argparse import ArgumentParser
from dataclasses import (
    dataclass,
    field,
    replace,
)
from pathlib import Path
from typing import Final


@dataclass(kw_only=True)
class Shape:
    scale: int | float = 1
    x_off: int | float = 0
    y_off: int | float = 0


@dataclass(kw_only=True)
class ShapeArc(Shape):
    cx: int | float = 0
    cy: int | float = 0
    rx: int | float = 0
    ry: int | float = 0
    t0: int | float = 0
    t1: int | float = 0

    def __str__(self) -> str:
        return "\n".join(
            [
                f"static PROGMEM const shape_arc_t shape_{id(self)} = {{",
                "\t.shape = SHAPE_ARC,",
                f"\t.cx    = FP_STATIC(u, {self.cx} + {self.x_off}),",
                f"\t.cy    = FP_STATIC(u, {self.cy} + {self.y_off}),",
                f"\t.rx    = FP_STATIC(u, {self.rx} * {self.scale}),",
                f"\t.ry    = FP_STATIC(u, {self.ry} * {self.scale}),",
                f"\t.t0    = FP_STATIC(i, {self.t0}),",
                f"\t.t1    = FP_STATIC(i, {self.t1}),",
                "};",
            ]
        )


@dataclass(kw_only=True)
class ShapeRect(Shape):
    x: int | float = 0
    y: int | float = 0
    width: int | float = 0
    height: int | float = 0
    rx: int | float = 0
    ry: int | float = 0

    def __str__(self) -> str:
        return "\n".join(
            [
                f"static PROGMEM const shape_rect_t shape_{id(self)} = {{",
                "\t.shape  = SHAPE_RECT,",
                f"\t.x      = FP_STATIC(u, {self.x} + {self.x_off}),",
                f"\t.y      = FP_STATIC(u, {self.y} + {self.y_off}),",
                f"\t.width  = FP_STATIC(u, {self.width} * {self.scale}),",
                f"\t.height = FP_STATIC(u, {self.height} * {self.scale}),",
                f"\t.rx     = FP_STATIC(u, {self.rx} * {self.scale}),",
                f"\t.ry     = FP_STATIC(u, {self.ry} * {self.scale}),",
                "};",
            ]
        )


@dataclass(kw_only=True)
class ShapeCircle(Shape):
    cx: int | float = 0
    cy: int | float = 0
    r: int | float = 0

    def __str__(self) -> str:
        return "\n".join(
            [
                f"static PROGMEM const shape_circle_t shape_{id(self)} = {{",
                "\t.shape = SHAPE_CIRCLE,",
                f"\t.cx    = FP_STATIC(u, {self.cx} + {self.x_off}),",
                f"\t.cy    = FP_STATIC(u, {self.cy} + {self.y_off}),",
                f"\t.r     = FP_STATIC(u, {self.r} * {self.scale}),",
                "};",
            ]
        )


@dataclass(kw_only=True)
class ShapeEllipse(Shape):
    cx: int | float = 0
    cy: int | float = 0
    rx: int | float = 0
    ry: int | float = 0

    def __str__(self) -> str:
        return "\n".join(
            [
                f"static PROGMEM const shape_ellipse_t shape_{id(self)} = {{",
                "\t.shape = SHAPE_ELLIPSE,",
                f"\t.cx    = FP_STATIC(u, {self.cx} + {self.x_off}),",
                f"\t.cy    = FP_STATIC(u, {self.cy} + {self.y_off}),",
                f"\t.rx    = FP_STATIC(u, {self.rx} * {self.scale}),",
                f"\t.ry    = FP_STATIC(u, {self.ry} * {self.scale}),",
                "};",
            ]
        )


@dataclass(kw_only=True)
class ShapeLine(Shape):
    x1: int | float = 0
    y1: int | float = 0
    x2: int | float = 0
    y2: int | float = 0

    def __str__(self) -> str:
        return "\n".join(
            [
                f"static PROGMEM const shape_line_t shape_{id(self)} = {{",
                "\t.shape = SHAPE_LINE,",
                f"\t.x1    = FP_STATIC(u, (({self.x1} + {self.x_off}) * {self.scale})),",
                f"\t.y1    = FP_STATIC(u, (({self.y1} + {self.y_off}) * {self.scale})),",
                f"\t.x2    = FP_STATIC(u, (({self.x2} + {self.x_off}) * {self.scale})),",
                f"\t.y2    = FP_STATIC(u, (({self.y2} + {self.y_off}) * {self.scale})),",
                "};",
            ]
        )


@dataclass(kw_only=True)
class ShapePoly(Shape):
    polygon: bool = False
    points: list[tuple[int | float, int | float]] = field(default_factory=list)

    def __post_init__(self) -> None:
        assert len(self.points) > 0

    def __str__(self) -> str:
        base_x = self.points[0][0]
        base_y = self.points[0][1]

        points = "\n".join(
            [
                f"\t{{\n\t\tFP_STATIC(u, (({x} - {base_x}) * {self.scale}) + {self.x_off} + {base_x}),\n\t\tFP_STATIC(u, (({y} - {base_y}) * {self.scale}) + {self.y_off} + {base_y}),\n\t}},"
                for (x, y) in self.points
            ]
        )

        return "\n".join(
            [
                f"static PROGMEM const shape_point_t shape_points_{id(self)}[] = {{\n{points}\n}};\n",
                f"static PROGMEM const shape_poly_t shape_{id(self)} = {{",
                "\t.shape   = SHAPE_POLY,",
                f"\t.polygon = {'true' if self.polygon else 'false'},",
                f"\t.size    = {len(self.points)},",
                f"\t.points  = shape_points_{id(self)},",
                "};",
            ]
        )


@dataclass(kw_only=True)
class Frame:
    duration: float
    doodles: list[Shape]


_NAME_TO_SHAPE: Final[dict[str, Shape]] = {
    "arc": ShapeArc,
    "rect": ShapeRect,
    "circle": ShapeCircle,
    "ellipse": ShapeEllipse,
    "line": ShapeLine,
    "poly": ShapePoly,
}


def generate_c(frames: list[Frame]) -> str:
    c = "\n".join(
        [
            "#include <avr/pgmspace.h>\n",
            "#include <avr-xy-doodles/doodle.h>",
            "#include <avr-xy-doodles/fixed-point.h>",
            "#include <avr-xy-doodles/shape.h>\n",
            "#include <stddef.h>",
            "\n",
        ]
    )

    shapes = []
    for frame in frames:
        shapes += [str(doodle) for doodle in frame.doodles]

    c += "\n\n".join(shapes)

    for frame in frames:
        c += f"\n\nstatic PROGMEM const shape_t * const doodle_shapes_{id(frame)}[] = {{\n"
        c += "\n".join(
            [f"\t&shape_{id(doodle)}.shape," for doodle in frame.doodles]
        )
        c += f"\n}};\n\nstatic PROGMEM const doodle_t doodle_{id(frame)} = {{"
        c += f"\n\t.shapes = doodle_shapes_{id(frame)},"
        c += f"\n\t.size     = {len(frame.doodles)},"
        c += f"\n\t.duration = {int(frame.duration * 1000)},\n}};"

    c += "\n\nPROGMEM const doodle_t * const doodles[] = {\n"
    c += "\n".join([f"\t&doodle_{id(frame)}," for frame in frames])
    c += "\n\tNULL,\n};"

    return c


def parse_doodle(doodle_path: Path, name: str) -> list[Shape]:
    path = doodle_path / f"{name}.yaml"

    with path.open() as f:
        doodle = yaml.safe_load(f)

    shapes = []
    for shape in doodle["shapes"]:
        for name, val in shape.items():
            shape = _NAME_TO_SHAPE[name]

            shapes.append(shape(**val))

    return shapes


def parse_scene(scene: dict, doodle_path: Path) -> list[Frame]:
    doodle_lookup = {}

    frames = []
    for frame in scene["frames"]:
        doodles = []

        for name, options in frame["doodles"].items():
            shapes = doodle_lookup.get(name)

            if shapes is None:
                doodle_lookup[name] = shapes = parse_doodle(doodle_path, name)

            doodles += [replace(shape, **options) for shape in shapes]

        frames += [Frame(duration=frame["duration"], doodles=doodles)]

    return frames


def main():
    parser = ArgumentParser(
        description="generate c code from scenes",
    )

    parser.add_argument(
        "-d",
        "--doodles",
        default=Path("doodles"),
        type=Path,
    )
    parser.add_argument(
        "-i",
        "--input",
        required=True,
        type=Path,
    )
    parser.add_argument(
        "-o",
        "--output",
        default=Path("scene.c"),
        type=Path,
    )

    args = parser.parse_args()

    with args.input.open() as f:
        scene = yaml.safe_load(f)

    frames = parse_scene(scene, args.doodles)

    args.output.write_text(generate_c(frames))


if __name__ == "__main__":
    sys.exit(main())

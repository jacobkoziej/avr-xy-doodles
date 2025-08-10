#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
#
# sinpi-lookup.py -- generate sin(x * pi) lookup table
# Copyright (C) 2025  Jacob Koziej <jacobkoziej@gmail.com>

import sys

from argparse import ArgumentParser
from math import (
    sin,
    pi,
)
from pathlib import Path


def generate_c(values: list[int], type: str, variable: str) -> str:
    values = [f"\t0x{value:02X}," for value in values]

    c = (
        f"#include <stdint.h>\n\nconst {type} {variable}[] = {{\n"
        + "\n".join(values)
        + "\n};"
    )

    return c


def main():
    parser = ArgumentParser(
        description="generate sin(x * pi) lookup table",
    )

    parser.add_argument(
        "-f",
        "--fractional-bits",
        default=8,
        type=int,
    )
    parser.add_argument(
        "-t",
        "--type",
        default="uint8_t",
        type=str,
    )
    parser.add_argument(
        "-o",
        "--output",
        default=Path("sinpi-lookup.c"),
        type=Path,
    )
    parser.add_argument(
        "-v",
        "--variable",
        default="sinpi_lookup_table",
        type=str,
    )

    args = parser.parse_args()

    steps = 1 << args.fractional_bits

    step = 1 / steps

    values = [
        min(round(sin(x * pi * step) * steps), steps - 1) for x in range(steps)
    ]

    args.output.write_text(generate_c(values, args.type, args.variable))


if __name__ == "__main__":
    sys.exit(main())

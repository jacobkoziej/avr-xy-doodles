# SPDX-License-Identifier: GPL-3.0-or-later

sinpi_lookup_table = custom_target(
  'sinpi-lookup-table',
  output: 'sinpi-lookup-table.c',
  command: [
    join_paths(tools, 'sinpi-lookup.py'),
    '--output',
    '@OUTPUT@',
  ],
)

avr_xy_doodles = executable(
  'avr-xy-doodles.elf',
  include_directories : [
    avr_xy_doodles_include,
  ],
  sources : [
    sinpi_lookup_table,
    'display.c',
    'fixed-point.c',
    'fuses.c',
    'main.c',
  ],
)

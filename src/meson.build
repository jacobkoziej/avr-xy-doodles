# SPDX-License-Identifier: GPL-3.0-or-later

scene = custom_target(
  'scene',
  output: 'scene.c',
  command: [
    join_paths(tools, 'scene2c.py'),
    '--doodles',
    doodles,
    '--input',
    join_paths(scenes, get_option('scene') + '.yaml'),
    '--output',
    '@OUTPUT@',
  ],
)

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
    scene,
    sinpi_lookup_table,
    'display.c',
    'fixed-point.c',
    'fuses.c',
    'main.c',
    'timer.c',
  ],
)

project(
  'avr-xy-doodles',
  'c',
  version : '0.1.0',
  license : 'GPL-3.0-or-later',
  license_files : [
    'LICENSE.md',
  ],
  default_options : [
    'c_std=c23',
    'warning_level=everything',
  ],
  meson_version : '>=1.8.2',
)

add_global_arguments(
  '-fdata-sections',
  '-ffunction-sections',
  '-DF_CPU=' + get_option('f_cpu').to_string() + 'ULL',
  '-Wno-alloca',
  '-Wno-c++-compat',
  '-Wno-vla',
  language : 'c',
)

avr_xy_doodles_include = include_directories('include')

doodles = join_paths(meson.project_source_root(), 'doodles')
scenes = join_paths(meson.project_source_root(), 'scenes')
tools = join_paths(meson.project_source_root(), 'tools')

subdir('src')

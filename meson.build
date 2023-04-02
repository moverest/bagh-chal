project(
  'bagh-chal',
  'c',
  license: 'MIT',
  default_options: ['c_std=gnu99'],
)

cc = meson.get_compiler('c')

sdl2 = dependency('sdl2')
sdl2_ttf = dependency('sdl2_ttf')
termbox = cc.find_library('termbox')
math = cc.find_library('m')

common_src = [
    'src/ui_game.c',
    'src/ui_game_menu.c',
    'src/game.c',
    'src/models.c',
    'src/ai_rand.c',
    'src/menu.c',
    'src/ui_menu.c',
    'src/ui_main.c',
    'src/ui_end_menu.c',
    'src/ui_pause_menu.c',
    'src/stack.c',
    'src/ai_heuristic.c',
    'src/ai_simple_heuristic.c',
]

executable(
  'bagh-chal-sdl',
  [
    'src/main_minimalist_sdl.c',
    'src/graphics_minimalist_sdl.c',
  ] + common_src,
  dependencies: [sdl2, sdl2_ttf],
  install: true,
)

executable(
  'bagh-chal-tb',
  [
    'src/main_tb.c',
    'src/graphics_tb.c',
  ] + common_src,
  dependencies: [termbox],
  install: true,
)


test(
  'test_test',
  executable(
    'test_test',
    [
      'src/test_test.c',
      'src/test.c',
    ]
  ),
)

test(
  'test_game',
  executable(
    'test_game',
    [
      'src/test_game.c',
      'src/models.c',
      'src/test.c',
      'src/game.c',
      'src/ai_rand.c',
      'src/stack.c',
    ]
  ),
)

test(
  'test_matrix',
  executable(
    'test_matrix',
    [
      'src/test_matrix.c',
      'src/test.c',
      'src/matrix.c',
    ],
    dependencies: [math],
  ),
)

test(
  'test_stack',
  executable(
    'test_stack',
    [
      'src/test_stack.c',
      'src/test.c',
      'src/stack.c',
    ]
  ),
)

test(
  'test_neuralnet',
  executable(
    'test_neuralnet',
    [
      'src/test_neuralnet.c',
      'src/test.c',
      'src/neuralnet.c',
      'src/matrix.c',
      'src/randn.c',
    ],
    dependencies: [math],
  ),
)

executable(
  'test_graphics_tb',
  [
    'src/test_graphics_tb.c',
    'src/models.c',
    'src/graphics_tb.c',
    'src/graphics_test.c',
  ],
  dependencies: [termbox],
)

executable(
  'test_graphics_minimalist_sdl',
  [
    'src/test_graphics_minimalist_sdl.c',
    'src/models.c',
    'src/graphics_minimalist_sdl.c',
    'src/graphics_test.c',
  ],
  dependencies: [sdl2, sdl2_ttf],
)

executable(
  'test_menu_tb',
  [
    'src/test_menu_tb.c',
    'src/models.c',
    'src/menu.c',
    'src/ui_menu.c',
    'src/graphics_tb.c',
    'src/menu_test.c',
  ],
  dependencies: [termbox],
)

executable(
  'test_menu_graphics_sdl',
  [
    'src/test_menu_graphics_sdl.c',
    'src/models.c',
    'src/menu.c',
    'src/ui_menu.c',
    'src/graphics_minimalist_sdl.c',
    'src/menu_test.c',
  ],
  dependencies: [sdl2, sdl2_ttf],
)

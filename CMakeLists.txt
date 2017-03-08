cmake_minimum_required(VERSION 2.8)
project(Bagh_chal C)

add_library(termbox STATIC IMPORTED)
set_property(TARGET termbox PROPERTY
  IMPORTED_LOCATION /usr/lib/libtermbox.a)

add_executable(term_bagh_chal
  src/term_main.c
  src/termbox_graphics.c
)

target_link_libraries(term_bagh_chal termbox)

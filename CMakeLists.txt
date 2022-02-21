cmake_minimum_required(VERSION 3.5)
project(textolitaire)

set(HEADER_FILES_COMMON inc/card.h
                        inc/solstack.h
                        inc/controls.h
                        inc/table.h
                        inc/diskdata.h
                        inc/game.h
                        inc/input.h
                        inc/display.h)

set(SOURCE_FILES_COMMON src/card.c
                        src/solstack.c
                        src/controls.c
                        src/table.c
                        src/game.c
                        src/diskdata.c
                        src/main.c)

include_directories(inc)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

# Textolitaire 1
set(SOURCE_FILES1 ${HEADER_FILES_COMMON}
                  ${SOURCE_FILES_COMMON}
                  src/input-termios.c
                  src/display-term.c)

add_executable(textolitaire1 ${SOURCE_FILES1})

add_custom_target(run1 COMMAND textolitaire1)

# Textolitaire 2
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})
find_library(CURSES_NCURSESW_LIBRARY "libncursesw.so")
if(NOT CURSES_NCURSESW_LIBRARY)
  find_library(CURSES_NCURSESW_LIBRARY "libncurses.so")
endif(NOT CURSES_NCURSESW_LIBRARY)
find_library(CURSES_TINFOW_LIBRARY "libtinfow.so")
if(NOT CURSES_TINFOW_LIBRARY)
  find_library(CURSES_TINFOW_LIBRARY "libtinfo.so")
endif(NOT CURSES_TINFOW_LIBRARY)

set(SOURCE_FILES2 ${HEADER_FILES_COMMON}
                  ${SOURCE_FILES_COMMON}
                  src/input-curses.c
                  src/display-curses.c)

add_executable(textolitaire2 ${SOURCE_FILES2})

target_link_libraries(textolitaire2 ${CURSES_NCURSESW_LIBRARY} ${CURSES_TINFOW_LIBRARY})

add_custom_target(run2 COMMAND textolitaire2)
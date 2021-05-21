cmake_minimum_required(VERSION 3.5)
project(textolitaire)

# It's recommended to use a separate build directory to avoid rewriting
# the GNU Makefile. For example:
#
# $ mkdir -p build; cd build && cmake .. && make


set(SOURCE_FILES_COMMON src/getche.c
                        src/card.c
                        src/solstack.c
                        src/controls.c
                        src/table.c
                        src/sol.c
                        src/diskdata.c)

include_directories(inc)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(SOURCE_FILES1 ${SOURCE_FILES_COMMON} src/display.c)

add_executable(textolitaire1 ${SOURCE_FILES1})

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

set(SOURCE_FILES2 ${SOURCE_FILES_COMMON} src/display-curses.c)

add_executable(textolitaire2 ${SOURCE_FILES2})

target_link_libraries(textolitaire2 ${CURSES_NCURSESW_LIBRARY} ${CURSES_TINFOW_LIBRARY})

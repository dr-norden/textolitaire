cmake_minimum_required(VERSION 3.7)
project(textolitaire)

# It's recommended to use a separate build directory to avoid rewriting
# the GNU Makefile. For example:
# 
# $ mkdir -p build; cd build && cmake .. && make

set(SOURCE_FILES1 getche.c card.c solstack.c controls.c display.c table.c sol.c diskdata.c)
add_executable(textolitaire1 ${SOURCE_FILES1})
 
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})
find_library(CURSES_NCURSESW_LIBRARY "libncursesw.so")
if(NOT CURSES_NCURSESW_LIBRARY)
  find_library(CURSES_NCURSESW_LIBRARY "libncurses.so")
endif(NOT CURSES_NCURSESW_LIBRARY)
message(STATUS "*** Curses: " ${CURSES_NCURSESW_LIBRARY})
find_library(CURSES_TINFOW_LIBRARY "libtinfow.so")
if(NOT CURSES_TINFOW_LIBRARY)
  find_library(CURSES_TINFOW_LIBRARY "libtinfo.so")
endif(NOT CURSES_TINFOW_LIBRARY)
message(STATUS "*** Curses: " ${CURSES_TINFOW_LIBRARY})

set(SOURCE_FILES2 getche.c card.c solstack.c controls.c display-curses.c table.c sol.c diskdata.c)
add_executable(textolitaire2 ${SOURCE_FILES2})
 
target_link_libraries(textolitaire2 ${CURSES_NCURSESW_LIBRARY} ${CURSES_TINFOW_LIBRARY})

# Copyright (C) Martín E. Zahnd
# This file is part of Pinder.
#
# Pinder is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Pinder is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Pinder.  If not, see <https://www.gnu.org/licenses/>.


# Some help from:
# https://github.com/tony/NCURSES-Programming-HOWTO-examples/blob/master/CMakeLists.txt

cmake_minimum_required(VERSION 3.13.0)

# Libraries

# ncurses
set(CURSES_USE_NCURSES true)
find_package(
    Curses REQUIRED
)
include_directories(${NCURSES_INCLUDE_DIR})

add_compile_options(
    -Wall
    -pedantic
    -DALLOW_DIAGONALS # Change allowing-disallowing using diagonals in the path
)

add_executable(${PROJECT_NAME}
    main.cpp
    graph/Board.cpp
    tui/Board.cpp
    tui/Tui.cpp
    tui/Menu.cpp
    tui/Window.cpp
)

target_link_libraries(${PROJECT_NAME}
    # For ncurses
    ncursesw
    panel
    menu
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include/algorithms
        ${PROJECT_SOURCE_DIR}/include/graph
        ${PROJECT_SOURCE_DIR}/include/tui
        ${NCURSES_LIBRARY}
        ${ADDITIONAL_LIBRARIES}
)
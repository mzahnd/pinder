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


cmake_minimum_required(VERSION 3.13.0)
project(pinder VERSION 0.1.0 LANGUAGES CXX)

# Release type
# 
# Available:
#   .- Debug
#   .- Release
#   .- RelWithDebInfo
#   .- MinSizeRel

set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE Release)


# C++ Standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

add_subdirectory(
    ${PROJECT_SOURCE_DIR}/src
)

# ------- What's this? VS Code added -------
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
# ------------------------------


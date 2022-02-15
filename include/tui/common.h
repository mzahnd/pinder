/* Copyright (C) Martín E. Zahnd
 * This file is part of Pinder.
 *
 * Pinder is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Pinder is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Pinder.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#ifndef TUICOMMON_H
#define TUICOMMON_H 1

#include "../graph/Board.h"       /* graph::Location */

namespace tui {

    #define BOARD_ROWS 16
    #define BOARD_COLS BOARD_ROWS

    #define BOARD_ROWS_MIN 5
    #define BOARD_COLS_MIN BOARD_ROWS_MIN

    #define CURSOR_X_OFFSET 3
    #define CURSOR_Y_OFFSET 1

    #define MENU_WIDTH  30
    #define MENU_HEIGHT 10

    #define BORDER_PADDING  1

    #define WINDOW_SEPARATION 2


    // Colors

    // Convert RGB value (0-255) to NCURSES_COLOR_T (0 to 1000)
    #define CHAR_RGB_TO_COLOR_T(color)  ((NCURSES_COLOR_T) ((color)*1000/255))

    // RGB color tuple with conversion to NCURSES_COLOR_T
    #define COLOR_T_TUPLE(r, g, b)      CHAR_RGB_TO_COLOR_T(r), \
                                        CHAR_RGB_TO_COLOR_T(g), \
                                        CHAR_RGB_TO_COLOR_T(b)


    #define TUI_COLOR_BACKGROUND            COLOR_BLACK
    #define TUI_COLOR_FOREGROUND            COLOR_WHITE

    #define TUI_COLOR_WINDOW_ACTIVE         COLOR_WHITE
    #define TUI_COLOR_WINDOW_INACTIVE       COLOR_GREY

    #define TUI_COLOR_MENU_CURRENT          COLOR_RED
    #define TUI_COLOR_MENU_DISABLED         COLOR_GREY

    #define TUI_COLOR_START_GOAL            COLOR_GREEN
    #define TUI_COLOR_WALL                  COLOR_RED
    #define TUI_COLOR_WEIGHT                COLOR_YELLOW
    #define TUI_COLOR_PATH                  COLOR_CYAN

    #define TUI_COLOR_ARROWS_LEFT           COLOR_ORANGE
    #define TUI_COLOR_ARROWS_UP             COLOR_LIGHT_BLUE
    #define TUI_COLOR_ARROWS_RIGHT          COLOR_VIOLET
    #define TUI_COLOR_ARROWS_DOWN           COLOR_REDISH_GREEN


    // // These are standard 16 bit colors, not defined in ncurses 
    // (as far as I know)
    // enum HighIntensityColors {
    //     COLOR_HIGH_BLACK = 8,
    //     COLOR_HIGH_RED,
    //     COLOR_HIGH_GREEN,
    //     COLOR_HIGH_YELLOW,
    //     COLOR_HIGH_BLUE,
    //     COLOR_HIGH_MAGENTA,
    //     COLOR_HIGH_CYAN,
    //     COLOR_HIGH_WHITE
    // };


    // Custom colors
    enum CustomColors {
        COLOR_GREY = 16,                 // 15 is the highest index (16 colors)

        COLOR_ORANGE,                   // Tetradic colors
        COLOR_REDISH_GREEN,
        COLOR_LIGHT_BLUE,
        COLOR_VIOLET,

        // Not initialized directely (by name) but through a loop. 
        // Defined to avoid using their index for another color
        COLOR_COST_0, COLOR_COST_1, COLOR_COST_2, COLOR_COST_3, COLOR_COST_4,
        COLOR_COST_5, COLOR_COST_6, COLOR_COST_7, COLOR_COST_8, COLOR_COST_9,
    };

    #define RGB_COLOR_RED                   COLOR_T_TUPLE(255, 0, 0)
    #define RGB_COLOR_GREEN                 COLOR_T_TUPLE(0, 255, 0)
    #define RGB_COLOR_YELLOW                COLOR_T_TUPLE(255, 255, 0)
    #define RGB_COLOR_BLUE                  COLOR_T_TUPLE(0, 0, 255)
    #define RGB_COLOR_WHITE                 COLOR_T_TUPLE(255, 255, 255)

    #define RGB_COLOR_GREY                  COLOR_T_TUPLE(75, 75, 75)

    #define RGB_COLOR_ORANGE                COLOR_T_TUPLE(255, 95, 0)
    #define RGB_COLOR_REDISH_GREEN          COLOR_T_TUPLE(33, 255, 0)
    #define RGB_COLOR_LIGHT_BLUE            COLOR_T_TUPLE(0, 160, 255)
    #define RGB_COLOR_VIOLET                COLOR_T_TUPLE(223, 0, 255)

    // Color pairs
    enum ColorPairs {
        ColorPairs_DEFAULT = 1,

        ColorPairs_WINDOW_ACTIVE, ColorPairs_WINDOW_INACTIVE,

        ColorPairs_MENU_ITEM,
        ColorPairs_MENU_ITEM_CURRENT,
        ColorPairs_MENU_ITEM_DISABLED,

        ColorPairs_START_GOAL,
        ColorPairs_PATH,
        ColorPairs_WEIGHT,
        ColorPairs_WALL,

        ColorPairs_ARROWS_LEFT, ColorPairs_ARROWS_UP,
        ColorPairs_ARROWS_RIGHT, ColorPairs_ARROWS_DOWN,

        ColorPairs_COST_0, ColorPairs_COST_1, ColorPairs_COST_2,
        ColorPairs_COST_3, ColorPairs_COST_4, ColorPairs_COST_5,
        ColorPairs_COST_6, ColorPairs_COST_7, ColorPairs_COST_8,
        ColorPairs_COST_9,
    };


    struct Coordinates {
        int x;
        int y;

        Coordinates& operator= (const Coordinates& p) 
        {
            if (this ==  &p)
                return * this;

            x = p.x;
            y = p.y;

            return *this;
        }

        operator graph::Location() const
        {
            return {x, y};
        }
    };

    struct Corners {
        Coordinates top_left;
        Coordinates bottom_right;

        Corners& operator= (const Corners& p)
        {
            if (this ==  &p)
                return * this;

            top_left = p.top_left;
            bottom_right = p.bottom_right;

            return *this;
        }
    };

    enum class TuiAnswerShow {
        none,
        cost,
        came_from,
        going_to,
        path
    };

}
#endif /* TUICOMMON_H */
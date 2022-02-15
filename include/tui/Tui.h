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
#ifndef TUICURSES_H
#define TUICURSES_H   1

#include "common.h"

#include "Board.h"
#include "Menu.h"

#include "../graph/Board.h"          /* graph::Location */

namespace tui {
    inline double heuristic(graph::Location a, graph::Location b);

    class Tui {
    public:
        Tui(int board_rows_, int board_cols_);
        Tui() : Tui(BOARD_ROWS, BOARD_COLS) {};
        ~Tui();

        void show();

        enum class AvailableAlgorithms {
            astar,
            bfs,
            dijkstra
        };
    private:
        enum class Windows {
            board,
            menu
        };

        int board_rows, board_cols;

        Tui::Windows selected_window;
        TuiAnswerShow data_to_display;

        Board* window_board;
        Menu* window_menu;

        void drawstdscr();
        bool userInput(const int key, bool& redraw, bool& exit);

        void calculateWindowsCorners(Corners& board, Corners& menu);

        bool runAlgorithm(Tui::AvailableAlgorithms algorithm);
    };
}
#endif /* TUICURSES_H */
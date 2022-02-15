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
#ifndef TUIBOARD_H
#define TUIBOARD_H  1

#include "common.h"          /* Corners, Coordinates, TuiAnswerShow */
#include "Window.h"          /* TuiWindow */

#include "../graph/Board.h"     /* graph::Board, graph::Location */

namespace tui {
    struct BoardData {
        std::vector<graph::Location> path;
        std::unordered_map<graph::Location, graph::Location> direction;
        std::unordered_map<graph::Location, double> cost;
        double max_cost;

        BoardData& operator= (const BoardData& p) {
            if (this ==  &p)
                return * this;

            path = p.path;
            direction = p.direction;
            cost = p.cost;

            return *this;
        }
    };

    class Board final : public Window {
    public:
        Board(Corners corners_, int board_rows_, int board_cols_);
        ~Board();

        void draw(bool active, void* opt = nullptr);
        bool userInput(const int key, bool& redraw, void* opt = nullptr);

        bool fillWithRandomData();

        bool resetBoard();
        bool clearData();

        const graph::Board& getBoard();

        BoardData& getBoardData();
    private:
        int board_rows, board_cols;
        graph::Board board;
        Coordinates cursor;
        BoardData data;

        chtype getElementSymbol(
            const Coordinates position, const TuiAnswerShow to_show
        ) const;
        chtype getDataElementTypeAt(
            const Coordinates position, const TuiAnswerShow to_show
        ) const;

        ColorPairs getColorPairFor (const chtype symbol) const;

        void updateDataStruct();
        void updateCursor();

        void loadDummyData();
    };
}
#endif /* TUIBOARD_H */
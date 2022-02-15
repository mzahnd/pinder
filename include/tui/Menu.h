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
#ifndef TUIMENU_H
#define TUIMENU_H   1

#include <menu.h>           /* ITEM, MENU */

#include "common.h"
#include "Window.h"

namespace tui {
    class Menu final : public Window {
    public:
        Menu(Corners corners_);
        ~Menu();

        void draw(bool active, void* opt = nullptr);
        bool userInput(const int key, bool& redraw, void* opt);

        enum AvailableOptions {
            none = -1,
            astar = 0,
            bfs,
            dijkstra,
            toggle_path,
            toggle_came_from,
            toggle_going_to,
            toggle_cost,
            random_fill,
            clear,
            exit,
            noptions
        };
    private:
        ITEM ** t_items;
        MENU * t_menu;

        int cursor;

        void onlyOneOpt(Menu::AvailableOptions option);
    };
}
#endif /* TUIMENU_H */
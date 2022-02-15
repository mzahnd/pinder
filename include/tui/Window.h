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
#ifndef TUIWINDOW_H
#define TUIWINDOW_H    1

#include <ncurses.h>        /* WINDOW */
#include <panel.h>          /* PANEL */

#include "common.h"      /* Corners */

namespace tui {

    class Window {
    public:
        Window(Corners corners_);
        ~Window();

        bool moveWindow(Corners position);

        void sendToFront();

        virtual void draw(bool active, void* opt) = 0;
        virtual bool userInput(const int key, bool& redraw, void* opt) = 0;

    protected:
        void drawBorder(bool active);
        WINDOW* getSubWindowPointer();
        WINDOW* getWindowPointer();


        void refresh();

    private:
        Corners corners;

        PANEL* panel_ptr;
        WINDOW* win_ptr;
        WINDOW* subwin_ptr;


        void createWindow();
        void destroyWindow();

        void createPanel();
        void destroyPanel();
    };

}

#endif /* TUIWINDOW_H */
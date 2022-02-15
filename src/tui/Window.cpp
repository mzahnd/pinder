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

#include <cstdlib>          /* std::abs */

#include <ncurses.h>
#include <panel.h>

#include "Window.h"


using namespace tui;


#ifndef SUBWINDOW_Y_PADDING
    #define SUBWINDOW_Y_PADDING     0
#endif

#ifndef SUBWINDOW_X_PADDING
    #define SUBWINDOW_X_PADDING     1
#endif

Window::Window(Corners corners_)
{
    corners = corners_;

    createWindow();
    createPanel();
}

Window::~Window()
{
    destroyPanel();
    destroyWindow();
}

bool Window::moveWindow(Corners position)
{
    int max_row = 0, max_col = 0;
    getmaxyx(stdscr, max_row, max_col);

    if (position.top_left.x < 0
        || position.top_left.y < 0
        || position.bottom_right.x > max_col 
        || position.bottom_right.y > max_row) 
    {
        return false;
    }

    corners = position;

    destroyWindow();
    createWindow();

    return true;
}

void Window::sendToFront()
{
    top_panel(this->panel_ptr);
}

void Window::drawBorder(bool active)
{
    if (active) {
        wattrset(this->win_ptr, COLOR_PAIR(ColorPairs_WINDOW_ACTIVE));
    }
    else {
        wattrset(this->win_ptr, COLOR_PAIR(ColorPairs_WINDOW_INACTIVE));
    }

    // 0, 0 gives default characters for vertical and horizontal lines
    wborder(this->win_ptr, 0, 0, 0, 0, 0, 0, 0, 0);
}

WINDOW* Window::getSubWindowPointer()
{
    return this->subwin_ptr;
}

WINDOW* Window::getWindowPointer()
{
    return this->win_ptr;
}

void Window::refresh()
{
    touchwin(win_ptr);
    wnoutrefresh(subwin_ptr);
    wnoutrefresh(win_ptr);
}

void Window::createWindow()
{
    if (win_ptr != NULL)
        destroyWindow();

    win_ptr = newwin(
        corners.bottom_right.y - corners.top_left.y,
        corners.bottom_right.x - corners.top_left.x,
        corners.top_left.y,
        corners.top_left.x
    );

    // subwin(win, height, width, start_y, start_x);
    subwin_ptr = subwin(
        win_ptr,
        corners.bottom_right.y - corners.top_left.y - 2 * BORDER_PADDING,
        corners.bottom_right.x - corners.top_left.x - 2 * BORDER_PADDING,
        corners.top_left.y + BORDER_PADDING + SUBWINDOW_Y_PADDING,
        corners.top_left.x + BORDER_PADDING + SUBWINDOW_X_PADDING
    );

    keypad(win_ptr, TRUE);      // Control over non-char keys (Ctrl, F1, etc)
    keypad(subwin_ptr, TRUE);   //

    drawBorder(false);

    this->refresh();
}

void Window::destroyWindow()
{
    // Erase the window before destroying
    wborder(win_ptr, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');

    this->refresh();
    delwin(subwin_ptr);
    delwin(win_ptr);
}


void Window::createPanel()
{
    if (panel_ptr != NULL)
        destroyPanel();

    panel_ptr = new_panel(win_ptr);
}

void Window::destroyPanel()
{
    del_panel(panel_ptr);
}
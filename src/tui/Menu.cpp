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
#include <menu.h>

#include "Tui.h"


using namespace tui;



#define MENU_MARK   "  > "

#define ARRAY_SIZE(a)   (sizeof(a) / sizeof(a[0]))

// Menu Options strings. Follows TuiMenu::AvailableOptions enum (!)
const char *choices[] = {
    "A*",
    "BFS",
    "Dijkstra",
    "Show path",
    "Show previous location",
    "Show following location",
    "Show cost",
    "Reset with random data",
    "Clear",
    "Exit",
    (char *)NULL    // Leave this NULL (!)
};

Menu::Menu(Corners corners_)
: Window(corners_)
{
    int n_choices = ARRAY_SIZE(choices);

    t_items = (ITEM **) calloc(n_choices, sizeof(ITEM *));
    if (t_items == NULL)
        return;

    for(int i = 0; i < n_choices; ++i) {
        t_items[i] = new_item(choices[i], "");
    }

    t_menu = new_menu((ITEM **) t_items);

    set_menu_win(t_menu, getWindowPointer());
    set_menu_sub(t_menu, getSubWindowPointer());

    // item_opts_off(t_items[4], O_SELECTABLE);

    set_menu_fore(t_menu, COLOR_PAIR(ColorPairs_MENU_ITEM_CURRENT));
    set_menu_back(t_menu, COLOR_PAIR(ColorPairs_MENU_ITEM));
    set_menu_grey(t_menu, COLOR_PAIR(ColorPairs_MENU_ITEM_DISABLED));

    set_menu_mark(t_menu, MENU_MARK);

    item_opts_off(
        t_items[AvailableOptions::toggle_cost],
        O_SELECTABLE
    );
    item_opts_off(
        t_items[AvailableOptions::toggle_came_from],
        O_SELECTABLE
    );
    item_opts_off(
        t_items[AvailableOptions::toggle_going_to],
        O_SELECTABLE
    );
    item_opts_on(
        t_items[AvailableOptions::toggle_path],
        O_SELECTABLE
    );

    post_menu(t_menu);
}

Menu::~Menu()
{
    int n_choices = ARRAY_SIZE(choices);

    unpost_menu(t_menu);
    free_menu(t_menu);

    for (int i = 0; i < n_choices; ++i) {
        free_item(t_items[i]);
    }

    free(t_items);
}

void Menu::draw(bool active, void* opt)
{
    if(active)
        curs_set(0);

    if (opt != nullptr) {
        TuiAnswerShow* to_show = (TuiAnswerShow *) opt;
        
        switch (*to_show) {
        case TuiAnswerShow::cost:
            onlyOneOpt(toggle_cost);
            break;

        case TuiAnswerShow::came_from:
            onlyOneOpt(toggle_came_from);
            break;

        case TuiAnswerShow::going_to:
            onlyOneOpt(toggle_going_to);
            break;

        case TuiAnswerShow::path:
            onlyOneOpt(toggle_path);
            break;

        case TuiAnswerShow::none:        // Fallsthrough
        default:
            // Set to default
            *to_show = TuiAnswerShow::path;
            break;
        }
    }

    drawBorder(active);

    this->refresh();
}

void Menu::onlyOneOpt(Menu::AvailableOptions option)
{
    item_opts_off(
        t_items[AvailableOptions::toggle_path],
        O_SELECTABLE
    );
    item_opts_off(
        t_items[AvailableOptions::toggle_came_from],
        O_SELECTABLE
    );
    item_opts_off(
        t_items[AvailableOptions::toggle_going_to],
        O_SELECTABLE
    );
    item_opts_off(
        t_items[AvailableOptions::toggle_cost],
        O_SELECTABLE
    );

    item_opts_on(t_items[option], O_SELECTABLE);
}

bool Menu::userInput(const int key, bool& redraw, void* opt)
{
    Menu::AvailableOptions* options = (Menu::AvailableOptions*) opt;
    *options = Menu::AvailableOptions::none;

    switch (key) {
    case KEY_UP:
        if (cursor <= 0) {
            cursor = noptions - 1;
            menu_driver(t_menu, REQ_LAST_ITEM);
        }
        else {
            --cursor;
            menu_driver(t_menu, REQ_PREV_ITEM);
        }
        
        redraw = true;
        break;

    case KEY_DOWN:
        if (cursor >= noptions - 1) {
            cursor = 0;
            menu_driver(t_menu, REQ_FIRST_ITEM);
        }
        else {
            ++cursor;
            menu_driver(t_menu, REQ_NEXT_ITEM);
        }

        redraw = true;
        break;

    case 10:        /* Enter key */
        *options = (Menu::AvailableOptions) cursor;
        break;

    default:
        return false;
        break;
    }

    return true;
}
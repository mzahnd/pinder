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

#include <cstdlib>          /* std::abs (int), printf */
#include <locale.h>         /* setlocale */
#include <ncurses.h>

#ifdef ALLOW_DIAGONALS
#include <cmath>            /* std::sqrt */
#endif

#include "../algorithms/search_algorithm.h"


#include "Tui.h"


using namespace tui;


#define INSTRUCTIONS_HEIGHT 4
#define INFO_TXT_BOARD      "Arrows: Move; S: Start; G: Goal; W: Wall; H: Heavy path; C: Clear location"
#define INFO_TXT_MENU       "Up/Down arrow: Move; Enter: Select"


#define WINDOW_MIN_HEIGHT   (board_rows * CURSOR_Y_OFFSET                   \
                            + 2 * BORDER_PADDING + INSTRUCTIONS_HEIGHT)

#define WINDONW_MIN_WIDTH   (board_cols * CURSOR_X_OFFSET                   \
                            + 2 * BORDER_PADDING                            \
                            + MENU_WIDTH + 2 * BORDER_PADDING               \
                            + WINDOW_SEPARATION)


#define IS_ACTIVE(w)        ((selected_window == Tui::Windows::w) ?   \
                                true : false)

#define DRAW_WINDOW(wname)  (window_##wname->draw(                          \
                                IS_ACTIVE(wname),                           \
                                &data_to_display)                           \
                            )


namespace tui {
    inline int abs(int n)
    {
        return (n > 0) ? n : -1 * n;
    }

#ifdef ALLOW_DIAGONALS
    inline double heuristic(const graph::Location a, const graph::Location b)
#else
    inline double heuristic(const graph::Location a, const graph::Location b)
#endif
    {

#ifdef ALLOW_DIAGONALS
        return std::sqrt(abs(a.x - b.x) + abs(a.y - b.y));
#else
        return abs(a.x - b.x) + abs(a.y - b.y);
#endif

    }
}

Tui::Tui(int board_rows_, int board_cols_)
{
    board_rows = board_rows_;
    board_cols = board_cols_;

    // Basics
    setlocale(LC_ALL, "");  // Unicode support
    initscr();              // Start curses mode

    if (has_colors() == FALSE || can_change_color() == FALSE) {
        endwin();
        printf("Your terminal does not support colors :(\n");
        exit(1);
    }

    if (LINES < WINDOW_MIN_HEIGHT || COLS < WINDONW_MIN_WIDTH) {
        printf("Your terminal must be at leat %dx%d but is %dx%d\n\n",
        WINDOW_MIN_HEIGHT+1, WINDONW_MIN_WIDTH+1, LINES, COLS);
        exit(2);
    }

    raw();                  // Line buffering disabled
    keypad(stdscr, TRUE);   // Control over non-char keys (Ctrl, F1, F2, etc)
    noecho();               // Don't echo() while doing getch

    // Colors
    start_color();

    // Redefinitions
    init_color(COLOR_RED, RGB_COLOR_RED);
    init_color(COLOR_GREEN, RGB_COLOR_GREEN);
    init_color(COLOR_BLUE, RGB_COLOR_BLUE);

    // Custom colors
    init_color(COLOR_GREY, RGB_COLOR_GREY);

    init_color(COLOR_ORANGE, RGB_COLOR_ORANGE);
    init_color(COLOR_REDISH_GREEN, RGB_COLOR_REDISH_GREEN);
    init_color(COLOR_LIGHT_BLUE, RGB_COLOR_LIGHT_BLUE);
    init_color(COLOR_VIOLET, RGB_COLOR_VIOLET);

    // Color pairs
    init_pair(
        ColorPairs_DEFAULT,
        TUI_COLOR_FOREGROUND, TUI_COLOR_BACKGROUND
    );

    init_pair(
        ColorPairs_WINDOW_ACTIVE,
        TUI_COLOR_WINDOW_ACTIVE, TUI_COLOR_BACKGROUND
    );
    init_pair(
        ColorPairs_WINDOW_INACTIVE,
        TUI_COLOR_WINDOW_INACTIVE, TUI_COLOR_BACKGROUND
    );



    init_pair(
        ColorPairs_MENU_ITEM,
        TUI_COLOR_FOREGROUND, TUI_COLOR_BACKGROUND
    );
    init_pair(
        ColorPairs_MENU_ITEM_CURRENT,
        TUI_COLOR_MENU_CURRENT, TUI_COLOR_BACKGROUND
    );
    init_pair(
        ColorPairs_MENU_ITEM_DISABLED,
        TUI_COLOR_MENU_DISABLED, TUI_COLOR_BACKGROUND
    );



    init_pair(
        ColorPairs_START_GOAL,
        TUI_COLOR_START_GOAL, TUI_COLOR_BACKGROUND
    );
    init_pair(ColorPairs_WEIGHT, TUI_COLOR_WEIGHT, TUI_COLOR_BACKGROUND);
    init_pair(ColorPairs_WALL, TUI_COLOR_WALL, TUI_COLOR_BACKGROUND);


    init_pair(ColorPairs_PATH, TUI_COLOR_PATH, TUI_COLOR_BACKGROUND);


    // Arrows
    init_pair(
        ColorPairs_ARROWS_LEFT,
        TUI_COLOR_ARROWS_LEFT, TUI_COLOR_BACKGROUND
    );
    init_pair(
        ColorPairs_ARROWS_UP,
        TUI_COLOR_ARROWS_UP, TUI_COLOR_BACKGROUND
    );
    init_pair(
        ColorPairs_ARROWS_RIGHT,
        TUI_COLOR_ARROWS_RIGHT, TUI_COLOR_BACKGROUND
    );
    init_pair(
        ColorPairs_ARROWS_DOWN,
        TUI_COLOR_ARROWS_DOWN, TUI_COLOR_BACKGROUND
    );


    // Cost (gradient)
    for(int i = 0, j = ColorPairs_COST_0; j <= ColorPairs_COST_9; ++i, ++j) {
        NCURSES_COLOR_T cost_color = COLOR_COST_0 + i;
        
        // Base color is (RGB):     110, 220, 25
        // Final color is (RGB):    218, 112, 25
        init_color(cost_color,
            CHAR_RGB_TO_COLOR_T(110 + i * 12),
            CHAR_RGB_TO_COLOR_T(220 - i * 12),
            CHAR_RGB_TO_COLOR_T(25)
        );
        init_pair(j, cost_color, TUI_COLOR_BACKGROUND);
    }


    // Windows and Panels
    Corners board_corners, menu_corners;

    calculateWindowsCorners(board_corners, menu_corners);

    window_menu = new Menu(menu_corners);
    window_board = new Board(board_corners, board_rows, board_cols);

    // Defaults
    data_to_display = TuiAnswerShow::path;

    update_panels();
}

Tui::~Tui()
{
    reset_color_pairs();

    delete window_board;
    delete window_menu;
    endwin();
}

void Tui::show()
{
    int key = 0;
    bool input = false, redraw = false, exit = false;

    Menu::AvailableOptions menu_opts = Menu::AvailableOptions::none;
    
    selected_window = Tui::Windows::board;

    DRAW_WINDOW(board);
    DRAW_WINDOW(menu);
    drawstdscr();

    update_panels();
    doupdate();

    do {
        key = getch();
        input = userInput(key, redraw, exit);

        if (input) {
            if(redraw) {
                DRAW_WINDOW(board);
                DRAW_WINDOW(menu);
                drawstdscr();
            }
        }
        else {
            switch(selected_window) {
            case Tui::Windows::board:
                window_board->userInput(key, redraw);

                if(redraw) DRAW_WINDOW(board);

                break;

            case Tui::Windows::menu:
                window_menu->userInput(key, redraw, &menu_opts);

                if(redraw) DRAW_WINDOW(menu);

                break;

            default:
                break;
            }

            switch (menu_opts) {
            case Menu::AvailableOptions::astar:
                runAlgorithm(Tui::AvailableAlgorithms::astar);
                break;

            case Menu::AvailableOptions::bfs:
                runAlgorithm(Tui::AvailableAlgorithms::bfs);
                break;

            case Menu::AvailableOptions::dijkstra:
                runAlgorithm(Tui::AvailableAlgorithms::dijkstra);
                break;

            case Menu::AvailableOptions::toggle_path:
                data_to_display = TuiAnswerShow::path;
                DRAW_WINDOW(board);
                DRAW_WINDOW(menu);
                break;

            case Menu::AvailableOptions::toggle_came_from:
                data_to_display = TuiAnswerShow::came_from;
                DRAW_WINDOW(board);
                DRAW_WINDOW(menu);
                break;

            case Menu::AvailableOptions::toggle_going_to:
                data_to_display = TuiAnswerShow::going_to;
                DRAW_WINDOW(board);
                DRAW_WINDOW(menu);
                break;

            case Menu::AvailableOptions::toggle_cost:
                data_to_display = TuiAnswerShow::cost;
                DRAW_WINDOW(board);
                DRAW_WINDOW(menu);
                break;

            case Menu::AvailableOptions::random_fill:
                window_board->fillWithRandomData();

                DRAW_WINDOW(board);
                break;

            case Menu::AvailableOptions::clear:
                window_board->resetBoard();

                DRAW_WINDOW(board);
                break;

            case Menu::AvailableOptions::exit:
                exit = true;
                break;

            case Menu::AvailableOptions::none:   // Fallsthrough
            default:
                break;
            }
        }

        update_panels();
        doupdate();

        redraw = false;
        key = 0;
        menu_opts = Menu::AvailableOptions::none;
    } while(!exit);
}

void Tui::drawstdscr()
{
    move(LINES - 3, 1);
    clrtoeol();

    switch (selected_window) {
    case Tui::Windows::board:
        addstr(INFO_TXT_BOARD);
        break;

    case Tui::Windows::menu:
        addstr(INFO_TXT_MENU);
        break;

    default:
        break;
    }

    mvaddstr(LINES - 2, 1, "TAB: Change window. F1, F2: Exit");
}

bool Tui::userInput(const int key, bool& redraw, bool& exit)
{
    switch (key) {
    case KEY_F(1):
    case KEY_F(2):
        exit = true;
        break;

    case 9:             /* TAB key */
        switch (selected_window) {
        case Tui::Windows::board:
            selected_window = Tui::Windows::menu;
            window_menu->sendToFront();
            break;

        case Tui::Windows::menu:
            selected_window = Tui::Windows::board;
            window_board->sendToFront();
            break;
        }

        redraw = true;
        break;

    default:
        return false;
        break;
    }

    return true;
}

void Tui::calculateWindowsCorners(Corners& board, Corners& menu)
{
    int max_col = 0, max_row = 0;
    getmaxyx(stdscr, max_row, max_col);

    board.top_left.x = (max_col
        - board_cols * CURSOR_X_OFFSET - 2 * BORDER_PADDING
        - MENU_WIDTH - 2 * BORDER_PADDING - WINDOW_SEPARATION
    ) / 2;
    board.top_left.y = (max_row 
        - board_rows * CURSOR_Y_OFFSET - 2 * BORDER_PADDING
        - INSTRUCTIONS_HEIGHT
    ) / 2;

    board.bottom_right.x = board.top_left.x
        + board_cols * CURSOR_X_OFFSET + 2 * BORDER_PADDING;
    board.bottom_right.y = board.top_left.y
        + board_rows * CURSOR_Y_OFFSET + 2 * BORDER_PADDING;


    menu.top_left.x = board.bottom_right.x + WINDOW_SEPARATION;
    menu.top_left.y = (max_row 
        - MENU_HEIGHT - 2 * BORDER_PADDING
        - INSTRUCTIONS_HEIGHT
    ) / 2;

    menu.bottom_right.x = menu.top_left.x
        + MENU_WIDTH + 2 * BORDER_PADDING;
    menu.bottom_right.y = menu.top_left.y
        + MENU_HEIGHT + 2 * BORDER_PADDING;
}

bool Tui::runAlgorithm(Tui::AvailableAlgorithms algorithm)
{
    bool path_found = false;
    BoardData& board_data = window_board->getBoardData();

    window_board->clearData();

    switch(algorithm) {
        case Tui::AvailableAlgorithms::astar:
        a_star_search(
            window_board->getBoard(),
            board_data.direction,
            board_data.cost,
            &heuristic
        );
        break;

    case Tui::AvailableAlgorithms::bfs:
        breadth_first_search(
            window_board->getBoard(),
            board_data.direction
        );
        break;

    case Tui::AvailableAlgorithms::dijkstra:
        dijkstra_search(
            window_board->getBoard(),
            board_data.direction,
            board_data.cost
        );
        break;
    }

    path_found = search_reconstruct_path(
        window_board->getBoard(),
        board_data.direction,
        board_data.path
    );
    DRAW_WINDOW(board);

    return path_found;
}
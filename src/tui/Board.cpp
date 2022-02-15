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

#include <algorithm>        /* find */
#include <cmath>            /* std::round */
#include <cstdlib>          /* std::abs ; std::rand */
#include <ctime>            /* std::time */
#include <stdexcept>        /* std::out_of_range */

#include <ncurses.h>

#include "Tui.h"            /* tui namespace */

#include "Board.h"


using namespace tui;



#define SUBWINDOW_Y_PADDING     0
#define SUBWINDOW_X_PADDING     1


#define SYMBOL_EMPTY        '.'
#define SYMBOL_START        'S'
#define SYMBOL_GOAL         'G'
#define SYMBOL_PATH         '@'
#define SYMBOL_WALL         '#'
#define SYMBOL_WEIGHT       ':'
#define SYMBOL_ARROW_LEFT   '<'
#define SYMBOL_ARROW_UP     '^'
#define SYMBOL_ARROW_RIGHT  '>'
#define SYMBOL_ARROW_DOWN   'v'

// Maximum cost that can be printed using a single character
#define MAX_COST        9

#define BOARD_AREA                      (board_cols * board_rows)
#define RANDOM_COORD_ROW                std::rand() % board_rows
#define RANDOM_COORD_COLUMN             std::rand() % board_cols

#define PRINT_SYM_AT(win, y, x, sym)    mvwaddch(                   \
                                            (win),                  \
                                            (y) * CURSOR_Y_OFFSET,  \
                                            (x) * CURSOR_X_OFFSET,  \
                                            (sym)                   \
                                        )

// Get cost value between 0 and 9 (both inclusive) to print a single char
#define NORMALIZE_COST_0TO9(current)   (std::round(                 \
                                            (current) * MAX_COST    \
                                            / data.max_cost)        \
                                        )

#define DIGIT_TO_CHAR(n)  ('0' + (n))

Board::Board(Corners corners_, int board_rows_, int board_cols_)
: Window(corners_), board(board_cols_, board_rows_)
{
    board_rows = board_rows_;
    board_cols = board_cols_;

    cursor = {0, 0};

    // Random seed
    std::srand(std::time(nullptr));

    loadDummyData();
}

Board::~Board()
{
    return;
}

void Board::draw(bool active, void* opt)
{
    WINDOW* subwin_ptr = getSubWindowPointer();
    
    chtype symbol = SYMBOL_EMPTY;
    ColorPairs cpair = ColorPairs_DEFAULT;

    TuiAnswerShow to_show = (opt == nullptr) 
        ? TuiAnswerShow::none 
        : *((TuiAnswerShow*) opt);


    wattrset(subwin_ptr, COLOR_PAIR(ColorPairs_DEFAULT));
    updateDataStruct();

    if (active) {
        curs_set(1);
        updateCursor();
    }

    for (int y = 0; y < board_rows; ++y) {
        for (int x = 0; x < board_cols; ++x ) {
            symbol = getElementSymbol({x,y}, to_show);

            cpair = getColorPairFor(symbol);

            if(cpair != ColorPairs_DEFAULT) 
                wattron(subwin_ptr, COLOR_PAIR(cpair));
            
            PRINT_SYM_AT(subwin_ptr, y, x, symbol);
            
            if(cpair != ColorPairs_DEFAULT)
                wattroff(subwin_ptr, COLOR_PAIR(cpair));
        }
    }

    drawBorder(active);
}

bool Board::userInput(const int key, bool& redraw, void* opt)
{
    switch (key) {
    case KEY_UP:
        cursor.y = (cursor.y == 0) ? board_rows - 1 : cursor.y - 1;
        updateCursor();
        break;
    case KEY_DOWN:
        cursor.y = (cursor.y == board_rows - 1) ? 0 : cursor.y + 1;
        updateCursor();
        break;
    case KEY_LEFT:
        cursor.x = (cursor.x == 0) ? board_cols - 1 : cursor.x - 1;
        updateCursor();
        break;
    case KEY_RIGHT:
        cursor.x = (cursor.x == board_cols - 1) ? 0 : cursor.x + 1;
        updateCursor();
        break;

    case 's': /* Fallsthrough */
    case 'S':
        if (board.setStart({cursor.x, cursor.y}))
            clearData();

        redraw = true;
        break;
    case 'g': /* Fallsthrough */
    case 'G':
        if (board.setGoal({cursor.x, cursor.y}))
            clearData();

        redraw = true;
        break;
    case 'w': /* Fallsthrough */
    case 'W':
        if (board.toggleWall({cursor.x, cursor.y}))
            clearData();

        redraw = true;
        break;
    case 'h': /* Fallsthrough */
    case 'H':
        if (board.toggleWeight({cursor.x, cursor.y}))
            clearData();

        redraw = true;
        break;
    case 'c': /* Fallsthrough */
    case 'C':
        if(board.setEmpty({cursor.x, cursor.y}))
            clearData();

        redraw = true;
        break;
    default:
        return false;
        break;
    }

    return true;
}

bool Board::resetBoard()
{
    board.clear();
    clearData();

    return true;
}

bool Board::clearData()
{
    data.direction.clear();
    data.cost.clear();
    data.path.clear();
    data.max_cost = 0;

    return true;
}

bool Board::fillWithRandomData()
{
    bool ans = resetBoard();
    loadDummyData();

    return ans;
}

const graph::Board& Board::getBoard()
{
    return this->board;
}

BoardData& Board::getBoardData()
{
    return this->data;
}

chtype Board::getElementSymbol(
    const Coordinates position, const TuiAnswerShow to_show
) const
{
    chtype symbol = SYMBOL_EMPTY;

    switch(board.getElementTypeAt((graph::Location) position)) {
    case graph::Board::ElementType::START:
        symbol = SYMBOL_START;
        break;
    case graph::Board::ElementType::GOAL:
        symbol = SYMBOL_GOAL;
        break;
    case graph::Board::ElementType::WALL:
        symbol = SYMBOL_WALL;
        break;
    case graph::Board::ElementType::WEIGHT:
        symbol = SYMBOL_WEIGHT;
        break;
    case graph::Board::ElementType::EMPTY: /* Fallsthrough */
    default:
        symbol = SYMBOL_EMPTY;
        break;
    }

    if (symbol != SYMBOL_START && symbol != SYMBOL_GOAL) {
        char tmp = getDataElementTypeAt(position, to_show);
        if (tmp != SYMBOL_EMPTY)
            symbol = tmp;
    }

    return symbol;
}

chtype Board::getDataElementTypeAt(
    const Coordinates position, const TuiAnswerShow to_show
) const
{
    double current_cost = 0;        // Must be defined outside switch
    graph::Location next = {0, 0};

    chtype symbol = SYMBOL_EMPTY;

    switch (to_show) {
    case TuiAnswerShow::cost:
        if(data.cost.empty()) {
            break;
        }

        try {
            current_cost = data.cost.at((graph::Location) position);
            symbol = DIGIT_TO_CHAR((int) NORMALIZE_COST_0TO9(current_cost));
        }
        catch (std::out_of_range& e) {
            symbol = SYMBOL_EMPTY;
        }
        break;
    case TuiAnswerShow::came_from:          // Fallsthrough
    case TuiAnswerShow::going_to:
        if(data.direction.empty()) {
            break;
        }
        try {
            next = data.direction.at((graph::Location) position);
            if (next.x == position.x + 1) {
                symbol = (to_show == TuiAnswerShow::came_from)
                            ? SYMBOL_ARROW_RIGHT : SYMBOL_ARROW_LEFT;
            }
            else if (next.x == position.x - 1) {
                symbol = (to_show == TuiAnswerShow::came_from)
                            ? SYMBOL_ARROW_LEFT : SYMBOL_ARROW_RIGHT; 
            }
            else if (next.y == position.y + 1) {
                symbol = (to_show == TuiAnswerShow::came_from)
                            ? SYMBOL_ARROW_DOWN : SYMBOL_ARROW_UP;
            }
            else if (next.y == position.y - 1) {
                symbol = (to_show == TuiAnswerShow::came_from)
                            ? SYMBOL_ARROW_UP : SYMBOL_ARROW_DOWN;
            }
        }
        catch (std::out_of_range& e) {
            symbol = SYMBOL_EMPTY;
        }
        break;
    case TuiAnswerShow::path:
        if  (find(data.path.begin(), 
                    data.path.end(),
                    position
                )
                != data.path.end()
            )
        {
            symbol = SYMBOL_PATH;
        }
        break;
    
    case TuiAnswerShow::none:       // Fallsthrough
    default:
        symbol = SYMBOL_EMPTY;
        break;
    }

    return symbol;
}

ColorPairs Board::getColorPairFor (const chtype symbol) const
{
    ColorPairs pair = ColorPairs_DEFAULT;

    switch (symbol) {
    case SYMBOL_START:  // Fallsthrough
    case SYMBOL_GOAL:
        pair = ColorPairs_START_GOAL;
        break;

    case SYMBOL_WALL:
        pair = ColorPairs_WALL;
        break;

    case SYMBOL_WEIGHT:
        pair = ColorPairs_WEIGHT;
        break;

    case SYMBOL_PATH:
        pair = ColorPairs_PATH;
        break;

    // Arrows
    case SYMBOL_ARROW_LEFT:     pair = ColorPairs_ARROWS_LEFT;  break;
    case SYMBOL_ARROW_UP:       pair = ColorPairs_ARROWS_UP;    break;
    case SYMBOL_ARROW_RIGHT:    pair = ColorPairs_ARROWS_RIGHT; break;
    case SYMBOL_ARROW_DOWN:     pair = ColorPairs_ARROWS_DOWN;  break;

    // Cost
    case '0': pair = ColorPairs_COST_0; break;
    case '1': pair = ColorPairs_COST_1; break;
    case '2': pair = ColorPairs_COST_2; break;
    case '3': pair = ColorPairs_COST_3; break;
    case '4': pair = ColorPairs_COST_4; break;
    case '5': pair = ColorPairs_COST_5; break;
    case '6': pair = ColorPairs_COST_6; break;
    case '7': pair = ColorPairs_COST_7; break;
    case '8': pair = ColorPairs_COST_8; break;
    case '9': pair = ColorPairs_COST_9; break;


    default:
        pair = ColorPairs_DEFAULT;
        break;
    }

    return pair;
}

void Board::updateDataStruct()
{
    if (!data.cost.empty() && data.max_cost == 0) {
        // Update max_cost data
        for(const auto& kv : data.cost) {
            if (data.max_cost < kv.second)
                data.max_cost = kv.second;
        }
    }
}

void Board::updateCursor()
{
    wmove(
        getWindowPointer(),
        cursor.y * CURSOR_Y_OFFSET + SUBWINDOW_Y_PADDING + 1,
        cursor.x * CURSOR_X_OFFSET + SUBWINDOW_X_PADDING + 1
    );
}


void Board::loadDummyData()
{
    int n_weights = std::rand() % (BOARD_AREA / 4), 
        n_walls = std::rand() % (BOARD_AREA / 6);

    graph::Location start = {RANDOM_COORD_COLUMN, RANDOM_COORD_ROW};
    graph::Location end = {RANDOM_COORD_COLUMN, RANDOM_COORD_ROW};

    while (start == end)
        end = {RANDOM_COORD_COLUMN, RANDOM_COORD_ROW};

    board.setStart(start);
    board.setGoal(end);

    for (int i = 0; i < n_walls; ++i)
        board.setWall({RANDOM_COORD_COLUMN, RANDOM_COORD_ROW});


    for (int i = 0; i < n_weights; i++)
        board.setWeight({RANDOM_COORD_COLUMN, RANDOM_COORD_ROW});
}
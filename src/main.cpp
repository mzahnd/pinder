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

#include <cstdlib>      /* atoi */
#include <iostream>

#include "tui/Tui.h"

void print_help()
{
    std::cerr
        << "pinder [ROWS] [COLUMNS]\n\n"
        << "If ROWS and COLUMNS is not provided, they take values "
        << BOARD_ROWS << " and " << BOARD_COLS << " respectively.\n"
        << "The minimum accepted value is " << BOARD_ROWS_MIN 
        << " for both dimensions.\n";
}

int main(int argc, char* argv[])
{
    if (argc > 1) {
        if (argc != 3) {
            std::cerr << "Invalid number of arguments.\n\n";
            print_help();

            exit(1);
        }

        int rows = atoi(argv[1]), cols = atoi(argv[2]);

        if (rows < BOARD_ROWS_MIN || cols < BOARD_ROWS_MIN) {
            std::cerr << "Invalid size for board.\n\n";
            print_help();

            exit(1);
        }

        tui::Tui tui(rows, cols);
        tui.show();
    }
    else {
        tui::Tui tui;
        tui.show();
    }

    return 0;
}

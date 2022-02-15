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
#ifndef BOARD_H
#define BOARD_H   1

#include <array>            /* std::array           */
#include <tuple>            /* std::tie             */
#include <unordered_map>    /* std::unordered_map   */
#include <unordered_set>    /* std::unordered_set   */
#include <vector>           /* std::vector          */

namespace graph {

#ifdef ALLOW_DIAGONALS
#define N_DIRS              8
#else
#define N_DIRS              4
#endif

    struct Location {
        int x;
        int y;

        Location& operator= (const Location& p)
        {
            if (this ==  &p)
                return * this;

            x = p.x;
            y = p.y;

            return *this;
        }

        bool operator== (const Location& p) const
        {
            return x == p.x && y == p.y;
        }

        bool operator!= (const Location& p) const
        {
            return !(*this == p);
        }

        bool operator< (const Location& p) const
        {
            return std::tie(x, y) < std::tie(p.x, p.y);
        }
        bool operator> (const Location& p) const
        {
            return p < *this;
        }
    };
}

namespace std {
    template<>
        struct hash<graph::Location> {
        std::size_t operator()(const graph::Location& position) const noexcept
        {
            // Using ElegantPairing as described by Matthew Szudzik 
            // http://szudzik.com/ElegantPairing.pdf
            return std::hash<int>()(
                    position.x >= position.y ? 
                    position.x * position.x + position.x + position.y :
                    position.x + position.y * position.y 
                );
        }
    };
}

namespace graph {
    class Board {
    public:
        typedef Location location_t;    // Simplifies algorithms code

        enum class ElementType {
            EMPTY = 0,
            START,
            GOAL,
            WALL,
            WEIGHT,
            PATH
        };

        Board(int rows_, int columns_);

        bool in_bounds(const Location position) const;
        bool passable(const Location position) const;
        
        bool isStartGoal(const Location position) const;

        std::vector<Location> neighbors(const Location position) const;
        double cost(const Location from, const Location to) const;


        void clear();

        const Location& getStart() const;
        const Location& getGoal() const;

        bool setStart (const Location position);
        bool setGoal (const Location position);
        bool setWall (const Location position);
        bool setWeight (const Location position);
        bool setEmpty (const Location position);

        bool toggleWall (const Location position);
        bool toggleWeight (const Location position);
        
        ElementType getElementTypeAt(const Location position) const;
    private:
        static std::array<Location, N_DIRS> DIRS;

        int rows, columns;

        Location start;
        Location goal;

        std::unordered_set<Location> walls;
        std::unordered_set<Location> weights;
    };
}



#endif /* BOARD_H */


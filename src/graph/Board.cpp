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

#include <algorithm>        /* std::reverse */
#include <iostream>         /* printf */

#include "Board.h"

using namespace graph;

std::array<Location, N_DIRS> Board::DIRS = {
    /* East, West, North, South */
    Location{1, 0}, Location{-1, 0},
    Location{0, -1}, Location{0, 1},
#ifdef ALLOW_DIAGONALS
    /* North-east, north-west, south-east, south-west */
    Location{1, -1}, Location{-1, -1},
    Location{1, -1}, Location{-1, 1}
#endif
};

Board::Board(int rows_, int columns_)
{
    this->rows = rows_;
    this->columns = columns_;
}

bool Board::in_bounds(const Location position) const
{
    return position.x >= 0 && position.x < columns 
        && position.y >= 0 && position.y < rows;
}

bool Board::passable(const Location position) const
{
    // position is NOT a wall (not in the set)
    return walls.find(position) == walls.end();
}

bool Board::isStartGoal(const Location position) const
{
    // position is NOT start or goal
    return position == start || position == goal;
}

std::vector<Location> Board::neighbors(const Location position) const
{
    std::vector<Location> results;
    Location next = {0, 0};

    for(Location direction : DIRS) {
        next.x = position.x + direction.x;
        next.y = position.y + direction.y;

        if (in_bounds(next) && passable(next)) {
            results.push_back(next);
        }
    }

    if ((position.x + position.y) % 2 == 0) {
        // Make paths look nicer
        std::reverse(results.begin(), results.end());
    }

    return results;
}

double Board::cost(const Location from, const Location to) const
{
    return weights.find(to) != weights.end() ? 5 : 1;
}


void Board::clear()
{
    walls.clear();
    weights.clear();
}

const Location& Board::getStart() const
{
    return this->start;
}

const Location& Board::getGoal() const
{
    return this->goal;
}

bool Board::setStart(const Location position)
{
    if (!in_bounds(position))
        return false;
    
    setEmpty(position);

    start.x = position.x;
    start.y = position.y;

    return true;
}

bool Board::setGoal(const Location position)
{
    if (! in_bounds(position))
        return false;

    setEmpty(position);

    goal.x = position.x;
    goal.y = position.y;

    return true;
}

bool Board::setWall(const Location position)
{
    if (!in_bounds(position) || isStartGoal(position))
        return false;

    walls.insert(position);

    return true;
}

bool Board::setWeight(const Location position)
{
    if (!in_bounds(position))
        return false;

    weights.insert(position);

    return true;
}

bool Board::setEmpty(const Location position)
{
    if (!in_bounds(position))
        return false;

    std::unordered_set<Location>::iterator search;

    if ((search = walls.find(position)) != walls.end()) {
        walls.erase(search);
    }
    else if ((search = weights.find(position)) != weights.end()) {
        weights.erase(search);
    }

    return true;
}

bool Board::toggleWall(const Location position)
{
    if (!in_bounds(position) || isStartGoal(position))
        return false;

    std::unordered_set<Location>::iterator search;
    if ((search = walls.find(position)) != walls.end()) {
        walls.erase(search);
    }
    else {
        setEmpty(position);
        setWall(position);
    }

    return true;
}

bool Board::toggleWeight(const Location position)
{
    if (!in_bounds(position))
        return false;

    std::unordered_set<Location>::iterator search;
    if ((search = weights.find(position)) != weights.end()) {
        weights.erase(search);
    }
    else {
        setEmpty(position);
        setWeight(position);
    }

    return true;
}

Board::ElementType Board::getElementTypeAt(const Location position) const
{
    ElementType element = ElementType::EMPTY;
    if (walls.find(position) != walls.end()) {
        element = ElementType::WALL;
    }
    else if (position == start) {
        element = ElementType::START;
    }
    else if (position == goal) {
        element = ElementType::GOAL;
    }
    else if (weights.find(position) != weights.end()) {
        element = ElementType::WEIGHT;
    }
    else {
        element = ElementType::EMPTY;
    }

    return element;
}
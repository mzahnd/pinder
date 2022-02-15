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
#ifndef SEARCH_ALGORITHM_H
#define SEARCH_ALGORITHM_H  1

#include <stdexcept>        /* std::out_of_range */

#include "a_star.h"
#include "bfs.h"
#include "dijkstra.h"


template<typename Graph>
bool
search_reconstruct_path(
    const Graph& graph,
    std::unordered_map<typename Graph::location_t, typename Graph::location_t>& came_from,
    std::vector<typename Graph::location_t>& path
)
{
    typedef typename Graph::location_t Location;

    bool found_path = true;

    const Location& start = graph.getStart();
    const Location& goal = graph.getGoal();
    Location current = goal;

    while (current != start) {
        path.push_back(current);

        try {
            current = came_from.at(current);
        }
        catch (std::out_of_range& e) {
            found_path = false;

            path.clear();
            break;
        }
    }

    if (found_path) {
        path.push_back(start); // Optional
        std::reverse(path.begin(), path.end());
    }

    return found_path;
}

#endif /* SEARCH_ALGORITHM_H */
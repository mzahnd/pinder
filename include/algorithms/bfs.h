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
#ifndef BFS_H
#define BFS_H   1

#include <queue>            /* std::queue           */
#include <unordered_map>    /* std::unordered_map   */

template<typename Graph>
void
breadth_first_search(
    const Graph& graph,
    std::unordered_map<typename Graph::location_t, typename Graph::location_t>& came_from
)
{
    typedef typename Graph::location_t Location;

    const Location& start = graph.getStart();
    const Location& goal = graph.getGoal();
    Location current = {};

    std::queue<Location> frontier;
    frontier.push(start);

    came_from[start] = start;

    while (!frontier.empty()) {
        current = frontier.front();
        frontier.pop();

        // Early exit
        if (current == goal)
            break;


        for (Location next: graph.neighbors(current)) {
            if (came_from.find(next) == came_from.end()) {
                frontier.push(next);
                came_from[next] = current;
            }
        }
    }
}

#endif /* BFS_H */

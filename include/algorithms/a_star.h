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
#ifndef ASTAR_H
#define ASTAR_H     1

#include <algorithm>        /* std::reverse         */
#include <queue>            /* std::priority_queue  */
#include <unordered_map>    /* std::unordered_map   */

#include "PriorityQueue.h"

template<typename Graph, typename CostType>
void
a_star_search (
    const Graph& graph, 
    std::unordered_map<typename Graph::location_t, typename Graph::location_t>& came_from,
    std::unordered_map<typename Graph::location_t, CostType>& cost_so_far,
    CostType (*heuristic) (
        const typename Graph::location_t a,
        const typename Graph::location_t b
    )
)
{
    typedef typename Graph::location_t Location;

    const Location& start = graph.getStart();
    const Location& goal = graph.getGoal();
    Location current = {};


    PriorityQueue<Location, CostType> frontier;
    frontier.put(start, 0);


    came_from[start] = start;
    cost_so_far[start] = 0;


    CostType new_cost, priority;

    while (!frontier.empty()) {
        current = frontier.get();
        
        // Early exit
        if (current == goal)
            break;

        for (Location next: graph.neighbors(current)) {
            new_cost = cost_so_far[current] + graph.cost(current, next);

            if (came_from.find(next) == came_from.end() 
                || new_cost < cost_so_far[next])
            {
                cost_so_far[next] = new_cost;
                came_from[next] = current;

                priority = new_cost + heuristic(next, goal);

                frontier.put(next, priority);
            }
        }
    }
}

#endif /* ASTAR_H */
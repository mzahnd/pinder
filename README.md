# Pinder

This project is just an excuse to learn some basic path-finding algorithms,
cmake and ncurses, as well as refreshing my C++ coding.

# Compilation

_TODO_

Debug mode compilation is enabled by default. There's a line to change it to
Release in the `CMakeList.txt` file in the root directory.

Also the default behaviour of the path finding alrotihms allows moving through
diagonals, which can be changed by removing `ALLOW_DIAGONALS` definition from
the `CMakeList.txt` file inside the `src/` folder.

# Sources
I used
[Introduction to the A* Algorithm from Red Blob Games](https://www.redblobgames.com/pathfinding/a-star/introduction.html)
as reference for understanding and coding the algorithms (a good part of the
code is actually borrowed from there).

Wikipedia's
[A* search algorithm](https://en.wikipedia.org/wiki/A*_search_algorithm),
[Breadth-first search](https://en.wikipedia.org/wiki/Breadth-first_search) and
[Dijkstra's algorithm](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm)
articles were also of great help.

For ncurses,
[NCURSES Programming HOWTO by Pradeep Padala](https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/)
was my guide alongside the project documentation (`man ncurses.3x`).
The hardest part was defining more colors, but I could figure it out after
reading 
[About ncurses Colors by Jim Hall](https://www.linuxjournal.com/content/about-ncurses-colors-0)
and noticing that the standard colors go from index 0 to 15 and high intensity
colors are not defined in ncurses (or so I think).

# License
![GPLv3 Logo](https://www.gnu.org/graphics/gplv3-with-text-136x68.png)

>This file is part of Pinder.
>
>Pinder is free software: you can redistribute it and/or modify
>it under the terms of the GNU General Public License as published by
>the Free Software Foundation, either version 3 of the License, or
>(at your option) any later version.
>
>Pinder is distributed in the hope that it will be useful,
>but WITHOUT ANY WARRANTY; without even the implied warranty of
>MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
>GNU General Public License for more details.
>
>You should have received a copy of the GNU General Public License
>along with Pinder.  If not, see <https://www.gnu.org/licenses/>.

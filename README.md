# MultiAgentPathFinding

The aim of the MultiAgentPathFinding repository is to create realizations of different path planning algorithms with flexible environment options, such as metric type and possible directions of movement. The project is made in the second year of HSE Faculty of Computer Science.

### Build on Linux and Mac

You can choose "Debug" or "Release" build.

```bash
cd MultiAgentPathFinding/Build/Release
cmake ../../ -DCMAKE_BUILD_TYPE="Release"
make
make install
```

To run use:

```bash
cd ../../Bin/{Debug|Release}/
Dmitriy_Shepelin_ASearch ../../Examples/example.xml
```

### Build on Windows

PATH must contain cmake and migw32-make.

```bash
cd MultiAgentPathFinding/Build/Release
cmake ../../ -DCMAKE_BUILD_TYPE="Release" -G "MinGW Makefiles"
mingw32-make
mingw32-make install
```

"Release" can be changed to "Debug".

To run use:

```bash
cd ../../Bin/{Debug|Release}/
Dmitriy_Shepelin_ASearch.exe ../../Examples/example.xml
```

### Input and output

A path is constructed on a 2D plane with traversable and non-traversable cells. Each task is given as an XML file. The format is shown in *Examples/example.xml*. This file is expected to have these fields:

- *map* describes the geometry of the plane and obstacles on it. You can choose size of the cell grid, locations of obstacles, start and finish locations. Positions start in the left upper corner with coordinates (0, 0).
- *algorithm* describes options of path planning:
  - *searchtype* can be "dijkstra" or "astar" to use Dijkstra or A* algorithm respectively;
  - *metrictype* describes heuristic to use (if A* is chosen):
    - diagonal — diagonal metric;
    - manhattan — Manhattan distance;
    - euclidean — Euclidean distance;
    - chebyshev — Chebyshev distance;
  - *hweight* is a multiplicator for heuristic;
  - *allowdiagonal* is true or false and allows the algorithm to consider paths through diagonals;
  - *cutcorners* is true or false and allows to use diagonals which intersect with one non-traversable cell on the way.
  - allowsqueeze is true or false and allows to use diagonal if it intersects two two non-traversable cells.

The output of the program is the same file with the "_log" suffix. It will show information about the search in the *log* field.

- *mapfilename* shows the original path to the file with task description.
- *summary time* is number of seconds which the program ran.
- *length_scaled* is the length of the found path with the map's *cellsize* multiplicator.
- *length* is the same but without multiplicator.
- *nodescreated* is the number of cells which the algorithm considered and created nodes for.
- *numberofsteps* is the number of nodes which the algorithm worked with (expanded and examined) in the path planning loop.
-  *path* draws a path from start to finish if it was found *.
- *lplevel* and *hplevel* describe the path and short and long versions respectively.

### Mentors

A big thanks to the mentors on this project!

[**Konstantin Yakovlev**](http://kyakovlev.me/ru/home/)

[**Stepan Dergachev**](https://github.com/haiot4105)


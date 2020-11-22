# MultiAgentPathFinding

Project is made It is being made in the second year of HSE Faculty of Computer Science.

#### Build on Linux and Mac

```bash
cd PathPlanningProject
cd Build
cd Release
cmake ../../ -DCMAKE_BUILD_TYPE="Release"
make
make install
```

"Release" can be changed to "Debug".

To run use:

```bash
cd ../../Bin/{Debug|Release}/
Dmitriy_Shepelin_ASearch ../../Examples/example.xml
```

#### Build on Windows

PATH should contain cmake and migw32-make.

```bash
cd PathPlanningProject
cd Build
cd Release
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

#### Debug with Visual Studio

You can use VS interaction with CMake to build, run and debug this project. To use command-line arguments you can edit *args* in *configurations* field of *.vs/launch.vs.json*.

